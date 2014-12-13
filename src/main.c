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

  // Count how many arguments have been parsed so far so we
  // can work out if we've consumed them all.
  int args_parsed = 0;

  for (int i = 1; i < argc; i++) {
    // Only parse if there is more than 1 argument
    if (argc < 2) {
      break;
    }
   
    // Input unit 
    if (!strcmp(argv[i], "-i")) {
      args_parsed += 2;

      unit_t input_unit = str_to_unit(argv[i + 1]);
      if (input_unit == NONE) {
        fprintf(stderr, "Input unit not recognized\n");
        return -1;
      }

      input->unit = input_unit;
    }

    // Output unit
    if (!strcmp(argv[i], "-o")) {
      args_parsed += 2;
      output = str_to_unit(argv[i + 1]);
      if (output == NONE) {
        fprintf(stderr, "Output unit not recognized\n");
        return -1;
      }
    }

    // Output format flags
    if (!strcmp(argv[i], "-fc")) {
      args_parsed++;
      format = CONTEXT_EXPONENTIAL;
    }
    if (!strcmp(argv[i], "-fe")) {
      args_parsed++;
      format = FORCED_EXPONENTIAL;
    }
    if (!strcmp(argv[i], "-fd")) {
      args_parsed++;
      format = DECIMAL;
    }
  }

  char *raw_number = malloc(1000 * sizeof(char));

  // If we have consumed all the arguments, use stdin as
  // there is no value argument
  if (args_parsed == argc - 1) {
    fgets(raw_number, 1000, stdin);
  } else {
    raw_number = argv[argc - 1];
  }

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

  // Strip trailing newline from unit
  size_t len = strlen(unit) - 1;
  if (unit[len] == '\n') {
    unit[len] = '\0';
  }

  if (unit) {
    // If result is NONE, unit not recognized
    if ((input->unit = str_to_unit(unit)) == NONE) {
      fprintf(stderr, "Unit not recognized\n");
      return -1;
    }
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

