#pragma once


#define INCHES_IN_FOOT 12
#define FEET_IN_MILE 5280

#define INCHES_IN_METRE 39.3700787
#define FEET_IN_METRE 3.2808399
#define MILES_IN_METRE 0.000621371192


typedef enum {
  METRES,
  INCHES,
  FEET,
  MILES,
  NONE
} unit_t;

unit_t str_to_unit(char *to_convert);
const char *unit_to_str(unit_t unit);
const char *unit_to_short_str(unit_t unit);


typedef struct {
  long double value;
  unit_t unit;
} value_t;

value_t *new_value();


// Convert to value in input to the unit given
int convert(value_t *input, unit_t unit);


