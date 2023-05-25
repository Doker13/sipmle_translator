#pragma once

int amount_of_arguments(int argc);
int is_letter_is_rus(wchar_t letter);
int is_letter_is_eng(wchar_t letter);
int language_define(wchar_t letter, int flag);
int is_letters_separate(wchar_t *string);
int string_check(wchar_t *string);
int dictionary_check(char *name);
