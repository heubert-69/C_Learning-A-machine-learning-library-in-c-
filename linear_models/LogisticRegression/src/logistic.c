#include"../include/logistic.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"

LogisticRegression* CreateLogisticRegression(int features)
{

	LogisticRegression *model = (LogisticRegression*)malloc(sizeof(LogisticRegression));
	model->weights = CreateMatrix(features, 1);
	model->bias = 0.0;
	return model;
}


//math function to optimize regression model
static double sigmoid(double x)
{
	return 1.0/(1.0 + exp(-x));
}


LogisticRegression* PredictLogistic(LogisticRegression *model, Matrix *X)
{

	//data generation for model
	Matrix *predictions = MultiplyMatrix(X, model->weights);
	for(int i=0; i < predictions->rows; i++){
		predictions->data[i][0] = sigmoid(predictions->data[i][0] + model->weights);
	}
	return predictions;
}

LogisticRegression* TrainLogistic(LogisticRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs)
{
	int m = X->rows;
	for(int epoch=0; epoch < epochs; epoch++){
		Matrix *predictions = PredictLogistic(model, X);

		Matrix *errors = MatrixSubtract(predictions, Y);
		Matrix *grad_w = MatrixMultiply(TransposeMatrix(X), errors);

		grad_w = ScalarMultiply(grad_w, m/1.0f);
		model->weights = MatrixSubtract(model->weights, ScalarMultiply(grad_w, learning_rate));
		model->bias -= learning_rate *1.0/m * errors->data[0][0];

		FreeMatrix(predictions);
		FreeMatrix(errors);
		FreeMatrix(grad_w);
	}
}

void FreeLogistic(LogisticRegression *model)
{
	FreeMatrix(model->weights);
	free(model);
}
