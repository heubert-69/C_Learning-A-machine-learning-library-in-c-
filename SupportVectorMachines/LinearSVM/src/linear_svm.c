#include<stdlib.h>
#include<stdio.h>
#include<cmath>
#include "../include/matrix.h"
#include "../include/linear_svm.h"




LinearSVM* CreateSVM(int features, double learning_rate, double lambda, double epochs)
{
	LinearSVM *model = (LinearSVM*)malloc(sizeof(LinearSVM));
	model->weights = CreateMatrix(features, 1);
	model->bias = 0.0;
	model->learning_rate = learning_rate;
	model->lambda = lambda;
	model->epochs = epochs;
	return model;
}



void TrainSVM(LinearSVM *model, Matrix *X, Matrix *Y)
{
	int samples = X->rows;
	int features = X->cols;

	for(int epoch=0; epoch < model->epochs; epoch++){
		for(int i=0; i < samples; i++){
			Matrix *x_ith = CreateMatrix(1, features);
			for(int j=0; j < features; j++){
				set_value(x_ith, 0, j, X->data[i][j]);
			}

			double y_ith = Y->data[i][0];
			double decision = 0; //calculates the w^t * x + b

			for(int j=0; j < features; j++){
				decision += model->weights->data[j][0] * x_ith->data[0][j];
			}
			decision += model->bias;


			//hinge loss condition
			if(y_ith * decision < 1){
				//update the weights of the model
				for(int x=0; x < features; x++){
					model->weights->data[x][0] = model->weights->data[x][0] - model->learning_rate *(model->lambda * model->weights->data[x][0] - y_ith * x_ith->data[0][j]);

				}
				model->bias += model->learning_rate * y_ith;
			} else{
				//applying l2 regularization
				for(int i=0; i < features; i++){
					model->weights->data[i][0] -= model->learning_rate * (model->lambda * model->weights->data[i][0]);
				}
			}
			//acts sort of a dstructor to matrices
			FreeMatrix(x_ith);
		}
	}
}


//predict using the trained SVM
double PredictSVM(LinearSVM *model, Matrix *X)
{
	double decision = 0;
	for(int i=0; i < X->cols; i++){
		decision += model->weights->data[i][0];
	}
	decision += model->bias;

	return decision >= 0 ? 1 : -1;
}


//destructor
void FreeSVM(LinearSVM *model)
{
	FreeMatrix(model->weights);
	free(model);
}
