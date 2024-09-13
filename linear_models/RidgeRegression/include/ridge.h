#ifndef RIDGE_H
#define RIDGE_H

#include<stdio.h>
#include "../include/matrix.h"
#include<stdlib.h>

typedef struct {
	Matrix *weights;
	double bias;
	double alpha; //Regularization for alpha

} RidgeRegression;

RidgeRegression* CreateRidgeRegression(int features, double alpha); //create a ridge regression
RidgeRegression* TrainRidgeRegression(RidgeRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs);
Matrix* PredictRidgeRegression(RidgeRegression *model, Matrix *X);

//acts as a destructor
void FreeRidgeRegression(RidgeRegression *model);

#endif //RIDGE_H
