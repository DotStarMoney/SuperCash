#pragma once

#define FLOAT_FMT double
#define FLOAT_PREC_SHIFT 8
#define FLOAT_PREC_MASK  0xFFFFFFFFFFF
#define FLOAT_PREC_BITS(x) ((*((__int64*) &(x)) >> FLOAT_PREC_SHIFT) & FLOAT_PREC_MASK)
#define FLOAT_PREC_EQL(x, y) (FLOAT_PREC_BITS(x) == FLOAT_PREC_BITS(y))
