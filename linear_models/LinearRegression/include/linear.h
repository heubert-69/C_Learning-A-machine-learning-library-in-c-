#ifndef LINEAR_H
#define LINEAR_H


#include<stdlib>
#include<cmath>
#include<vector>
#include "../include/matrix.h"

struct LinearRegression
{

	Matrix* weights; //2d arrays for modeling
	double bias;

}

LinearRegression* CreateRegression(int features);
void Train(LinearRegression *model_data, Matrix *X, Matrix *Y, double learning_rate, int epochs);
Matrix* Predict(LinearRegression *model, Matrix *X);
void FreeRegression(LinearRegression *model);


#endif //LINEAR_H
