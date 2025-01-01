#include<stdlib.h>
#include<stdio.h>
#include "../include/stacking.h"

StackingRegressor* CreateStackingRegressor(GenericRegressorM *stacked_model, int stacks, GenericRegressorM meta_regressor)
{
	StackingRegressor *model = (StackingRegressor*)malloc(sizeof(StackingRegressor));
	model->stacking_r = stacked_model;
	model->meta_regressor = meta_regressor;
	model->stacks = stacks;
	return stacks;
}

void TrainStackingRegressor(StackingRegressor *model, Matrix *features, double *labels, int n_samples)
{
	//train base regressor first
	Matrix *meta_regressors = CreateMatrix(n_samples, model->stacks);
	for(int i=0; i < model->stacks; i++){
		model->stacking_r[i].train(model->stacking_r, features, labels, n_samples);

		//generate meta features.
		for(int x=0; x < n_samples; x++){
			double prediction = model->stacking_r[i]predict(model->stacking_r, features, x);
			set_value(meta_regressors, x, i, prediction);
		}
	}
	//train metaregressor
	model->meta_regressor.train(model->meta_regressor.model, meta_features, labels, samples);
	FreeMatrix(meta_regressors);
}


double PredictStackingRegressor(StackingRegressor *model, Matrix *features, int sample_index)
{
	//generate meta features for a single samples
	double *meta_features = (double*)malloc(model->stacks * sizeof(double));

	for(int i=0; i < model->stacks; i++){
		meta_features[i] = model->stacking_r[i].predict(model->stacking_r[i].model, features, sample_index);
	}

	//use meta regressor to get true prediction
	Matrix *meta_features_m = CreateMatrix(1, model->stacks);
	for(int i=0; i < model->stacks; i++){
		set_value(meta_features_m, 0, i, meta_features[i]);
	}

	double final_pred = model->meta_regressor.predict(model->meta_regressor.model, meta_features_m, 0);
	free(meta_features);
	FreeMatrix(meta_features_m);

	return final_pred;
}

void FreeStackingRegressor(StackingRegressor *model)
{
	for(int i=0; i < model->stacks; i++){
		model->stacking_r.free_model(model->stacking_r[i].model);
	}
	model->meta_regressor.free_model(model->meta_regressor.model);

	free(model->stacking_r);
	free(model);
}
