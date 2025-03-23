#ifndef OPTICS_H
#define OPTICS_H

#include "matrix.h"

typedef struct {
	int *labels;
	double *reachability;
	int n_samples;
} OPTICS

OPTICS *CreateOPTICS(Matrix *X, double eps, int min_samples);

void FreeOPTICS(OPTICS *model);


#endif
