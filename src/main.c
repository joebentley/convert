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


  // Check if the user gave value argument, if not use stdin 
  char *raw_number = argv[argc - 1];
  // TODO: Fix stdin

  // Split string by spaces, the first string will be the
  // number, the second will be the unit
  char *value = strtok(raw_number, " ");
  char *unit = strtok(NULL, " ");

  if (value) {
    sscanf(value, "%Lf", &input->value);
  } else {
    fprintf(stderr, "No value given\n");
    return -1;
  }

  if (unit) {
    input->unit = str_to_unit(unit);
  } else if (input->unit == NONE) {
    fprintf(stderr, "No unit given\n");
    return -1;
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
      printf("%Lg\n", input->value);
      break;
    case FORCED_EXPONENTIAL:
      printf("%Le\n", input->value);
      break;
    case DECIMAL:
      printf("%Lf\n", input->value);
      break;
  }

  return 0;
}

