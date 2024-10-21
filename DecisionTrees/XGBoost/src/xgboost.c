#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../include/matrix.h"
#include "../include/xgboost.h"

//calculation for gradient error loss aka mean squared error
void CalculateGradients(double *gradients, double *hessians, double *labels, double *predictions, int samples)
{
	for(int i=0; i < samples; i++){
		gradients[i] = predictions[i] - labels[i]; //calculating the accurate actual
		hessians[i] = 1.0;
	}
}

double GetRandomThreshold(Matrix *features, int feature_index, int samples)
{
        double min_value = DBL_MAX;
        double max_value = -DBL_MAX;

        //locate min and max value in the features
        for(int i=0; i < samples; i++){
                double value = get_value(features, i, feature_index);
                if(value < min_value) min_value = value;
                if(value > max_value) max_value = value;
        }

        double result = min_value + (rand() / (double)RAND_MAX) * (max_value - min_value);

        return result;
}

double CalculateSplitGain(double G_L, double H_L, double G_R, double H_R, double gamma)
{
	double gain_left = (G_L * G_L) / H_L;
	double gain_right = (G_R * G_R) / H_R;
	double gain_parent = ((G_L + G_R) * (G_L + G_R)) / (H_L + H_R);

	double gain = gain_left + gain_right - gain_parent - gamma;
	return gain;
}



double PredictXGBTree(XGBoost *model, Matrix *features, int sample_index)
{
	if(model->is_leaf){
		return model->leaf_value;
	}
	double value = get_value(features, sample_index, model->feature_index);
	if(value <= model->threshold){
		return PredictXGB(model->left, features, sample_index);
	} else {
		return PredictXGB(model->right, features, sample_index);
	}
}

XGBTree *BuildXGBTree(Matrix *features, double *gradients, double *hessians, int samples, int n_features, int max_depth, double gamma, double lambda)
{
	if(max_depth == 0){
		XGBTree *model = (XGBTree*)malloc(sizeof(XGBTree));
		model->is_leaf = 1;
		double sum_grad = 0.0, sum_hess = 0.0;
		for(int i=0; i < samples; i++){
			sum_grad += gradients[i];
			sum_hess += hessians[i];
		}
		model->leaf_value = -sum_grad/(sum_hess + lambda); //optimal leaf value for the tree node
		model->left = leaf->right = NULL;
		return model;
	}

	XGBTree *model = (XGBTree*)malloc(sizeof(XGBTree));
	model->is_leaf = 0;
	model->feature_index = rand() % n_features;
	model->threshold = GetRandomThreshold(features, model->feature_index, samples);

	double gain = CalculateSplitGain(gradients, hessians, samples, gamma);
	if(gain < gamma){
		XGBTree *model = (XGBTree*)malloc(sizeof(XGBTree));
		model->is_leaf = 1;
		double sum_grad = 0.0, sum_hess = 0.0;
                for(int i=0; i < samples; i++){
                        sum_grad += gradients[i];
                        sum_hess += hessians[i];
                }
		model->leaf_value = -sum_grad/(sum_hess + lambda);
		return model;
	}

	int *left_indices = (int*)malloc(samples * sizeof(int));
	int *right_indices = (int*)malloc(samples * sizeof(int));

	int left_size, right_size;
	split_data(features, gradients, samples, model->feature_index, model->threshold, left_indices, right_indices, &left_size, &right_size);


	//splitting it recursively
	model->left = BuildXGBTree(features, gradients, hessians, left_size, n_features, max_depth-1, gamma, lambda);
	model->right = BuildXGBTree(features, gradients, hessians, right_size, n_features, max_depth-1, gamma, lambda);

	free(right_indices);
	free(left_indices);

	return model;
}


XGBoost *TrainXGB(Matrix *features, double *labels, int samples, int n_features, int trees, int max_depth, double learning_rate, double gamma, double lambda)
{
	XGBoost *model = (XGBoost*)malloc(sizeof(XGBoost));
	XGBoost->trees = (XGBTree**)malloc(trees * sizeof(XGBoost));
	XGBoost->n_trees = trees;
	XGBoost->learning_rate = learning_rate;
	XGBoost->gamma = gamma;
	XGBoost->lambda = lambda;

	double *predictions = (double*)calloc(samples, sizeof(double));
	double *gradients = (double*)calloc(samples, sizeof(double));
	double *hessians = (double*)calloc(samples, sizeof(double));

	for(int i=0; i < trees; i++){
		CalculateGradients(gradients, hessians, labels, predictions, samples);

		model->trees[i] = BuildXGBTree(features, gradients, hessians, samples, n_features, max_depth, gamma, lambda);

		for(int x=0; x < samples; x++){
			predictions[x] += learning_rate * PredictXGB(model->trees[i], features, x);
		}
	}

	free(predictions);
	free(hessians);
	free(gradients);

	return model;
}


double PredictXGB(XGBoost *model, Matrix *features, int sample_index)
{
	double prediction = 0.0;
	for(int i=0; i < model->n_trees; i++){
		prediction += model->learning_rate * PredictXGBTree(model->trees[i], features, sample_index);
	}
	return prediction;
}

void FreeXGB(XGBoost *model)
{
	for(int i=0; i < model->n_trees; i++){
		FreeXGBTree(model->trees[i]);
	}
	free(model->trees);
	free(model);
}

void FreeXGBTree(XGBTree *model)
{
	if(model == NULL) return;
	FreeXGBTree(model->left);
	FreeXGBTree(model->right);
	free(model);
}
