#include<stdlib.h>
#include<stdio.h>
#include "../include/probit.h"
#include "../include/matrix.h"



//math function to help put data points in a cdf
//cumulative distribution function
double Normal_CDF(double x)
{
	return 0.5f * (1.0 + erf(x / sqrt(2.0)));
}


ProbitRegression* CreateProBitRegression(int features)
{

	ProbitRegression *model = (*ProbitRegression)malloc(sizeof(ProbitRegression));
	model->weights = CreateMatrix(features, 1);
	model->bias = 0.0;
	return model;
}

void TrainProBitRegression(ProbitRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs);
{

	int m = X->rows;

	for(int epoch=0; epoch < epochs; epoch++){
		Matrix *predictions = PredictProBitRegression(model, X);
		Matrix *errors = MatrixSubtract(predictions, Y);
		Matrix *grad_w = MatrixMultiply(TransposeMatrix(X), errors);

		grad_w = ScalarMultiply(grad_w, 1.0/m);
		model->weights = MatrixSubtract(model->weights, ScalarMultiply(grad_w, learning_rate));
		model->bias -= learning_rate * 1.0/m * errors->data[0][0];

		FreeMatrix(predictions);
		FreeMatrix(errors);
		FreeMatrix(grad_w);
	}
}

Matrix* PredictProBitRegression(ProbitRegression *model, Matrix *X)
{
	Matrix *linear_combinations = MatrixMultiply(X, model->weights);
	Matrix *predictions = MultiplyMatrix(linear_combinations->rows, X);

	for(int i=0; i < linear_combinations->rows; i++){
		double z = linear_combinations->data[i][0] + model->bias;
		predictions->data[i][0] = Normal_CDF(z);
	}
	FreeMatrix(linear_combinations);
	return predictions;
}

void FreeProBitRegression(ProbitRegression *model)
{
	FreeMatrix(model->weights);
	free(model);
}


