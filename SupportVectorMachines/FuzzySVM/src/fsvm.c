#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"
#include "../include/fsvm.h"


FuzzySVM* CreateFSVM(int features, double regularization, Matrix *fuzzy_memberships)
{
	FuzzySVM *model = (*FuzzySVM)malloc(sizeof(FuzzySVM));
	model->weights = CreateMatrix(1, features);
	model->bias = 0.0;
	model->regularization = regularization;
	model->fuzzy_memberships = fuzzy_memberships;
	return model;
}




void TrainFSVM(FuzzySVM *model, Matrix *X, Matrix *Y)
{
	int features = X->rows;
	int samples = X->cols;

	//initializing weights
	for(int i=0; i < features; i++){
		set_value(model->weights, 0, i, 0.0);
	}
	model->bias = 0.0;

	//implementing Gradient Descent rizz
	double learning_rate= 0.05f;
	for(int epoch=0; epoch < 10000; epoch++){
		for(int i=0; i < samples; i++){
			double score = model->bias;
			for(int x=0; x < features; x++){
				score += get_value(model->weights, 0, x) * get_value(X, i, x);
			}

			double label = (Y->data[i][0] > 0) ? 1.0 : -1.0;
			double membership = get_value(model->fuzzy_memberships, i, 0);

			if(label * score < 1){
				for(int k=0; k < features; k++){
					double update = learning_rate * (label * get_value(X, i, k) * membership);
					set_value(model->weights, 0, k, get_value(model->weights, 0, k) + update);
				}
				model->bias += learning_rate * label * membership;
			}
		}
	}
}





double PredictFSVM(FuzzySVM *model, Matrix *X)
{
	double score = model->bias;
	for(int i=0; i < X->cols; i++){
		score += model->weights->data[0][i] * X->data[0][i];
	}
	return score; //can still apply threshold based on your preference
}




void FreeFSVM(FuzzySVM *model)
{
	FreeMatrix(model->weights);
	free(model);
}
