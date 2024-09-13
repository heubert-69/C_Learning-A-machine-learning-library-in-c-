#ifndef PROBIT_H
#define PROBIT_H


#include<stdio.h>
#include<stdlib.h>
#include<cmath.h>
#include "../include/matrix.h"


typedef struct {

	Matrix *weights
	double bias;

} ProbitRegression;

//math to help put all data points in a CDF(Cumulative Distribution Function)
double Normal_CDF(double x);


ProbitRegression* CreateProBitRegression(int features);
void TrainProBitRegression(ProbitRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs);
Matrix* PredictProBitRegression(ProbitRegression *model, Matrix *X);

//acts as a destructor
void FreeProBitRegression(ProbitRegression *model);


#endif //PROBIT_H
