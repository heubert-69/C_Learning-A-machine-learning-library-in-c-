#ifndef SVR_H
#define SVR_H

#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"

typedef struct {

	Matrix *support_vectors;
	Matrix *dual_coeffs;
	double bias;
	double gamma;
	double C;
	double epsilon;
	double learning_rate;
	int epochs;

} SupportVectorRegression;


SupportVectorRegression* CreateSVR(int features, double gamma, double C, double epsilon, double learning_rate, int epochs);
void TrainSVR(SupportVectorRegression *model, Matrix *X, Matrix *Y);
double PredictSVR(SupportVectorRegression *model, Matrix *X);
void FreeSVR(SupportVectorRegression *model);


#endif //SVR_H
