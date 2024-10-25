#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"
#include "../include/lightgbm.h"

void CalculateGradients(double *gradients, double *hessians, double *labels, double *predictions, int samples)
{
        for(int i=0; i < samples; i++){
                gradients[i] = predictions[i] - labels[i]; //calculating the accurate actual
                hessians[i] = 1.0;
        }
}

//Histogram Based split finding
double FindBestSplit(Matrix *features, double *gradients, double *hessians, int samples, int feature_index, double *best_threshold, double lambda)
{
	double best_gain = -INFINITY;
	double sum_hess_left = 0.0, sum_grad_left = 0.0, sum_grad_right = 0.0, sum_hess_right = 0.0;

	for(int i=0; i < samples; i++){
		double feature_value = get_value(features, i, feature_index);

		sum_grad_right += gradients[i];
		sum_hess_right += hessians[i];

		double left_loss = pow(sum_grad_left, 2) / (sum_hess_left + lambda);
		double right_loss = pow(sum_grad_right, 2) / (sum_hess_right + lambda);
		double total_gain = right_loss + left_loss;

		if(total_gain > best_gain){
			best_gain = total_gain;
			*best_threshold = feature_value;
		}

		//update left hess and grad value
		sum_left_hess += hessians[i];
		sum_left_grad += gradients[i];
	}
	return best_gain;
}

LightGBMTree *BuildLGBMTree(Matrix *features, double *gradients, double hessians, int samples, int n_features, int max_depth, double gamma, double lamda)
{
	//base case
	if(max_depth == 0){
		LightGBMTree *model = (LightGBMTree*)malloc(sizeof(LightGBMTree));
		model->is_leaf = 1; //True
		double sum_grad = 0.0, sum_hess = 0.0;
		for(int i=0; i < samples; i++){
			sum_grad += gradients[i];
			sum_hess += hessians[i];
		}
		model->leaf_value = -sum_grad / (sum_hess + lambda);
		model->left = NULL;
		return model;
	}


	LightGBMTree *model = (LightGBMTree*)malloc(sizeof(LightGBMTree));
	model->is_leaf = 0; //False
	model->feature_index = rand() % n_features;

	double best_threshold = 0.0;
	double best_gain = FindBestSplit(features, gradients, hessians, samples, model->feature_index, &best_threshold, lambda);

	if(best_gain < gamma){
		LightGBMTree *model = (LightGBMTree*)malloc(sizeof(LightGBMTree));
                model->is_leaf = 1; //True
                double sum_grad = 0.0, sum_hess = 0.0;
                for(int i=0; i < samples; i++){
                        sum_grad += gradients[i];
                        sum_hess += hessians[i];
                }
                model->leaf_value = -sum_grad / (sum_hess + lambda);
                return model;
	}

	model->threshold = best_threshold;


	int *left_indices = (int*)malloc(samples * sizeof(int));
	int *right_indices = (int*)malloc(samples * sizeof(int));
	int left_size, right_size;
	split_data(features, gradients, samples, model->feature_index, model->threshold, left_indices, right_indices, left_indices, right_indices, &left_size, &right_size);

	model->left = BuildLGBMTree(features, gradients, hessians, left_size, n_features, max_depth-1, gamma, lambda);
        model->right = BuildLGBMTree(features, gradients, hessians, right_size, n_features, max_depth-1, gamma, lambda);

        free(right_indices);
        free(left_indices);

        return model;
}

LightGBM *TrainLGBM(Matrix *features, double *labels, int samples, int features, int n_trees, int max_depth, double learning_rate, double gamma, double lambda)
{
	LightGBM *model = (LightGBM*)malloc(sizeof(LightGBM));
        model->trees = (LightGBMTree**)malloc(n_trees * sizeof(LightGBM*));
        model->n_trees = n_trees;
        model->learning_rate = learning_rate;
        model->gamma = gamma;
        model->lambda = lambda;

        double *predictions = (double*)calloc(samples, sizeof(double));
        double *gradients = (double*)malloc(samples * sizeof(double));
        double *hessians = (double*)malloc(samples * sizeof(double));

        for(int i=0; i < trees; i++){
                CalculateGradients(gradients, hessians, labels, predictions, samples);

                model->trees[i] = BuildLGBMTree(features, gradients, hessians, samples, n_features, max_depth, gamma, lambda);

		 for(int x=0; x < samples; x++){
                        predictions[x] += learning_rate * PredictXGB(model->trees[i], features, x);
                }
        }

        free(predictions);
        free(hessians);
        free(gradients);

        return model;
}

double PredictLGBMTree(LightGBMTree *model, Matrix *features, int sample_index)
{
        if(model->is_leaf){
                return model->leaf_value;
        }
        double value = get_value(features, sample_index, model->feature_index);
        if(value <= model->threshold){
                return PredictLGBMTree(model->left, features, sample_index);
        } else {
                return PredictLGBMTree(model->right, features, sample_index);
        }
}


double PredictLGBM(LightGBM *model, Matrix *features, int sample_index)
{
        double prediction = 0.0;
        for(int i=0; i < model->n_trees; i++){
                prediction += model->learning_rate * PredictLGBMTree(model->trees[i], features, sample_index);
        }
        return prediction;
}

//free memory
void FreeLGBM(LightGBM *model)
{
        for(int i=0; i < model->n_trees; i++){
                FreeXGBTree(model->trees[i]);
        }
        free(model->trees);
        free(model);
}

void FreeLGBMTree(LightGBMTree *model)
{
        if(model == NULL) return;
        FreeXGBTree(model->left);
        FreeXGBTree(model->right);
        free(model);
}

