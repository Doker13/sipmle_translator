#pragma once

typedef struct {
  wchar_t *word;
  wchar_t *translation;
} dictionary;

wchar_t *delwchar(wchar_t *str, int number, int count);
int wcmp(wchar_t *str, wchar_t *ptr, int n);
size_t first_word_size(FILE *fp);
size_t second_word_size(FILE *fp);
int ammount_of_lines(char *name);
dictionary *create_dictionary(char *name, int lines);
void dictionary_free(int lines, dictionary *d);
wchar_t *input_reader(char *name);
void input_free(wchar_t *input);
void output_free(wchar_t *output);
wchar_t *word_replace(wchar_t *output, wchar_t *word, wchar_t *translation);
wchar_t *replacer(wchar_t *input, dictionary *d, int lines);
int printer(wchar_t *str, char *name);
