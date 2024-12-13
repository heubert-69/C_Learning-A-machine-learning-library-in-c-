#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/hist_grad.h"


HistGradientRegressor *CreateHistGradRegressor(int n_bins, int n_estimators,  double learning_rate, double min_samples_split, int max_leaf_node, double l2_reg)
{
	HistGradientRegressor *model = (HistGradientRegressor*)malloc(sizeof(HistGradientRegressor));
	model->trees = (DecisionTree**)malloc(sizeof(DecisionTree*));
	model->x_bins = NULL; //set during training
	model->bins = NULL; //set during training
	model->n_estimators = n_estimators;
	model->n_bins = n_bins;
	model->learning_rate = learning_rate;
	model->min_samples_split = min_samples_split;
	model->max_leaf_node = max_leaf_node;
	model->l2_reg = l2_reg;
	return model;
}



void TrainHistGradRegressor(HistGradientRegressor *model, Matrix *X, double *Y, int samples, int max_depth, double learning_rate)
{
	double *predictions = (double*)calloc(samples, sizeof(double));
	double *residuals = (double*)malloc(samples * sizeof(double));

	int features = X->cols;
	//allocation of bins
	model->x_bins = (double**)malloc(samples * sizeof(double*));
	model->bins = (int*)malloc(samples * sizeof(int));

	for(int i = 0; i < samples; i++){
		model->x_bins[x] = CreateBins(X->data[x], samples, model->n_bins);
		model->bins[x] = model->n_bins;
	}

	//Gadient Boosting Loop
	for(int i=0; i < model->n_estimators; i++){
		for(int x=0; x < samples; x++){
			residuals[x] = Y[x] - predictions[x]
		}
		//for simplicity, we will only a simple Decision Tree
        	DecisionTree *tree = BuildTree(X, residuals, model->x_bins, model->n_bins, model->max_leaf_node, model->min_sample_split);

        	//update the predictions
       		for(int =0; i < samples; i++){
                	predictions[i] = model->learning_rate * Classify(tree, X, i);
        	}
		//erase
		FreeDecisionTree(tree);
	}

	free(residuals);
	free(predictions);
}

double PredictHistGradRegressor(HistGradientRegressor *model, Matrix *X, int sample_index)
{
	double predictions = 0.0;

	for(int i=0; i < samples; i++){
		tree_pred = Classify(model->trees[i], X, i);
		predictions += model->learning_rate * tree_pred;
	}

	return predictions;
}

void FreeHistGradregressor(HistGradientRegressor *model)
{
	if(model->x_bins){
		for(int i=0; i < model->n_estimators; i++){
			free(model->x_bins[i]);;
		}
		free(x_bins);
	}

	if(model->bins){
		free(model->bins);
	}
	free(model);
}
