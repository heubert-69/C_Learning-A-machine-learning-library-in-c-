#include<cstdlib.h>
#include<cstdio.h>
#include<cmath.h>
#include "../include/matrix.h"
#include "../include/ridge.h"



RidgeRegression* CreateRidgeRegression(int features, double alpha)
{
	RidgeRegression *model = (*RidgeRegression)malloc(sizeof(RidgeRegression));
	model->weights = CreateMatrix(features, 1);
	model->bias = 0.0;
	model->alpha = alpha;
	return model;
}

Matrix* PredictRidgeRegression(RidgeRegression *model, Matrix *X)
{
	Matrix *predictions = MultiplyMatrix(X, model->weights);
	for(int i=0; i < predictions->rows; i++){
		predictions->data[i][0] += model->bias;
	}
	return predictions;
}


RidgeRegression* TrainRidgeRegression(RidgeRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs)
{

	int m = X->rows; //initializing data points on m

	for(int epoch=0; epoch < epochs; epoch++){
		Matrix* predictions = PredictRidgeRegression(model, X);
		Matrix* errors = SubtractMatrix(predictions, Y);

		Matrix *grad_w = MultiplyMatrix(TransposeMatrix(X), errors);
		grad_w = ScalarMultiply(grad_w, 1.0/m);

		//Adding L2 regularization
		Matrix *reg_term = ScalarMultiply(model->weights, ScalarMultiply(grad_w, learning_rate));
		grad_w = MatrixAdd(grad_w, reg_term);

		model->weights = MatrixSubtract(model->weights, ScalarMultiply(grad_w, learning_rate));
		model->bias -= learning_rate * 1.0/m * errors->data[0][0];

		FreeMatrix(predictions);
		FreeMatrix(errors);
		FreeMatrix(grad_w);
		FreeMatrix(reg_term);
	}
}

void FreeRidgeRegression(RidgeRegression *model)
{
	FreeMatrix(model->weights);
	free(model);
}
