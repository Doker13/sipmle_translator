#include <locale.h>
#include <stdio.h>
#include <wchar.h>

#include <libtranslator/parser.h>

int amount_of_arguments(int argc) {
  if (argc > 4) {
    printf("Слишком большое количество аргуметов, используйте "
           "форму:\n'./translate text_rus.txt dictionary.txt text_eng.txt'\n");
    return -1;
  } else if (argc < 4) {
    printf("Недостаточно аргуметов, используйте форму:\n'./translate "
           "text_rus.txt dictionary.txt text_eng.txt'\n");
    return -1;
  } else {
    return 0;
  }
}

int is_letter_is_rus(wchar_t letter) {
  if ((letter >= 1040 && letter <= 1103) || letter == 1105 || letter == 1025) {
    return 0;
  }
  return -1;
}

int is_letter_is_eng(wchar_t letter) {
  if ((letter >= 65 && letter <= 90) || (letter >= 97 && letter <= 122)) {
    return 0;
  }
  return -1;
}

int language_define(wchar_t letter, int flag) {
  if (flag == 0) {
    if (is_letter_is_rus(letter) == 0) {
      return 1;
    } else {
      return 0;
    }
  } else if (flag == 1 && is_letter_is_eng(letter) == 0) {
    return -1;
  } else if (flag == 0 && is_letter_is_eng(letter) == 0) {
    return -1;
  }
  return flag;
}

int is_letters_separate(wchar_t *string) {
  for (int i = 0; string[i] != '\0'; i++) {
    if ((is_letter_is_rus(string[i]) == 0 &&
         is_letter_is_eng(string[i + 1]) == 0) ||
        (is_letter_is_rus(string[i + 1]) == 0 &&
         is_letter_is_eng(string[i]) == 0)) {
      return -1;
    }
  }
  return 0;
}

int string_check(wchar_t *string) {
  int sep = 0;
  int space = 0;
  int dash = 0;
  if (string[0] == 45 || string[0] == 32) {
    return -1;
  }
  for (int i = 0; string[i] != '\n' && string[i] != '\0'; i++) {
    if (string[i] == 45 || string[i] == 32 ||
        is_letter_is_eng(string[i]) == 0 || is_letter_is_rus(string[i]) == 0) {
      if (string[i] == 45 && string[i + 1] == 32 && string[i - 1] == 32 &&
          (is_letter_is_eng(string[i - 2]) == 0 ||
           is_letter_is_rus(string[i - 2]) == 0) &&
          (is_letter_is_eng(string[i + 2]) == 0 ||
           is_letter_is_rus(string[i + 2]) == 0)) {
        sep++;
      }
      if (string[i] == 45) {
        dash++;
      }
      if (string[i] == 32) {
        space++;
      }
    } else {
      return -1;
    }
  }
  if (sep == 1 && space == 2 && dash == 1) {
    return 0;
  }
  return -1;
}

int dictionary_check(char *name) {
  FILE *fp = fopen(name, "r");
  if (!fp) {
    printf("Словарь не найден\n");
    return -1;
  }
  wchar_t string[100];
  int count = 0;
  int flag = 0;
  while (fgetws(string, 100, fp) != NULL) {
    count++;
    if (string_check(string) == -1 || is_letters_separate(string) == -1) {
      printf(
          "Ошибка в %d строке словаря, используйте форму:\n'Слово - Перевод'\n",
          count);
          fclose(fp);
      return -1;
    }
    flag = language_define(string[0], flag);
    if (flag == -1) {
      printf("В %d строке словаря неправильное следование слов, используйте "
             "форму:\n'Слово - Перевод'\n",
             count);
             fclose(fp);
      return -1;
    }
  }
  fclose(fp);
  return 0;
}
