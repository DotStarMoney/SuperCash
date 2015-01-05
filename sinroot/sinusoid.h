#pragma once

#include "float_format.h"
#include "NFunction.h"

namespace SuperCash{
	namespace NSolve{

		class Sinusoid : public NFunction{
		public:
			Sinusoid();
			Sinusoid(bool linear_, SC_FLOAT r_, SC_FLOAT theta_, SC_FLOAT omega_);
			~Sinusoid();

			void setParameters(bool linear_, SC_FLOAT r_, SC_FLOAT theta_, SC_FLOAT omega_);
			SC_FLOAT getR();
			SC_FLOAT getTheta();
			SC_FLOAT getOmega();
			bool getLinear();
			void setR(SC_FLOAT r_);
			void setTheta(SC_FLOAT theta_);
			void setOmega(SC_FLOAT omega_);
			void setLinear(bool linear_);


			SC_FLOAT value(SC_FLOAT t);
			SC_FLOAT getLLBFGamma(SC_FLOAT intervalA, SC_FLOAT intervalB, bool lowerBound);
		private:
			void init();
			bool linear;
			SC_FLOAT r;
			SC_FLOAT theta;
			SC_FLOAT omega;

		};

	}
}