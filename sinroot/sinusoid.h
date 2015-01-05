#pragma once

#include "float_format.h"

class Sinusoid{
public:
	Sinusoid();
	Sinusoid(bool linear_, FLOAT_FMT r_, FLOAT_FMT theta_, FLOAT_FMT omega_);
	~Sinusoid();

	void setParameters(bool linear_, FLOAT_FMT r_, FLOAT_FMT theta_, FLOAT_FMT omega_);
	FLOAT_FMT value(FLOAT_FMT t);
	FLOAT_FMT getLLBFGamma(FLOAT_FMT intervalA, FLOAT_FMT intervalB);

private:
	void init();
	bool linear;
	FLOAT_FMT r;
	FLOAT_FMT theta;
	FLOAT_FMT omega;

};
