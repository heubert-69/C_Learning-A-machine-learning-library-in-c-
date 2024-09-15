#ifndef BAYES_H
#define BAYES_H


#include<stdio.h>
#include<stdlib.h>
#include "../include/matrix.h"


typedef struct {
	Matrix *weights;
	Matrix *covariance;
} BayesianLinearRegression;


BayesianLinearRegression* CreateBayesianModel(int features);
void TrainBayesianModel(BayesianLinearRegression *model, Matrix *X, Matrix *Y, double alpha, int beta);
Matrix* PredictBayesianModel(BayesianLinearRegression *model, Matrix *X);
void FreeBayesianModel(BayesianLinearRegression *model);

#endif //BAYES_H
