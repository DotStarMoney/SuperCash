#pragma once

#define SC_FLOAT double
#define SC_FLOAT_PREC_SHIFT 4
#define SC_FLOAT_PREC_MASK  0x3FFFFFFFFFFF
#define SC_FLOAT_PREC_BITS(x) ((*((__int64*) &(x)) >> SC_FLOAT_PREC_SHIFT) & SC_FLOAT_PREC_MASK)
#define SC_FLOAT_PREC_EQL(x, y) (SC_FLOAT_PREC_BITS(x) == SC_FLOAT_PREC_BITS(y))
