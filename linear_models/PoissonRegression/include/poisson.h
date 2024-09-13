#ifndef POISSON_H
#define POISSON_H

#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"


typedef struct {

	Matrix *weights;
	double bias;



} PoissonRegression;


PoissonRegression* CreatePoissonRegression(int features);
void TrainPoissonRegression(PoissonRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs);
Matrix* PredictPoissonRegression(PoissonRegression *model, Matrix *X);

//acts as a destructor
void FreePoissonRegression(PoissonRegression *model);



#endif //POISSON_H
