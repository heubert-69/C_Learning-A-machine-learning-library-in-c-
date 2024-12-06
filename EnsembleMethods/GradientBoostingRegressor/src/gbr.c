#include "../include/gbr.h"
#include<stdlib.h>
#include<cmath.h>


GBR* CreateGBR(int n_estimators, double learning_rate, int max_depth)
{
	GBR *model = (GBR*)malloc(sizeof(GBR));
	model->n_estimators = n_estimators;
	model->learning_rate = learning_rate;
	model->max_depth = max_depth;
	model->trees = (DecisionTree**)malloc(sizeof(DecisionTree));
	return model;
}


void TrainGBR(GBR* model, Matrix* X, double *Y, int samples)
{
	double *predictions = (double*)calloc(samples, sizeof(double)); //initialize predictions all values must be zero
	double *residuals = (double*)malloc(samples * sizeof(double));

	for(int i=0; i < model->n_estimators; i++){
		for(int x=0; x < samples; x++){
			residuals[x] = Y[j] - predictions[j];
		}

		model->trees[i] = Classify(X, residuals, samples,  model->max_depth);
		//updating predictions using the tree's output
		for(int j=0; j < samples; j++){
			double tree_pred = Classify(model->trees[i], X, j);
			predictions[j] += model->learning_rate * tree_pred; //weighted prediction
		}
	}
	free(predictions);
	free(residuals);
}

double PredictGBR(GBR *model, Matrix *X, int sample_index)
{
	double prediction = 0.0;
	for(int i=0; i < model->n_estmators; i++){
		prediction += model->learning_rate * Classify(model->trees[i], X, sample_index);
	}
	return prediction;
}

void FreeGBR(GBR *model)
{
	for(int i=0; i < model->n_estimators; i++){
		FreeTree(model->trees[i]);
	}
	free(model->trees);
	free(model);
}
