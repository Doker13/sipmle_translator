#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include <libtranslator/parser.h>
#include <libtranslator/translator.h>

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  if (amount_of_arguments(argc) == -1) {
    return -1;
  }
  if (dictionary_check(argv[2]) == -1) {
    return -1;
  }
  int lines = ammount_of_lines(argv[2]);

  dictionary *d = NULL;
  d = create_dictionary(argv[2], lines);

  wchar_t *input = input_reader(argv[1]);
  if (input == NULL) {
    dictionary_free(lines, d);
    return -1;
  }
  wchar_t *output = replacer(input, d, lines);
  if (output == NULL) {
    return -1;
  }
  if (printer(output, argv[3]) == -1) {
    dictionary_free(lines, d);
    input_free(input);
    output_free(output);
    return -1;
  }
  dictionary_free(lines, d);
  input_free(input);
  output_free(output);
  return 0;
}

