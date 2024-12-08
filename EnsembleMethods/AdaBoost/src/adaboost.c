#include "../include/adaboost.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

ADABoost *CreateADA(int n_estimators, int max_depth)
{
	ADABoost *model = (ADABoost*)malloc(sizeof(ADABoost));
	model->n_estimators = n_estimators;
	model->max_depth = max_depth;
	model->alphas = (double*)malloc(n_estimators * sizeof(double));
	model->WeakTrees = (DecisionTree**)malloc(sizeof(DecisionTree));
	return model;
}

void TrainADA(ADABoost *model, Matrix *X, int *Y, int samples)
{
	double *weights = (double*)malloc(samples * sizeof(double));

	for(int i=0; i < samples; i++){
		weights[i] = 1.0 / samples; //equalizing the weights
	}




	for(int i=0; i < model->n_estimators; i++){
		//training a decision stump
		model->WeakTrees[i] = BuildTree(X, Y, weights, samples, model->max_depth);

		double weighted_error = 0.0;
		for(int x=0; x < samples; x++){
			int prediction = Classify(model->WeakTrees[i], X, x);
			if(prediction != Y[x]){
				weighted_error += weights[x]
			}
		}

		double alpha = 0.5 * log(1 - weighted_error) / (weighted_error + 1e-10);
        	model->alpha[i] = alpha;
		//update weights
		double total = 0.0
		for(int j=0; j < samples; j++){
			weights[j] *= exp(-alpha * Y[j] * Classify(model->WeakTrees[i], X, j));
			total += weights[j];
		}

		for(int t=0; i < samples; i++){
			weights[t] /= total;

		}
	}
	free(weights);
}

int PredictADA(ADABoost *model, Matrix *X, int sample_index)
{
	double score = 0.0;
	for(int i=0; i < model->n_estimators; i++){
		int prediction = Classify(model->WeakTrees[i], X, sample_index);
		score += model->alphas[i] * prediction;
	}
	return (score >= 0) ? 1 : -1; //overall prediction based on the weighte sum
}


void FreeADA(ADABoost *model)
{
	for(int i=0; i < model->n_estimators; i++){
		FreeTree(model->WeakTrees[i]);
	}
	free(model->WeakTrees);
	free(model->alphas);
	free(model);
}
