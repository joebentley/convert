#include <stdlib.h>
#include <string.h>
#include "unit.h"

int conversion_factor(unit_t input, unit_t output, double *factor);

int convert(value_t *value, unit_t output) {
  double factor;
  if (conversion_factor(value->unit, output, &factor) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  value->value = value->value * factor;
  return EXIT_SUCCESS;
}

int conversion_factor(unit_t input, unit_t output, double *factor) {
  if (input == METRES) {
    switch (output) {
      case METRES:
        *factor = 1;
        break;
      case INCHES:
        *factor = INCHES_IN_METRE;
        break;
      case FEET:
        *factor = FEET_IN_METRE;
        break;
      case MILES:
        *factor = MILES_IN_METRE;
        break;
      default:
        return EXIT_FAILURE;
    }
  }

  if (input == INCHES) {
    switch (output) {
      case INCHES:
        *factor = 1;
        break;
      case METRES:
        *factor = 1./INCHES_IN_METRE;
        break;
      case FEET:
        *factor = 1./INCHES_IN_FOOT;
        break;
      case MILES:
        *factor = 1./(INCHES_IN_FOOT * FEET_IN_MILE);
        break;
      default:
        return EXIT_FAILURE;
    }
  }

  if (input == FEET) {
    switch (output) {
      case FEET:
        *factor = 1;
        break;
      case METRES:
        *factor = 1./FEET_IN_METRE;
        break;
      case INCHES:
        *factor = INCHES_IN_FOOT;
        break;
      case MILES:
        *factor = 1./FEET_IN_MILE;
        break;
      default:
        return EXIT_FAILURE;
    }
  }

  if (input == MILES) {
    switch (output) {
      case MILES:
        *factor = 1;
        break;
      case METRES:
        *factor = 1./MILES_IN_METRE;
        break;
      case INCHES:
        *factor = INCHES_IN_FOOT * FEET_IN_MILE;
        break;
      case FEET:
        *factor = FEET_IN_MILE;
        break;
      default:
        return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}


value_t *new_value() {
  value_t *out = malloc(sizeof(value_t));
  out->value = 0;
  out->unit = NONE;
  return out;
}


unit_t str_to_unit(char* to_convert) {
  if (!strcmp(to_convert, "metres")) {
    return METRES;
  }
  if (!strcmp(to_convert, "miles")) {
    return MILES;
  }
  if (!strcmp(to_convert, "feet")) {
    return FEET;
  }
  if (!strcmp(to_convert, "inches")) {
    return INCHES;
  }

  return NONE;
}

const char *unit_strs[] = { "inches", "feet", "miles", "metres", "none" };

const char *unit_to_str(unit_t unit) {
  return unit_strs[unit];
}
