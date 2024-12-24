#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../include/hist_classifier.h"

//helper function: sigmoid activation 
double Sigmoid(double x)
{
	return 1.0 / (1.0 + exp(-x));
}

HistGradientClassifier *CreateHistClassifier(int n_estimators, double learning_rate, int max_depth, int n_bins)
{
	HistGradientClassifier *model = (HistGradientClassifier*)malloc(sizeof(HistGradientClassifier));
	model->trees = (DecisionTree**)malloc(n_estimators * sizeof(DecisionTree*));
	model->n_estimators = n_estimators;
	model->learning_rate = learning_rate;
	model->max_depth = max_depth;
	model->n_bins = n_bins;
	model->gradients = NULL;
	model->residuals = NULL;
	return model;
}

//Calculating log-loss derivative
void CalculateLogLoss(Matrix *Y_true, Matrix *Y_pred, Matrix *gradients)
{
	for(int i=0; i < Y_true->rows; i++){
		double pred_prob = Sigmoid(get_value(Y_pred, i, 0));
		set_value(gradients, i, 0, pred_prob - get_value(Y_true, i, 0));
	}
}

void TrainHistClassifier(HistGradientClassifier *model, Matrix *X, Matrix *Y)
{
	model->predictions = CreateMatrix(X->rows, 1);
	model->residuals = CreateMatrix(X->rows, 1);

	for(int i=0; i < model->n_estimators; i++){
		//computing gradients
		CalculateLogLoss(model, Y);
		//training and building of tree is initilized in all in one function
		DecisionTree *tree = BuildTree(model->max_depth, X, Y, model->n_bins);


		Matrix *tree_pred = CreateMatrix(tree, X, model->gradients
		for(int x=0; x < X->rows; x++){
			double tree_update = model->learning_rate * get_value(tree_pred, x, 0);
			set_value(model->predictions, x, 0, get_value(model->predictions, x, 0) - tree_update);
		}

		model->trees[i] = tree; //storing the trained tree in our trees attribute

		FreeMatrix(tree_pred);
		double loss = 0.0; //monitoring loss to see errors in the trained model
		for(int j=0; j < X->rows; j++){
			double pred_prob = Sigmoid(get_value(model->pedictions, j, 0));
			double label = get_value(Y, j, 0);
			loss -= label * log(pred_prob + 1e-10) + (1 - label) * log(1 - pred_prob + 1e-10);
		}
		printf("Iteration %d: Log Loss = %f\n", i+1, loss/X->rows);
	}
}

Matrix* PredictHistClassifier(HistGradientClassifier *model, Matrix *X)
{
	Matrix *True_pred = CreateMatrix(X->rows, 1);

	for(int i=0; i < model->n_estimators; i++){
		Matrix *tree_pred = Classify(model->trees[i], X, i); //predicting each tree

		for(int x=0; x < X->rows; x++){
			double curr = get_value(True_pred, x, 0);
			set_value(True_pred, x, 0, curr - model->learnin_rate * get_value())
		}
		FreeMatrix(tree_pred);
	}



	//applying sigmoid to get probability predictions for each tree
	for(int i=0; i < X->rows; i++){
		double raw_prediction = get_value(True_pred, i, 0);
		set_value(True_pred, i, 0, Sigmoid(raw_prediction);)
	}
	return True_pred;
}

void FreeHistClassifier(HistGradientClassifier *model)
{
	for(int i=0; i < model->n_estimators; i++){
		FreeTree(model->trees[i]);
	}

	free(model->trees);
	FreeMatrix(model->gradients);
	FreeMatrix(model->predictions);
	free(model);
}
