#pragma once

/*
 * CONTEXT_EXPONENTIAL - Print in exponential format only for big numbers
 * FORCED_EXPONENTIAL  - Print in exponential format always
 * DECIMAL             - Print in decimal format always
 */

typedef enum {
  CONTEXT_EXPONENTIAL,
  FORCED_EXPONENTIAL,
  DECIMAL
} format_t;
