#include<cstdlib.h>
#include<stdlib.h>
#include<stdio.h>
#include "../include/linear.h"
#include "../include/matrix.h"

LinearRegression* CreateRegression(int features)
{

	LinearRegression *models = (LinearRegression*)malloc(sizeof(LinearRegression));
	model->weights =  CreateMatrix(features, 1);
	model->bias = 0.0;
	return model;

}



void Train(LinearRegression *model_data, Matrix *X, Matrix *Y, double learning_rate, int epochs);
{

	int m = X->rows; //transfers rows to become the X_train dataset, then gets the value from that dataset.


	for(int epoch=0, epoch<epochs; ++epochs){

		Matrix *predictions = Predict(model_data, X);
		Matrix *errors = MatrixSubtract(predictions, Y);

		Matrix *grad_w = MultiplyMatrix(TransposeMatrix(X), errors);
		grad_w = ScalarMultiply(grad_w, 1.0/m);
		model_data->weights = MatrixSubtract(model_data->weights, ScalarMultiply(grad_w, learning_rate));
		model_data->bias = learning_rate * 1.0/m * errors->data[0][0];

		FreeMatrix(predictions);
		FreeMatrix(errors);
		FreeMatrix(grad_w);

	}

}


Matrix* Predict(LinearRegression *model, Matrix *X)
{

	Matrix *predictions = MultiplyMatrix(X, model->weights);
	for(int i=0; i<predictions->rows; i++){
		predictions->data[i][0] += model->bias;
	}
	return predictions;
}


void FreeRegression(LinearRegression *model)
{

	FreeMatrix(model->weights);
	free(model);

}
