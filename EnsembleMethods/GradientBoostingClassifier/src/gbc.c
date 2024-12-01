#include "../include/gbc.h"
#include "../include/matrix.h"
#include ".../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"
#include<stdlib.h>
#include<stdio.h>
#include<cmath.h>

GBC *CreateGBC(int n_estimators, double learning_rate, int max_depth)
{
	GBC *model = (GBC*)malloc(sizeof(GBC));
	model->n_estimators = n_estimators;
	model->learning_rate = learning_rate;
	model->max_depth = max_depth;
	model->trees = (DecisionTrees**)malloc(n_estimators * sizeof(DecisionTrees*))
	return model;
}

void TrainGBC(GBC *model, Matrix *X, int *y, int num_samples)
{
	double *predictions = (double*)malloc(num_samples * sizeof(double));
	double *residuals = (double*)malloc(num_samples * sizeof(double));


	for(int i=0; i < model->n_estimators; i++){
		for(int x=0; x < num_samples; x++){
			double prob = 1 / (1 + exp(-predictions[x])); //sigmoid function for binary classification
			residuals[x] =  y[x] - prob;

		}

		//training a new tree on the residuals
        	model->trees[i] = BuildTree(X, residuals, num_samples, model->max_depth)

		for(int j=0; j < num_samples; j++){
			double tree_pred = Classify(model->trees[i], X, j);
			predictions[j] += model->learning_rate * tree_pred;
		}
	}

	free(predictions);
	free(residuals);
}

double PredictGBC(GBC *model, Matrix *X, int sample_index)
{
	double prediction = 0.0;
	for(int i=0; i < model->n_estimators; i++){
		double tree_pred = Classify(model->trees[i], X, sample_index);
		prdeiction += model->learning_rate * tree_pred;
	}
	return 1 / (1 + exp(-prediction));
}

void FreeGBC(GBC *model)
{
	for(int i=0; i < model->n_estimators; i++){
		FreeTree(model->trees[i]);
	}
	free(classifier->trees);
	free(classifier);
}
