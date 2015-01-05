#pragma once

#include "float_format.h"

class Sinusoid;

class CompositeSinusoid{
public:
	CompositeSinusoid();
	CompositeSinusoid(int N_, bool* linear_array_, FLOAT_FMT* r_array_, FLOAT_FMT* theta_array_, FLOAT_FMT* omega_array_);
	~CompositeSinusoid();

	void setParameters(int N_, bool* linear_array_, FLOAT_FMT* r_array_, FLOAT_FMT* theta_array_, FLOAT_FMT* omega_array_);

	FLOAT_FMT value(FLOAT_FMT t, int deriv = 0, int degree = -1);
	int findRootsInterval(FLOAT_FMT t_end, FLOAT_FMT** roots);
	int getComponentsN();

private:
	void init();
	Sinusoid* waveParts;
	int       waveParts_N;
};