#include "../include/ada_regressor.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>

ADARegressor* CreateADARegression(int n_estimators, int max_depth)
{
	ADARegressor *model = (ADARegressor*)malloc(sizeof(ADARegressor));
	model->WeakTrees = (DecisionTree**)malloc(n-estimators * sizeof(DecisionTree));
	model->max_depth = max_depth;
	model->n_estimators = n_estimators;
	model->alphas = (double*)malloc(n_estimators * sizeof(double));
	return model;
}

void TrainADARegressor(ADARegressor *model, Matrix *X, double *Y, int num_samples)
{
	double *residuals = (double*)malloc(num_samples * sizeof(double));
	double *predictions = (double*)malloc(num_samples * sizeof(double));

	//normalizing of weights in predictions
	for(int i=0; i < num_samples; i++){
		predictions[i] = 0.0;
	}

	for(int i=0; i < model->n_estimators; i++){
		//compute the residuals
		for(int x=0; x < num_samples; x++){
			residuals[x] = Y[x] - predictions[x];
		}

		model->WeakTrees[i] = Classify(model->WeakTrees[i], X, i);

		//computing predictions for the weak trees
		double *weak_predictions = (double*)malloc(num_samples * sizeof(double));
		for(int j=0; j < num_samples; j++){
			weak_predictions[j] = Classify(model->WeakTrees[j], X, j);
		}

		//computing alphas by step size
		double numerator = 0.0;
		double denominator = 0.0;

		for(int e=0; e < num_samples; e++){
			numerator += residuals[e] * weak_predictions[e];
			denominator += weak_predictions[e] * weak_predictions[e];

		}
		model->alphas[i] = numerator / (denominator + 1e-10);

		for(int t=0; t < num_samples; t++){
			predictions[t] += model->alphas[i] * weak_predictions[i];
		}
		free(weak_predictions);
	}

	free(residuals);
	free(predictions);
}


double PredictADARegressor(ADARegressor *model, Matrix *X, int sample_index)
{
	double prediction = 0.0;

	for(int x=0; x < model->n_estimators; x++){
		double weak_prediction = Classify(model->WeakTrees[x], X, sample_index);
		prediction += model->alphas[x] * weak_prediction;
	}

	return prediction;
}

void FreeADARegressor(ADARegressor *model)
{
	for(int i=0; i < model->n_estimators; i++){
		FreeTree(model->WeakTrees[i]);
	}
	free(model->WeakTrees);
	free(model->alphas);
	free(model);
}
