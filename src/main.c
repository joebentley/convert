
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "unit.h"


int main(int argc, char *argv[]) {
  value_t *input = calloc(1, sizeof(value_t));
  unit_t output = NONE;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-i")) {
      unit_t input_unit = str_to_unit(argv[i + 1]);
      if (input_unit == NONE) {
        fprintf(stderr, "Input unit not recognized\n");
        return -1;
      }
     
      input->unit = input_unit;
      // Check if the user gave value argument, if not use stdin 
      if (argc > 5) {
        sscanf(argv[argc - 1], "%lf", &input->value);
      } else {
        char number[100];
        fgets(number, 100, stdin);
        sscanf(number, "%lf", &input->value);
      }
    }

    if (!strcmp(argv[i], "-f")) {
      output = str_to_unit(argv[i + 1]);
      if (output == NONE) {
        fprintf(stderr, "Output unit not recognized\n");
        return -1;
      }
    }
  }

  if (input->unit == NONE) {
    fprintf(stderr, "No input unit given\n");
    return -1;
  }
  if (output == NONE) {
    fprintf(stderr, "No output unit given\n");
    return -1;
  }

  // Convert to new unit
  convert(input, output);

  printf("%f\n", input->value);

  return 0;
}

