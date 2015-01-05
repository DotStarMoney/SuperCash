#include "compositesinusoid.h"
#include "sinusoid.h"
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>

#define INCLUDE_EDGE_NODES 4

// TODO: Fix linearly scaled sines, producing bizzare values.

#include <iostream>



CompositeSinusoid::CompositeSinusoid(){
	init();
}

CompositeSinusoid::CompositeSinusoid(int N_, bool* linear_array_, FLOAT_FMT* r_array_, FLOAT_FMT* theta_array_, FLOAT_FMT* omega_array_){
	init();
	setParameters(N_, linear_array_, r_array_, theta_array_, omega_array_);
}

CompositeSinusoid::~CompositeSinusoid(){
	if (waveParts) delete[] waveParts;
}

void CompositeSinusoid::setParameters(int N_, bool* linear_array_, FLOAT_FMT* r_array_, FLOAT_FMT* theta_array_, FLOAT_FMT* omega_array_){
	int i;

	waveParts_N = N_;
	waveParts = new Sinusoid[waveParts_N];
	for (i = 0; i < waveParts_N; i++){
		waveParts[i].setParameters(linear_array_[i], r_array_[i], theta_array_[i], omega_array_[i]);
	}
}

int CompositeSinusoid::getComponentsN(){
	return waveParts_N;
}

FLOAT_FMT CompositeSinusoid::value(FLOAT_FMT t, int deriv, int degree){
	int i;
	FLOAT_FMT ret;
	if (degree < 0) degree = waveParts_N;
	ret = 0;
	degree++;
	for (i = 0; i < degree; i++){
		ret += waveParts[i].value(t, deriv);
	}

	return ret;
}

int CompositeSinusoid::findRootsInterval(FLOAT_FMT t_end, FLOAT_FMT** roots){
	#define MAXIMA 0
	#define MINIMA 1

	int numRoots;
	std::list<FLOAT_FMT>* extrema;
	std::list<FLOAT_FMT>::iterator* extrema_i;
	int i;
	int q;
	int rootIndex;
	int peakSign;
	int emptyPeaks;
	FLOAT_FMT odDist;
	FLOAT_FMT firstEx;
	int       firstExRootIndex;
	int       lastExRootIndex;
	int	      prevLastExRootIndex;
	std::list<FLOAT_FMT>::iterator firstEx_i;
	int                            firstEx_i_sign;
	FLOAT_FMT lastEx;
	std::list<FLOAT_FMT>::iterator lastEx_i;
	int                            lastEx_i_sign;
	int memberCount;
	FLOAT_FMT prevLastEx;
	std::list<FLOAT_FMT>::iterator prevLastEx_i;
	int                            prevLastEx_i_sign;
	int prevMemberCount;
	FLOAT_FMT curPeak;
	FLOAT_FMT prevCurPeak;
	FLOAT_FMT waveTheta;
	FLOAT_FMT ptA;
	FLOAT_FMT ptB;
	FLOAT_FMT delta;
	FLOAT_FMT derivRootLoc;
	FLOAT_FMT last_derivRootLoc;
	int       exIndexA;
	int       exIndexB;
	bool recalcExtrema;
	Sinusoid curWave;
	
	extrema = new std::list<FLOAT_FMT>[2];
	extrema_i = new std::list<FLOAT_FMT>::iterator[2];

	numRoots = 0;

	for (i = 0; i < waveParts_N; i++){
		std::cout << "RECALCULATING EXTREMA" << std::endl;

		recalcExtrema = false;
		curWave = waveParts[i];
		waveTheta = M_PI / curWave.getOmega();
		if (!curWave.isConst()){
			rootIndex = 1 - INCLUDE_EDGE_NODES;
			if ((extrema[MINIMA].size() + extrema[MAXIMA].size()) == 0){
				q = 0;
				while (q < INCLUDE_EDGE_NODES){
					curPeak = curWave.derivRoot(rootIndex);
					rootIndex++;
					if (curPeak >= t_end) q++;
					peakSign = signbit(curWave.value(curPeak));

					extrema[peakSign].push_back(curPeak);
				}
			}
			else{
				// we observe a non constant wave part...
				rootIndex = 1 - INCLUDE_EDGE_NODES;
				emptyPeaks = 0;
				q = 0;
				lastEx = -1;
				firstEx = -1;
				memberCount = -1;
				lastEx_i_sign = -1;
				lastExRootIndex = -1;
				extrema_i[MINIMA] = extrema[MINIMA].begin();
				extrema_i[MAXIMA] = extrema[MAXIMA].begin();
				//for each peak

				while (q < INCLUDE_EDGE_NODES){
					prevCurPeak = curPeak; 
					curPeak = curWave.derivRoot(rootIndex);
					rootIndex++;

					if (curPeak >= t_end) q++;
					peakSign = signbit(curWave.value(curPeak));

					prevLastEx_i_sign = lastEx_i_sign;
					prevLastEx_i = lastEx_i;
					prevLastEx = lastEx;
					prevLastExRootIndex = lastExRootIndex;
					prevMemberCount = memberCount;

					//go through extrema that match our sign while we have ones that match up with us
					firstEx = -1;
					lastEx = -1;
					memberCount = 0;

					while (extrema_i[peakSign] != extrema[peakSign].end()){
						FLOAT_FMT test = *(extrema_i[peakSign]);
						odDist = *(extrema_i[peakSign]) - curPeak;

						if (abs(odDist) < waveTheta){

							//extrema_i is a peak on the running curve that matches the current peak we examine on the new sin
							if (firstEx == -1){
								firstEx = *(extrema_i[peakSign]);
								firstEx_i_sign = peakSign;
								firstEx_i = extrema_i[peakSign];
								firstExRootIndex = rootIndex - 1;
							}

							lastEx = *(extrema_i[peakSign]);
							lastEx_i_sign = peakSign;
							lastEx_i = extrema_i[peakSign];
							lastExRootIndex = rootIndex - 1;

							memberCount++;


							extrema_i[peakSign]++;
						}
						else if (odDist < 0){
							// skip peaks that are too far back
							extrema_i[peakSign]++;

						}
						else{
							break;
						}
					}

					if (memberCount > 0){
						emptyPeaks = 0;
					}
					else if (memberCount == 0){
						emptyPeaks++;
					}

					if (memberCount == 1){
						*lastEx_i = curPeak;
					}
					if (memberCount > 1){
						if (prevMemberCount > 1){
							//TODO: try merging prevLastEx and firstEx, maybe check to see first if they're next to each other?
							//this is seeming more and more like a totally bogus test... lol lets try it :D
							ptA = value(prevLastEx, 2, i);
							ptB = value(firstEx, 2, i);
							if (((firstExRootIndex - prevLastExRootIndex) != 1) || ((((signbit(ptA) ^ signbit(ptB)) & 1) == 1))){
								//
							}
							else{
								// gotta numb them gums!
								extrema[prevLastEx_i_sign].erase(prevLastEx_i);
								extrema[firstEx_i_sign].erase(firstEx_i);
							}

						}
					}
					else if (emptyPeaks == 2){
						emptyPeaks = 0;
						//TODO: try creating new peaks at curPeak and prevCurPeak
						
						ptA = value(prevCurPeak, 0, i - 1);
						ptB = value(curPeak, 0, i - 1);
						delta = ptB - ptA;

						ptA = value(prevCurPeak, 0, i);
						ptB = value(curPeak, 0, i);
						if (((signbit(delta) ^ signbit(ptB - ptA)) & 1) == 1){
							// insert two new peaks, one at derivRoot(curPeak) after the last extrema_i
							// and one at derivRoot(prevCurPeak) after the last extrema_i

							// this type of thing would have to change in a general implementation, like maybe some function to retrieve the 
							// type of extrema at a current peak
							exIndexA = signbit(curWave.value(prevCurPeak));
							exIndexB = signbit(curWave.value(curPeak));

							//if ((extrema_i[exIndexA] != extrema[exIndexA].end()) && 
							//	(extrema_i[exIndexB] != extrema[exIndexB].end())){
								
							extrema[exIndexA].insert(extrema_i[exIndexA], prevCurPeak);
							extrema[exIndexB].insert(extrema_i[exIndexB], curPeak);

							//}

						

						}
						else{
							//
						}
					}

				}
				recalcExtrema = true;
			}
		}

		std::list<FLOAT_FMT> extremaA = extrema[MINIMA];
		std::list<FLOAT_FMT> extremaB = extrema[MAXIMA];
		//apply constant parts later

		// fix all peaks:
		// go through the x's in extrema's, and peak find on value(x, through i, 1). set value to new x

		// may need some logic to make sure the found root is the same min/max type as list they're in
		if (recalcExtrema){
			for (extrema_i[MINIMA] = extrema[MINIMA].begin(); extrema_i[MINIMA] != extrema[MINIMA].end(); extrema_i[MINIMA]++){
				derivRootLoc = *(extrema_i[MINIMA]);
				do {
					last_derivRootLoc = derivRootLoc;
					derivRootLoc -= value(derivRootLoc, 1, i) / value(derivRootLoc, 2, i);

				} while (!FLOAT_PREC_EQL(derivRootLoc, last_derivRootLoc));
				*(extrema_i[MINIMA]) = derivRootLoc;
			}

			for (extrema_i[MAXIMA] = extrema[MAXIMA].begin(); extrema_i[MAXIMA] != extrema[MAXIMA].end(); extrema_i[MAXIMA]++){
				derivRootLoc = *(extrema_i[MAXIMA]);
				do {

					last_derivRootLoc = derivRootLoc;
					derivRootLoc -= value(derivRootLoc, 1, i) / value(derivRootLoc, 2, i);
				} while (!FLOAT_PREC_EQL(derivRootLoc, last_derivRootLoc));
				*(extrema_i[MAXIMA]) = derivRootLoc;
			}
		}

		extremaA = extrema[MINIMA];
		extremaB = extrema[MAXIMA];

		for (extrema_i[MINIMA] = extrema[MINIMA].begin(); extrema_i[MINIMA] != extrema[MINIMA].end(); extrema_i[MINIMA]++){
			std::cout << "MIN " << *(extrema_i[MINIMA]) << std::endl;
		}

		for (extrema_i[MAXIMA] = extrema[MAXIMA].begin(); extrema_i[MAXIMA] != extrema[MAXIMA].end(); extrema_i[MAXIMA]++){
			std::cout << "MAX " << *(extrema_i[MAXIMA]) << std::endl;
		}
		

	}
	
	delete[] extrema;
	delete[] extrema_i;

	return numRoots;
}

void CompositeSinusoid::init(){
	waveParts = nullptr;
	waveParts_N = 0;
}
