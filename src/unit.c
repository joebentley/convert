#include <string.h>
#include "unit.h"

double conversion_factor(unit_t input, unit_t output);

void convert(value_t *value, unit_t output) {
  value->value = value->value * conversion_factor(value->unit, output);
}

double conversion_factor(unit_t input, unit_t output) {
  if (input == METRES) {
    switch (output) {
      case INCHES:
        return INCHES_IN_METRE;
      case FEET:
        return FEET_IN_METRE;
      case MILES:
        return MILES_IN_METRE;
      default:
        return 0;
    }
  }

  return 0;
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
