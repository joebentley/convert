#include <unistd.h>
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

  char c;
  while ((c = getopt(argc, argv, "i:o:f:us")) != -1) {
    switch (c) {
      // Input unit
      case 'i':
        {
          unit_t input_unit = str_to_unit(optarg);
          if (input_unit == NONE) {
            fprintf(stderr, "Input unit not recognized\n");
            return -1;
          }
          input->unit = input_unit;
          break;
        }

      // Output unit
      case 'o':
        {
          output = str_to_unit(optarg);
          if (output == NONE) {
            fprintf(stderr, "Output unit not recognized\n");
            return -1;
          }
          break;
        }

      // Output format flags
      case 'f':
        {
          switch(optarg[0]) {
            case 'c':
              format = CONTEXT_EXPONENTIAL;
              break;
            case 'e':
              format = FORCED_EXPONENTIAL;
              break;
            case 'd':
              format = DECIMAL;
              break;
          }
          break;
        }

      // Don't display unit in output
      case 'u':
        show_unit = 0;
        break;

      // Allow space between value and unit
      case 's':
        allow_space = 1;
        break;

      // Print usage string
      default:
        {
          fprintf(stderr, "\nUsage: convert [<options>] [value]\n\n");
          fprintf(stderr, "Options:\n");
          fprintf(stderr, "-i [unit]  set input unit to [unit]\n");
          fprintf(stderr, "-o [unit]  set output unit to [unit]\n");
          fprintf(stderr, "-f [style] select output style\n");
          fprintf(stderr, "-s         allow spaces in input and output\n");
          fprintf(stderr, "-u         don't display unit in output\n");
          fprintf(stderr, "--help     show this message\n\n");
          return -1;
        }
    }
  }

  char raw_number[1000];

  // If we have consumed all the arguments, use stdin as
  // there is no value argument
  if (optind == argc) {
    fgets(raw_number, 1000, stdin);
  } else {
    strcpy(raw_number, argv[optind]);
  }

  // Strip trailing newline from input
  size_t len = strlen(raw_number) - 1;
  if (raw_number[len] == '\n') {
    raw_number[len] = '\0';
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

  if (strlen(value) > 0) {
    sscanf(value, "%Lf", &input->value);
  } else {
    fprintf(stderr, "No value given\n");
    return -1;
  }

  if (strlen(unit) > 0) {
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

