#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"
#include "../include/tsvm.h"

TransductiveSVM* CreateTSVM(int features, double C, double C_unlabeled)
{
	TransductiveSVM *model = (*TransductiveSVM)malloc(sizeof(TransductiveSVM));
	model->weights = CreateMatrix(1, features);
	model->bias = 0.0;
	model->C = C;
	model->C_unlabeled = C_unlabeled;
	return model;
}

void TrainSVM(TransductiveSVM *model, Matrix *X_labeled, Matrix *X_unlabeled, Matrix *Y_labeled)
{
	int labeled = X_labeled->rows;
	int unlabeled = X_unlabeled->rows;
	int features = X_labeled->cols;

	//initialize weights and bias
	for(int i=0; i < features; i++){
		set_value(model->weights, 0, i, 0.0);
	}

	model->bias = 0.0;

	//making unlabeled columns to 0
	Matrix *Y_unlabeled = CreateMatrix(unlabeled, 1);
	for(int x=0; x < unlabeled; x++){
		set_value(Y_unlabeled, x, 0, 0.0);
	}

	//Gradient Descent Time
	double learning_rate = 0.05f;
	double epochs = 1000;

	for(int epoch=0; epoch < epochs; epoch++){
		for(int i=0; i < labeled; i++){
			double score = model->bias;
			for(int k=0; k < features; k++){
				score += get_value(model->weights, 0, k) * get_value(X_labeled, i, k);
			}


			double label = (Y_labeled->data[i][0] > 0) ? 1.0 : -1;

			if(label * score < 1){
				for(int n=0; i< features; n++){
					double update = learning_rate * (label * get_value(X_labeled, i, n);
					set_value(model->weights, 0, n, get_value(model->weights, 0, n) + update);
				}
				model->bias += learning_rate * label;


			}
		}

		//assign pseudo-native labels to unlabeled data
		for(int k=0; k < unlabeled; k++){
			double score = model->bias;
			for(int i=0; features; i++){
				score += get_value(model->weights, 0, i) * get_value(X_unlabeled, k, i);
			}
			int pseudo_label = sign(score); //assign a pseudo label
			set_value(Y_unlabeled, k, 0, pseudo_label);

			if(pseudo_label * score < 1){
				for(int i=0; i < features; i++){
					double update = learning_rate * (pseudo_label * get_value(X_unlabeled, k, i));
					set_value(model->weights, 0, i, get_value(model->weights, 0, i) + update);
				}
				model->bias += learning_rate * pseudo_label;
			}
		}
	}
	FreeMatrix(Y_unlabeled);
}

double PredictTSVM(TransductiveSVM *model, Matrix *X)
{
	double score = model->bias;
	for(int i=0; i < X->cols; i++){
		score += get_value(model->weights, 0, i) * get_value(X, 0, i);
	}
	return score;
}


void FreeTSVM(TransductiveSVM *model)
{
	FreeMatrix(model->weights);
	free(model);
}
