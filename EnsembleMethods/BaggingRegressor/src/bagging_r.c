#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "../include/baggingR.h"


BaggingRegressor *CreateBaggingRegressor(int n_estimators, int max_samples, int max_depth)
{
	BaggingRegressor *model = (BaggingRegressor*)malloc(sizeof(BaggingRegressor));
	model->trees = (DecisionTree**)malloc(n_estimators * sizeof(DecisionTree*));
	model->n_estimators = n_estimators;
	model->max_samples = max_samples;
	model->max_depth = max_depth;
	return model;
}

void GenerateBootstrapSamples(Matrix *X, Matrix *Y, Matrix **X_B, Matrix **Y_B, int max_samples)
{
	int rows = X->rows;
	*X_B = CreateMatrix(max_samples, X->cols);
	*Y_B = CreateMatrix(max_samples, 1);

	for(int i = 0; i < max_samples; i++){
		int rand_ind = rand() % rows;
		for(int x=0; x < X->cols; x++){
			set_value(*X_B, i, x, get_value(X, rand_ind, 0));
		}
		set_value(*Y_B, i, 0, get_value(Y, rand_ind, 0));
	}
}






void TrainBGRegressor(BaggingRegressor *model, Matrix *X, Matrix *Y)
{
	srand((unsigned int)time(NULL));
	//create bootstrap samples for the matrices
	for(int i=0; i < model->n_estimators; i++){
		Matrix *X_B, *Y_B; //bootstrap samples for X and Y matrix
		GenerateBootstrapSamples(X, Y, &X_B, &Y_B, model->max_samples);

		DecisionTree *tree = BuildTree(X_B, Y_B->data, model->max_depth, X->cols);

		//storing the trained tree
		model->trees[i] = tree

		FreeMatrix(X_B);
		FreeMatrix(Y_B);

		printf("Trained Tree %d/%d\n", i+1, model->n_estimators);
	}
}

Matrix *PredictBGRegressor(BaggingRegressor *model, Matrix *X)
{
	Matrix *predictions = CreateMatrix(X->rows, 1);

	for(int i=0; i < model->n_estimators; i++){
		Matrix *tree_pred = Classify(model->trees[i], X, i);
		//accumulate preds
		for(int x=0; x < X->rows; x++){
			double curr = get_value(predictions, i, 0);
			set_value(predictions, x, 0, curr + get_value(tree_pred, x, 0));
		}
		FreeMatrix(tree_pred);
	}

	//average predictions
	for(int j=0; j < X->rows; j++){
		double avg = get_value(predictions, j, 0) / model->n_estimators;
		set_value(predictions, i, 0, avg);
	}

	return predictions;
}


void FreeBGRegressor(BaggingRegressor *model)
{
	for(int i=0; i < model->n_estimators; i++){
		FreeTree(model->trees[i]);
	}
	free(model->trees);
	free(model);
}
