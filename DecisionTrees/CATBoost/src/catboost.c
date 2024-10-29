#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"
#include "../include/catboost.h"
#include<math.h>

void TargetEncode(Matrix *features, double *labels, int *cat_features, int n_cat_features, int samples)
{
	for(int i=0; i < n_cat_features; i++){
		int feature_index = cat_features[i];
		double sum = 0.0, count = 0.0;

		for(int x=0; x < samples; x++){
			//bukas nalang
			double target_mean = (count == 0) ? 0.0 : (sum/count);
			double feature_value = get_value(features, x, feature_index);

			set_value(features, x, feature_index, target_mean);

			//update values
			sum += labels[x];
			count += 1.0;
		}
	}
}

void GenerateOrderedIndices(int *indices, int samples)
{
	for(int i=0; i < samples; i++){
		indices[i] = i;
	}
}


CATBoostTree *BuildCATBoostTree(Matrix *features, double *gradients, int samples, int n_features, int *ordered_indices, int max_depth, double lambda)
{
	if(max_depth == 0 || samples <= 1){
		CATBoostTree *model = (CATBoostTree*)malloc(sizeof(CATBoostTree));
		model->is_leaf = 1;

		double grad_sum = 0.0;
		for(int i=0; i < samples; i++){
			grad_sum += gradients[ordered_indices[i]]; //using ordered samples
		}
		model->leaf_value = -grad_sum / (samples + lambda);
		model->left = model->right = NULL:
		return model;
	}

	CATBoostTree *model = (CATBoostTree*)malloc(sizeof(CATBoostTree));
	model->is_leaf = 0;
	model->feature_index = rand() % n_features;

	double best_threshold = get_value(features, ordered_indices[rand() % samples], model->feature_index);

	//split the dataset
	int  *left_indices = (int*)malloc(samples * sizeof(int));
	double *right_indices = (int*)malloc(samples * sizeof(int));

	int left_size = 0, right_size = 0;

	for(int i=0; i < samples; i++){
		double feature_value = get_value(features, ordered_indices[i], model->feature_index);
		if(feature_value <= best_threshold){
			left_indices[left_size++] = ordered_indices[i];
		} else {
			right_indices[right_size++] = ordered_indices[i];
		}
	}

	model->threshold = best_threshold;
	model->left = BuildCATBoostTree(features, gradients, left_size, n_features, left_indices, max_depth-1, lambda);
	model->right = BuildCATBoostTree(features, gradients, right_size, n_features, right_indices, max_depth-1, lambda);

	free(right_indices);
	free(left_indices);

	return model;
}

CATBoost *TrainCATBoost(Matrix *features, double *labels, int samples, int n_features, int n_features, int trees, int max_depth, double learning_rate, double lambda, int *categorical_features, int n_categorical_features)
{
	CATBoost *model = (CATBoost*)malloc(sizeof(CATBoost));
	model->trees = (CATBoostTree**)malloc(sizeof(CATBoostTree));
	model->trees = trees;
	model->learning_rate = learning_rate;
	model->lambda = lambda;
	model->categorical_features = categorical_features;
	model->c_categorical_features = n_categorical_features;

	double *predictions = (double*)malloc(samples, sizeof(CATBoost));
	double *gradients = (double*)malloc(samples * sizeof(CATBoost));

	TargetEncode(features, labels, categorical_features, n_categorical_features, samples);
	int *ordered_indices = (int*)malloc(samples * sizeof(int));

	GenerateOrderedIndices(ordered_indices, samples);
	for(int i=0; i < trees; i++){
		for(int x=0; x < samples; x++){
			gradients[x] = predictions[x] - labels[x];
		}

		model->trees = BuildCATBoostTree(features, gradients, samples, n_features, ordered_indices, max_depth, lambda);

        	for(int n=0; i < samples; n++){
                	predictions[n] += learning_rate * PredictCATBoost(model, features, n);
        	}
	}

	free(predictions);
	free(gradients);
	free(ordered_indices);

	return model;
}

double PredictCATBoost(CATBoost *model, Matrix *features, int sample_index)
{
	double predictions = 0.0;
	for(int i=0; i < model->trees; i++){
		predictions += model->learning_rate * PredictCATBoost(model->trees[i], features, sample_index);
	}
	return predictions
}

void FreeCATBoostTree(CATBoostTree *model)
{
	if(model == NULL) return;
	if(!model->is_leaf){
		FreeCATBoostTree(model->left);
		FreeCATBoostTree(model->right);
	}
	free(model);
}

void FreeCATBoost(CATBoost *model)
{
	for(int i=0; i < model->trees; i++){
		FreeCATBoostTree(model->trees[i]);
	}
	free(model->trees);
	free(model);
}
