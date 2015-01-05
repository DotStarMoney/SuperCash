#include "rootsolve.h"
#include "float_format.h"

using namespace SuperCash;
using namespace NSolve;

SC_FLOAT rootSolveLBF(NFunction& f, SC_FLOAT x0, SC_FLOAT delta, SC_FLOAT expStep){
	SC_FLOAT f_x;
	SC_FLOAT x;
	SC_FLOAT slope;
	SC_FLOAT interval;
	bool isLower;

	x = x0;
	f_x = f.value(x);
	if (f_x >= 0){
		isLower = true;
	}
	else{
		isLower = false;
	}
	interval = 0;

	while(!SC_FLOAT_PREC_ZERO(f_x)){

		slope = f.getLLBFGamma(x, x + delta, isLower);
		if (slope != 0.0){
			interval = -f_x / slope;
		}

		if ((interval > 0) && (interval < delta)){
			x += interval;
			delta = interval;
		}
		else{
			x += delta;
			delta *= expStep;
		}

		f_x = f.value(x);

	}
}

