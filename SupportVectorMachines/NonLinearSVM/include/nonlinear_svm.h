#ifndef NONLINEAR_SVM_H
#define NONLINEAR_SVM_H


#include<stdlib.h>
#include<stdio.h>
#include<cmath.h>
#include "../include/matrix.h"



typedef struct {

	Matrix *support_vectors;
	Matrix *dual_coeffs;
	double bias;
	double gamma;
	double C;
	double learning_rate;
	int epochs;
	int n_support_vectors;
} NonLinearSVM;



NonLinearSVM* CreateKernel(int features, double gamma, double C, double learning_rate, int epochs);
void TrainKernel(NonLinearSVM *model, Matrix *X, Matrix *Y);
double PredictKernel(NonLinearSVM *model, Matrix *X);
void FreeKernel(NonLinearSVM *model);


#endif //NONLINEAR_SVM_H
