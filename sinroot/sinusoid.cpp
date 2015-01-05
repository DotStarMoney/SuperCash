#include "sinusoid.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
	theta = fmod(theta_, 2*M_PI);
	omega = omega_;

	d1_r = r * omega;
	d2_r = -d1_r * omega;

	d2_l_r1 = 2 * d1_r;
	d2_l_r2 = d2_r;

	if (theta >= M_PI_2){
		sroot1 = M_PI_2 - theta + M_PI;
	}
	else{
		sroot1 = M_PI_2 - theta;
	}
	
	if (linear){
		if((theta == 0) || (signbit(value(0, 1)) != signbit(value(sinusDerivRoot(1), 1)))){
			linRootOffset = true;
		}
		else{
			linRootOffset = false;
		}
	}
}

FLOAT_FMT Sinusoid::value(FLOAT_FMT t, int deriv){
	FLOAT_FMT arg;
	arg = theta + omega * t;
	if (linear){
		switch (deriv){
		case 0:
			return r*t*sin(arg);
		case 1:
			return r*(sin(arg) + omega*t*cos(arg));
		case 2:
			return d2_l_r1*cos(arg) + d2_l_r2*t*sin(arg);
		}
	}
	else{
		switch (deriv){
		case 0:
			return r*sin(arg);
		case 1:
			return d1_r*cos(arg);
		case 2:
			return d2_r*sin(arg);
		}
	}
}

FLOAT_FMT Sinusoid::sinusDerivRoot(int root){
	return (sroot1 + (root - 1) * M_PI) / omega;
}
	
FLOAT_FMT Sinusoid::linearSinusDerivRoot(int root){
	FLOAT_FMT root_x;
	FLOAT_FMT last_root_x;
	
	if (root >= 1){
		if (linRootOffset){
			root--;
			if (root <= 0){
				root_x = 0;
			}
			else{
				root_x = sinusDerivRoot(root);
			}
		}
		else{
			root_x = sinusDerivRoot(root);
		}
	}
	else{
		if (!linRootOffset){
			root++;
			if (root > 0){
				root_x = 0;
			}
			else{
				root_x = sinusDerivRoot(root);
			}
		}
		else{
			root_x = sinusDerivRoot(root);
		}
	}

	
	do {
		last_root_x = root_x;
		root_x -= value(root_x, 1) / value(root_x, 2);

	} while (!FLOAT_PREC_EQL(root_x, last_root_x));
	
	return root_x;
}

bool Sinusoid::isConst(){
	if (omega != 0){
		return false;
	}
	else{
		return true;
	}
}

bool Sinusoid::isLinear(){
	return linear;
}

FLOAT_FMT Sinusoid::derivRoot(int root){
	if (linear){
		return linearSinusDerivRoot(root);
	}
	else{
		return sinusDerivRoot(root);
	}
}

FLOAT_FMT Sinusoid::getOmega(){
	return omega;
}

void Sinusoid::init(){
	//
}