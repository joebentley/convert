#include <ctype.h>
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

  // Whether to print the unit in output
  int show_unit = 1;

  // Whether to put/allow space between unit and value
  int allow_space = 0;

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
      format = CONTEXT_EXPONENTIAL;
      args_parsed++;
    }
    if (!strcmp(argv[i], "-fe")) {
      format = FORCED_EXPONENTIAL;
      args_parsed++;
    }
    if (!strcmp(argv[i], "-fd")) {
      format = DECIMAL;
      args_parsed++;
    }

    // Don't display unit in output
    if (!strcmp(argv[i], "-u")) {
      show_unit = 0;
      args_parsed++;
    }

    // Allow space between value and unit
    if (!strcmp(argv[i], "-s")) {
      allow_space = 1;
      args_parsed++;
    }
  }

  char raw_number[1000];

  // If we have consumed all the arguments, use stdin as
  // there is no value argument
  if (args_parsed == argc - 1) {
    fgets(raw_number, 1000, stdin);
  } else {
    strcpy(raw_number, argv[argc - 1]);
  }


  char *value;
  char *unit;
  if (allow_space) {
    // Split string by spaces, the first string will be the
    // number, the second will be the unit
    value = strtok(raw_number, " ");

    char *u = strtok(NULL, " ");
    // Check that the unit isn't null (that a unit is given)
    if (u) {
      unit = u;
    } else {
      fprintf(stderr, "No unit given in input string (no space found)\n");
      return -1;
    }
  } else {
    value = malloc(1000 * sizeof(char));
    unit  = malloc(1000 * sizeof(char));
    // Walk backwards through string until number found,
    // all the characters we walk through are our units
    for (int i = strlen(raw_number); i > 0; i--) {
      // Spaces aren't allowed
      if (isspace(raw_number[i])) {
        fprintf(stderr, "Spaces in input requires -s flag\n");
        return -1;
      }

      if (isdigit(raw_number[i])) {
        // Fill value with all chars before and including raw_number[i]
        strncpy(value, raw_number, i + 1);
        // Fill unit with all chars after raw_number[i]
        strncpy(unit, raw_number + i + 1, strlen(raw_number) - i - 1);
        break;
      }
    }
  }

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

  char buffer[100];

  switch (format) {
    case CONTEXT_EXPONENTIAL:
      snprintf(buffer, 100, "%Lg", input->value);
      break;
    case FORCED_EXPONENTIAL:
      snprintf(buffer, 100, "%Le", input->value);
      break;
    case DECIMAL:
      snprintf(buffer, 100, "%Lf", input->value);
      break;
  }

  if (show_unit) {
    if (allow_space) {
      printf("%s %s\n", buffer, unit_to_short_str(output));
    } else {
      printf("%s%s\n", buffer, unit_to_short_str(output));
    }
  } else {
    printf("%s\n", buffer);
  }

  return 0;
}

