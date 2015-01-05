#include "sinusoid.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define M_2_x_PI M_PI*2

Sinusoid::Sinusoid(){
	init();
}

Sinusoid::Sinusoid(bool linear_, FLOAT_FMT r_, FLOAT_FMT theta_, FLOAT_FMT omega_){
	init();
	setParameters(linear_, r_, theta_, omega_);
}

Sinusoid::~Sinusoid(){
	//
}

void Sinusoid::setParameters(bool linear_, FLOAT_FMT r_, FLOAT_FMT theta_, FLOAT_FMT omega_){
	linear = linear_;
	r = r_;
	theta = theta_;
	omega = omega_;
}

FLOAT_FMT Sinusoid::value(FLOAT_FMT t){
	FLOAT_FMT arg;
	arg = theta + omega * t;
	if (linear){
		return sin(arg) * r;
	}
	else{
		return t * sin(arg) * r;
	}
}

FLOAT_FMT Sinusoid::getLLBFGamma(FLOAT_FMT intervalA, FLOAT_FMT intervalB){
	FLOAT_FMT testDeriv;
	FLOAT_FMT ptSlope;
	FLOAT_FMT yA;
	FLOAT_FMT xA;
	FLOAT_FMT a;
	FLOAT_FMT b;
	FLOAT_FMT delta;
	FLOAT_FMT sinLBF;

	intervalA = intervalA*omega + theta;
	intervalB = intervalB*omega + theta;

	a = M_PI - intervalB;
	b = M_PI - intervalA;

	delta = b - a;
	b = fmod(b, M_2_x_PI);
	a = b - delta;

	testDeriv = cos(b);
	if (a <= -1){
		yA = -1;
	}
	else if (a <= 0){
		yA = a;
	}
	else{
		yA = sin(a);
	}

	xA = fmax(a, -1);

	ptSlope = (yA - sin(b)) / (xA - b);
	
	sinLBF = -fmax(testDeriv, ptSlope) * omega * r;
	return sinLBF * (linear ? (b - a) : 1);
}

void Sinusoid::init(){
	//
}