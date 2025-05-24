#ifndef BAYES_G_H
#define BAYES_G_H


#include "matrix.h"

typedef struct {
	int n_components;
	int n_features;
	double *weights;
	Matrix **means;
	Matrix **variances;
	double alpha; //for measuring the dirichlet prior for weights
	double beta; //for measuring the means
} BayesianGaussianMixture;

BayesianGaussianMixture* CreateBGM(int n_components, int n_features, double alpha, double beta);
void TrainBGM(BayesianGaussianMixture *model, Matrix *X, int max_iter);
int PredictBGM(BayesianGaussianMixture *model, double *X);
void FreeBGM(BayesianGaussianMixture *model);

#endif //BAYES_G_H
