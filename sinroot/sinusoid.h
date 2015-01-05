#pragma once

#include "float_format.h"

class Sinusoid{
public:
	Sinusoid();
	Sinusoid(bool linear_, FLOAT_FMT r_, FLOAT_FMT theta_, FLOAT_FMT omega_);
	~Sinusoid();

	void setParameters(bool linear_, FLOAT_FMT r_, FLOAT_FMT theta_, FLOAT_FMT omega_);
	FLOAT_FMT value(FLOAT_FMT t, int deriv = 0);
	FLOAT_FMT derivRoot(int root);
	bool isConst();
	bool isLinear();
	FLOAT_FMT getOmega();

private:
	void init();
	FLOAT_FMT sinusDerivRoot(int root);
	FLOAT_FMT linearSinusDerivRoot(int root);

	bool linear;
	bool linRootOffset;

	FLOAT_FMT r;
	FLOAT_FMT theta;
	FLOAT_FMT omega;

	FLOAT_FMT d1_r;
	FLOAT_FMT d2_r;

	FLOAT_FMT d2_l_r1;
	FLOAT_FMT d2_l_r2;

	FLOAT_FMT sroot1;
};
