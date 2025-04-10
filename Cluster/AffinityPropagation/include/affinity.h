#ifndef AFFINITY_H
#define AFFINITY_H

#include "matrix.h"

typedef struct {
	int *labels;
	Matrix *similarity;
	Matrix *responsibility;
	Matrix *availability;
	int max_iter;
	double damping;
} AffinityPropagation;

AffinityPropagation* CreateAP(int max_iter, double damping);
void FitAP(AffinityPropagation *ap, Matrix *X);
void FreeAP(AffinityPropagation *model);

#endif //AFFINITY_H
