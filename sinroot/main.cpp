#include <iostream>
#include "compositesinusoid.h"
#include "float_format.h"

int main(){
	CompositeSinusoid func;
	int numRoots;
	FLOAT_FMT* roots;

	bool l[] = { false, false, false, false, false, true, false, false };
	FLOAT_FMT r[] = { 18.92922380007803, 11.98041898198426, 21.00809772498906, 6.755743827670813,
		              21.13537477329373, 1.885753609240055, 31.04674531146884, 39.75722121074796 };
	FLOAT_FMT p[] = { 5.174603016935638, 2.928211712643425, 2.939181668160558, 1.515709360720394,
					  0.8320124981922423, 2.748066201473134, 1.243998661311129, 2.183854899328588 };
	FLOAT_FMT w[] = { 0.676373988691892, 2.042265274011697, 0.2474932704251795, 2.496194280495064,
					  0.8677303846172033, 2.762173312672803, 2.360979850020236, 1.486675554520062 };

	func.setParameters(8, l, r, p, w);

	numRoots = func.findRootsInterval(30, &roots);

	std::cin.ignore();
	return 0;
}