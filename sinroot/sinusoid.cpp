#include "sinusoid.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define M_2_x_PI M_PI*2

using namespace SuperCash;
using namespace NSolve;
using namespace Model;

Sinusoid::Sinusoid(){
	init();
}

Sinusoid::Sinusoid(bool linear_, SC_FLOAT r_, SC_FLOAT theta_, SC_FLOAT omega_){
	init();
	setParameters(linear_, r_, theta_, omega_);
}

Sinusoid::~Sinusoid(){
	//
}

void Sinusoid::setParameters(bool linear_, SC_FLOAT r_, SC_FLOAT theta_, SC_FLOAT omega_){
	linear = linear_;
	r = r_;
	theta = theta_;
	omega = omega_;
}

SC_FLOAT Sinusoid::getR(){
	return r;
}
SC_FLOAT Sinusoid::getTheta(){
	return theta;
}
SC_FLOAT Sinusoid::getOmega(){
	return omega;
}
bool Sinusoid::getLinear(){
	return linear;
}
void Sinusoid::setR(SC_FLOAT r_){
	r = r_;
}
void Sinusoid::setTheta(SC_FLOAT theta_){
	theta = theta_;
}
void Sinusoid::setOmega(SC_FLOAT omega_){
	omega = omega_;
}
void Sinusoid::setLinear(bool linear_){
	linear = linear_;
}

SC_FLOAT Sinusoid::value(SC_FLOAT t){
	SC_FLOAT arg;
	arg = theta + omega * t;
	if (linear){
		return sin(arg) * r;
	}
	else{
		return t * sin(arg) * r;
	}
}

SC_FLOAT Sinusoid::getLLBFGamma(SC_FLOAT intervalA, SC_FLOAT intervalB, bool lowerBound){
	SC_FLOAT testDeriv;
	SC_FLOAT ptSlope;
	SC_FLOAT yA;
	SC_FLOAT xA;
	SC_FLOAT a;
	SC_FLOAT b;
	SC_FLOAT delta;
	SC_FLOAT sinLBF;

	intervalA = intervalA*omega + theta;
	intervalB = intervalB*omega + theta;

	a = M_PI - intervalB;
	b = M_PI - intervalA;

	if ((r < 0) ^ (lowerBound == false)){
		a += M_PI;
		b += M_PI;
	}

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
	
	sinLBF = -fmax(testDeriv, ptSlope) * omega * abs(r) * (lowerBound ? 1 : -1);
	return sinLBF * (linear ? (b - a) : 1);
}

void Sinusoid::init(){
	//
}