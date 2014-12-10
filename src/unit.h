#pragma once


#define INCHES_IN_METRE 39.3700787
#define FEET_IN_METRE 3.2808399
#define MILES_IN_METRE 0.000621371192


typedef enum {
  INCHES,
  FEET,
  MILES,
  METRES,
  NONE
} unit_t;

typedef struct {
  double value;
  unit_t unit;
} value_t;


unit_t str_to_unit(char *to_convert);

const char *unit_to_str(unit_t unit);

// Convert to value in input to the unit given
void convert(value_t *input, unit_t unit);

