#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"
#include "../include/lasso.h"



LassoRegression* CreateLassoRegression(int features, double alpha)
{
	LassoRegression *model = (*LassoRegression)malloc(sizeof(LassoRegression));
	model->weights = CreateMatrix(features, 1);
	model->bias = 0.0;
	model->alpha = alpha;
	return model;
}

Matrix* PredictLassoRegression(LassoRegression *model, Matrix *X)
{

	Matrix *predictions = MultiplyMatrix(X, model->weights);
	for(int i=0; i < predictions->rows; i++){
		predictions[i][0] += model->bias;
	}
	return predictions;
}


LassoRegression* TrainLassoRegression(LassoRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs)
{

	int m = X->rows;

	for(int epoch=0; epoch < epochs; epoch++){
		Matrix *predictions = PredictLassoRegression(model, X);
		Matrix *errors = SubtractMatrix(predictions, Y);


		Matrix *grad_w = MultiplyMatrix(TransposeMatrix(x), errors);
		grad_w = ScalarMultiply(grad_w, 1.0/m);

		//adding L2 regularization to model
		for(int i=0; i < model->weights->rows; i++){
			if(model->weights->data[i][0] > 0){
				model->weights->data[i][0] -= model->alpha;
			} else {model->weights->data[i][0] += model->alpha;}
		}

		model->weights = SubtractMatrix(model->weights, ScalarMultiply(grad_w, learning_rate));
		model->bias -= learning_rate * 1.0/m * errors->data[0][0];

		FreeMatrix(predictions);
		FreeMatrix(errors);
		FreeMatrix(grad_w);
	}
}


void FreeLassoRegression(LassoRegression *model)
{
	FreeMatrix(model->weights);
	free(model);
}
