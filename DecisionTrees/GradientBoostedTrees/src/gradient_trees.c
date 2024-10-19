#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<float.h>
#include "../include/matrix.h"
#include "../include/gradient_trees.h"



//calculating the errors
void CalculateResiduals(Matrix *features, int *labels, GradientBoostedTrees *model, double *residuals, int samples)
{
	for(int i=0; i < samples; i++){
		double predictions = PredictGBT(model, features, i);
		residuals[i] = labels[i] - prediction;
	}
}

//helper function
double PredictGBTTree(Tree *node, Matrix *features, int sample_index)
{
	if(node->is_leaf){
		return node->leaf_value;
	}
	double value = get_value(features, sample_index, node->feature_index);
	if(value <= node->threshold){
		return PredictGBT(node->left, features, sample_index);
	} else {
		return PredictGBT(node->right, features, sample_index);
	}
}

Tree *BuildGBTTree(Matrix *features, double *residuals, int samples, int n_features, int max_depth)
{
	//if max_depth == actual max_depth or the residuals are small, return a leaf node
	if(max_depth == 0){
		Tree *leaf = (Tree*)malloc(sizeof(Tree));
		leaf->is_leaf = 1;
		double residual_sum = 0.0;
		for(int i=0; i < samples; i++){
			residual_sum += residuals[i];
		}
		leaf->leaf_value = residual_sum / samples;
		leaf->left = leaf->right = NULL;
		return leaf;
	}

	Tree *leaf = (Tree*)malloc(sizeof(Tree));
	leaf->is_leaf = 0;
	leaf->feature_index = rand() % n_features;
	leaf->threshold = GetRandomThreshold(features, leaf->feature_index, samples);

	//split it into left and right
	int *left_indices = (int*)malloc(samples * sizeof(int));
	int *right_indeices = (int*)malloc(samples * sizeof(int));

	int right_size, left_size;
	split_data(features, residuals, samples, leaf->feature_index, leaf->threshold, left_indices, right_indices, &left_size, &right_size);

	//recursively build both left and right side of trees
	leaf->left = BuildGBTTree(features, residuals, left_size, n_features, max_depth - 1);
	leaf->right = BuildGBTTree(features, residuals, right_size, n_features, max_depth - 1);

	free(left_indices);
	free(right_indices);

	return leaf;
}


GradientBoostedTrees *TrainGBT(Matrix *features, int *labels, int samples, int n_features, int trees, double learning_rate)
{
	GradientBoostedTrees *model = (GradientBoostedTrees*)malloc(sizeof(GradientBoostedTrees));
	model->n_trees = trees;
	model->learning_rate = learning_rate;
	model->trees = (Tree**)malloc(trees * sizeof(Tree*));


	double predictions = (double*)calloc(samples, sizeof(double));
	double residuals = (double*)calloc(samples * sizeof(double));

	for(int i=0; i < trees; i++){
		CalculateResiduals(residuals, predictions, label, samples); //current errors

		nodel->trees[i] = BuildGBT(features, residuals, samples, features, max_depth);

		//update predictions
		for(int x=0; x < samples; x++){
			predictions[x] += learning_rate * PredictGBT(model->trees[i], features, x);
		}
	}

	free(predictions);
	free(residuals);
	return model;
}

double PredictGBT(GradientBoostedTrees *model, Matrix *features, int sample_index)
{
        double predictions = 0.0;
	for(int i=0; i < model->n_trees; i++){
		predictions += model->learning_rate * PredictGBTTree(model->trees[i], features, sample_index);
	}
	return predictions
}


void FreeGBT(GradientBoostedTrees *model)
{
	for(int i=0; i < model->n_trees; i++){
		FreeGBTTree(model->trees[i]);
	}
	free(model->trees);
	free(model);
}

void FreeGBTTree(Tree *node)
{
	if(node == NULL) return;
	FreeGBTTree(node->left);
	FreeGBTTree(node->right);
	free(node);
}
