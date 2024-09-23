#include<stdlib.h>
#include<stdio.h>
#include<cmath.h>
#include "../include/matrix.h"
#include "../include/proxima.h"

//helper function for L2 regularization
void proxima(Matrix *weights, double lambda)
{
	for(int i=0; i < weights->cols; i++){
		double w = get_value(weights, 0, j);
		if(w > lambda){
			set_value(weights, 0, i, w - lambda);
		} else if(w < -lambda){
			set_value(weights, 0, i, w - lambda);
		} else {
			set_value(weights, 0, i, w - lambda);
		}
	}
}



ProximalSVM* CreateProximalSVM(int features, double learning_rate, double regularization, int epochs)
{
	ProximalSVM *model = (*ProximalSVM)malloc(sizeof(ProximalSVM));
	model->weights = CreateMatrix(1, features); //only a single weight vector :C
	model->learning_rate = learning_rate;
	model->regularization = regularization;
	model->epochs = epochs;

	//generate random weights/inputs to the matrix
	for(int i=0; i < features; i++){
		set_value(model->weights, 0, i, ((double)rand()/RAND_MAX) - 0.5);
	}
	return model;
}



void TrainProximalSVM(ProximalSVM *model, Matrix *X, Matrix *Y)
{
	int samples = X->rows;
	int features = X->cols;

	for(int epoch=0; epoch < model->epochs; epoch++){
		for(int i=0; i < samples; i++){
			Matrix *x_ith = CreateMatrix(1, features);

			for(int x=0; x < features; x++){
				set_value(x_ith, 0, x, X->data[i][x]);
			}

			double true_label = Y->data[i][0];
			double score = 0.0;

			for(int k=0; k < features; k++){ //dot product rizz
				score += model->weights->data[0][k] * x_ith->data[0][k];
			}

			if(true_label * score < 1){
				for(int i=0; i < features; i++){
					double gradient = -true_label * x_ith->data[0][i];
					set_value(model->weights, 0, i, get_value(model->weights, 0, i) - model->learning_rate * gradient);
				}
			}

			//applying L2 proximal operator
			proxima(model->weights, model->learning_rate * model->regularization);
			FreeMatrix(x_ith);
		}
	}
}

int PredictProximalSVM(ProximalSVM *model, Matrix *X)
{
	double score = 0.0;

	for(int x=0; x < model->weights->cols; x++){
		score += model->weights->data[0][x] * X->data[0][x];
	}
	return (score >= 0) ? 1 : -1;
}


void FreeProximalSVM(ProximalSVM *model)
{
	FreeMatrix(model->weights);
	free(model);
}
