#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include <libtranslator/parser.h>
#include <libtranslator/translator.h>

wchar_t *delwchar(wchar_t *str, int number, int count) {
  for (int i = 0; i != count; i++) {
    for (int j = number; str[j] != '\0'; j++) {
      str[j] = str[j + 1];
    }
  }
  return str;
}

wchar_t *movewchar(wchar_t *str, int n, int count) {
  for (int i = 0; i != count; i++) {
    wcscat(str, L" ");
    for (int j = wcslen(str)-1; j != n; j--){
      str[j] = str[j - 1];
    }
  }
  return str;
}

int wcmp(wchar_t *str, wchar_t *ptr, int n) {
  int j = 0;
  for (int i = n; ptr[j] != '\0'; i++) {
    if (towlower(ptr[j]) != towlower(str[i])) {
      return -1;
    }
    j++;
  }
  return 0;
}

size_t first_word_size(FILE *fp) {
  size_t count = 0;
  for (wchar_t c = getc(fp); c != ' '; c = getc(fp)) {
    count++;
  }
  return count;
}

size_t second_word_size(FILE *fp) {
  size_t count = 0;
  for (wchar_t c = getc(fp); c != '\n' && c != EOF; c = getc(fp)) {
    count++;
  }
  return count;
}

int ammount_of_lines(char *name) {
  FILE *fp = fopen(name, "r");
  int count = 0;
  for (char c = getc(fp); c != EOF; c = getc(fp)) {
    if (c == '\n' || c == '\0') {
      count++;
    }
  }
  fclose(fp);
  return count;
}

dictionary *create_dictionary(char *name, int lines) {
  FILE *fp = fopen(name, "r");
  dictionary *d = NULL;
  d = malloc(lines * sizeof(*d));
  if (!d) {
    fclose(fp);
    return NULL;
  }
  for (int i = 0; i != lines; i++) {
    size_t size1 = first_word_size(fp);
    size_t size2 = second_word_size(fp);
    fseek(fp, -size1 - size2 - 2, SEEK_CUR);
    size1++;
    d[i].word = calloc((size1 + 1), sizeof(wchar_t));
    d[i].translation = calloc((size2 + 1), sizeof(wchar_t));
    if (!d[i].word || !d[i].translation) {
      dictionary_free(i - 1, d);
      fclose(fp);
      return NULL;
    }
    fscanf(fp, "%ls - %ls\n", d[i].word, d[i].translation);
  }
  fclose(fp);
  return d;
}

void dictionary_free(int lines, dictionary *d) {
  for (int i = 0; i != lines; i++) {
    free(d[i].word);
    free(d[i].translation);
  }
  free(d);
}

wchar_t *input_reader(char *name) {
  FILE *fin = fopen(name, "r");
  if (!fin) {
    printf("Файл для перевода не найден\n");
    return NULL;
  }
  fseek(fin, 0, SEEK_END);
  size_t size = ftell(fin);
  fseek(fin, 0, SEEK_SET);
  char *tmp = calloc(size + 1, sizeof(char));
  if (!tmp) {
    fclose(fin);
    return NULL;
  }
  char *str = calloc(size + 1, sizeof(char));
  if (!str) {
    free(tmp);
    fclose(fin);
    return NULL;
  }
  while (fgets(str, 100, fin) != NULL) {
    strcat(tmp, str);
  }
  wchar_t *input = calloc((size + 1), sizeof(wchar_t));
  if (!input) {
    fclose(fin);
    free(str);
    free(tmp);
    return NULL;
  }
  mbstowcs(input, tmp, size + 1);
  free(tmp);
  free(str);
  fclose(fin);
  return input;
}

void input_free(wchar_t *input) { free(input); }

void output_free(wchar_t *output) { free(output); }

wchar_t *word_replace(wchar_t *output, wchar_t *word, wchar_t *translation) {
  for (int i = 0; output[i] != '\0'; i++) {
    if (towlower(output[i]) == towlower(word[0]) &&
        wcmp(output, word, i) == 0 &&
        is_letter_is_rus(output[i + wcslen(word)]) == -1 &&
        is_letter_is_eng(output[i + wcslen(word)]) == -1) {
      if (output[i] == towlower(word[0])) {
        output[i] = towlower(translation[0]);
      } else if (output[i] == towupper(word[0])) {
        output[i] = towupper(translation[0]);
      } else {
        output[i] = translation[0];
      }
      if (wcslen(word) == wcslen(translation)) {
        int k = 1;
        for (int j = i + 1; translation[k] != '\0'; j++) {
          output[j] = towlower(translation[k]);
          k++;
        }
      } else if (wcslen(word) > wcslen(translation)) {
        output = delwchar(output, i + 1, wcslen(word) - wcslen(translation));
        int k = 1;
        for (int j = i + 1; translation[k] != '\0'; j++) {
          output[j] = towlower(translation[k]);
          k++;
        }
      } else {
        output = realloc(output, (wcslen(translation) + wcslen(output) + 1) * sizeof(wchar_t));
        if (!output){
        return NULL;
        }
        output = movewchar(output, i + wcslen(word), wcslen(translation) - wcslen(word));
        int k = 1;
        for (int j = i + 1; translation[k] != '\0'; j++) {
          output[j] = towlower(translation[k]);
          k++;
        }
      }
    }
  }
  return output;
}

wchar_t *replacer(wchar_t *input, dictionary *d, int lines) {
  wchar_t *output = calloc(wcslen(input) + 1, sizeof(wchar_t));
  if (!output) {
    input_free(input);
    dictionary_free(lines, d);
    return NULL;
  }
  const wchar_t *cinput = input;
  wcscpy(output, cinput);
  for (int i = 0; i != lines; i++) {
    output = word_replace(output, d[i].word, d[i].translation);
    if (output == NULL) {
      dictionary_free(lines, d);
      input_free(input);
      return NULL;
    }
  }
  return output;
}

int printer(wchar_t *str, char *name){
  FILE *fout = fopen(name, "w");
  if (!fout) {
    printf("Ошибка при создании файла записи\n");
    return -1;
  }
  fputws(str, fout);
  fclose(fout);
  return 0;
}
