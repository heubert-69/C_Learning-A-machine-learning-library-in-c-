#ifndef GAUSSIAN_M_H
#define GAUSSIAN_M_H

#include "matrix.h"

typedef struct {
	int n_components;
	int max_iter;
	double tol;
	double *weights;
	Matrix **means;
	Matrix **covariances;
	double **responsibilities;
} GaussianMixture;

GaussianMixture *CreateGM(int n_components, int max_iter, double tol);
void FitGM(GaussianMixture *model, Matrix *X);
int PredictGM(GaussianMixture *model, Matrix *X, int sample_index);
void FreeGM(GaussianMixture *model);

#endif //GAUSSIAN_M_H
