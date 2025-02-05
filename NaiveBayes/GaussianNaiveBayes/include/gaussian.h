#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include "matrix.h"

typedef struct {
	Matrix **means; //means of each feature class
	Matrix **variance; //variance of each feature class
	Matrix *class_priori; //prior probabilities of each class
	int n_classes;
	int n_features;
} GaussianNaiveBayes;

GaussianNaiveBayes *CreateGaussianNaiveBayes(int n_classes, int n_features);
void TrainGaussianNaiveBayes(GaussianNaiveBayes *model, Matrix *X, Matrix *Y);
int PredictNaiveBayes(GaussianNaiveBayes *model, double *samples);
Matrix *PredictGaussianNaiveBayes(GaussianNaiveBayes *model, Matrix *X);
void FreeGaussianNaiveBayes(GaussianNaiveBayes *model);

#endif //GAUSSIAN_H
