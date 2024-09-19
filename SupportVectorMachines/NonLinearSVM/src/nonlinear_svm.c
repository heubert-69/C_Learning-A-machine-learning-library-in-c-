#include<stdio.h>
#include<stdlib.h>
#include "../include/matrix.h"
#include "../include/nonlinear_svm.h"
#include<cmath.h>




NonLinearSVM* CreateKernel(int features, double gamma, double C, double learning_rate, int epochs)
{

	NonLinearSVM *model = (*NonLinearSVM)malloc(sizeof(NonLinearSVM));
	model->support_vectors = NULL;
	model->dual_coeffs = NULL; //two of these will be set during training
	model->bias = 0;
	model->gamma = gamma;
	model->C = C;
	model->learning_rate = learning_rate
	model->epochs = epochs;
	model->n_support_vectors = 0;
	return model;
}

void TrainKernel(NonLinearSVM *model, Matrix *X, Matrix *Y)
{
	int samples = X->rows;
	int features = X->cols;

	//initializing matrices
	model->support_vectors = CreateMatrix(samples, features);
	model->dual_coeffs = CreateMatrix(samples, 1);

	//pasting the data as support vectors
	for(int i=0; i < samples; i++){
		for(int x=0; x < features; x++){
			model->support_vectors->data[i][x] = X->data[i][x];
		}
	}

	//initialize dual coefficients w/ Gradient Descent optimization
	for(int epochs=0; epoch < model->epochs; epoch++){
		for(int i=0; i < samples; i++){
			Matrix *x_ith = CreateMatrix(1, features);
			for(int x=0; x < features; x++){
				set_value(x_ith, 0, x, X->data[i][x]);
			}
			double y_ith = Y->data[i][0];
			double decision = 0.0;
			for(int j=0; j < features; j++){
				decision += model->weights->data[j][0] * x_ith->data[0][j];
			}
			decision += model->bias;

			if(y_ith * decision < 1){
				for(int i=0; i <  features; i++){
					model->weights->data[i][0] = model->weights->data[i][0] - model->learning_rate * (model->lambda * model->weights->data[i][0] - y_ith * x_ith->data[0][i]);
				}
				model->bias += model->learning_rate * y_ith;
			} else {
				//apply L2 regularization
				for(int j=0; j < features; j++){
					model->weights->data[j][0] -= model->learning_rate * (svm->lambda * model->weights->data[j][0]);
				}
			}
			//free da memory
			FreeMatrix(x_ith);
		}
	}
}


double PredictKernel(NonLinearSVM *model, Matrix *X)
{

	double decision=0.0;

	for(int x=0; x < X->cols; x++){
		decision += model->weights->data[x][0] * X->data[0][x]
	}

	decision += model->bias;

	return decision >= 0 ? 1 : -1;
}



void FreeKernel(NonLinearSVM *model)
{
	FreeMatrix(model->weights);
	free(model);
}
