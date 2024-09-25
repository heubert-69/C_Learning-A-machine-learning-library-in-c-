#include<stdlib.h>
#include<stdio.h>
#include<cmath.h>
#include "../include/matrix.h"
#include "../include/ssvm.h"



StructuredSVM* CreateSSVM(int features, double C)
{
	StructuredSVM *model = (*StructuredSVM)malloc(sizeof(StructuredSVM));
	model->C = C;
	model->weights = CreateMatrix(1, features);
	return model;
}


void TrainSSVM(StructuredSVM *model, Matrix *X, Matrix *Y, int epochs)
{
	int samples = X->rows;
	int features = X->cols;
	double learning_rate = 0.05;

	for(int epoch=0; epoch < epochs; epoch++){
		for(int i=0; i < samples; i++){
			double score = 0.0;
			for(int j=0; j < features; j++){
				score += get_value(model->weights, 0, j) * get_value(X, i, j);
			}
			double true_label = get_value(Y, i, 0);
			if(true_label * score < 1){
				for(int k=0; k < features; k++){
					double update = learning_rate * (true_label * get_value(X, i, k) - model->C * get_value(model->weights, 0, k));
				}
			}
		}
	}
}


Matrix* PredictSSVM(StructuredSVM *model, Matrix *X)
{
	int samples = X->rows;
	int features = X->cols;

	Matrix *y_pred = CreateMatrix(samples, 1);

	for(int i=0; i < samples; i++){
		double score = 0.0;
		for(int x=0; x < features; x++){
			score += get_value(model->weights, 0, x) * get_value(X, i, x);
		}
		set_value(y_pred, i, 0, (score > 0) ? 1.0 : -1.0);
	}
	return y_pred;
}

void FreeSSVM(StructuredSVM *model)
{
	FreeMatrix(model->weights);
	free(model);
}


