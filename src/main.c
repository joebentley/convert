
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "unit.h"
#include "format.h"


int main(int argc, char *argv[]) {
  value_t *input = new_value();
  unit_t output = NONE;

  // Print in context-sensitive exponential format by default
  format_t format = CONTEXT_EXPONENTIAL;

  for (int i = 1; i < argc; i++) {
    // Only parse if there is more than 1 argument
    if (argc < 2) {
      break;
    }
   
    // Input unit 
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
      } else if (argc > 4) { // Check that at least input, output given
        char number[100];
        fgets(number, 100, stdin);
        sscanf(number, "%lf", &input->value);
      }
    }

    // Output unit
    if (!strcmp(argv[i], "-o")) {
      output = str_to_unit(argv[i + 1]);
      if (output == NONE) {
        fprintf(stderr, "Output unit not recognized\n");
        return -1;
      }
    }

    // Output format flags
    if (!strcmp(argv[i], "-fc")) {
      format = CONTEXT_EXPONENTIAL;
    }
    if (!strcmp(argv[i], "-fe")) {
      format = FORCED_EXPONENTIAL;
    }
    if (!strcmp(argv[i], "-fd")) {
      format = DECIMAL;
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
  if (convert(input, output) == EXIT_FAILURE) {
    fprintf(stderr, "Conversion Failed!\n");
    return -1;
  }

  switch (format) {
    case CONTEXT_EXPONENTIAL:
      printf("%g\n", input->value);
      break;
    case FORCED_EXPONENTIAL:
      printf("%e\n", input->value);
      break;
    case DECIMAL:
      printf("%f\n", input->value);
      break;
  }

  return 0;
}

