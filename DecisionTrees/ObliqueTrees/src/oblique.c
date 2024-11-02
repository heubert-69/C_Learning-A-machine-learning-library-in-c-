#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"
#include "../include/oblique.h"

double DotProduct(double *arr_1, double *arr_2, int lenght)
{
	double res = 0.0;
	for(int i=0; i < length; i++){
		res += arr_1[i] * arr_2[i];
	}
	return res;
}

ObliqueTree *CreateLeafNode(double *labels, int *indices, int samples)
{
	ObliqueTree *model = (ObliqueTree*)malloc(sizeof(ObiqueTree));
	model->is_leaf = 1;
	model->coefficients = NULL;
	model->threshold = 0.0;

	double sum = 0.0;
	for(int i=0; i < samples; i++){
		sum += labels[indices[i]];
	}
	model->prediction = sum/samples;
	model->left = model->right  = NULL;
	return model;
}

ObliqueTree *BuildObliqueTree(Matrix *features, double *labels, int *indices, int samples, int n_features, int depth, int max_depth)
{
	if(depth >= max_depth || samples <= 1){
		return CreateLeafNode(labels, indices, samples);
	}

	ObliqueTree *model = (ObliqueTree*)malloc(sizeof(ObliqueTree));
	model->is_leaf = 0;
	model->coefficients = (double*)malloc(n_features * sizeof(double));
	double *projections = (double*)malloc(n_samples * sizeof(double)); //sample data projections to be used later

	//assign random coefficients to make linear cmbinations for hyperplane split
	for(int i=0; i < n_features; i++){
		model->coefficents[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; //random values in the range of -1 to 1
	}

	//setting threshold based on sample data projections
	for(int i=0; i < samples; i++){
		projections[i] = DotProduct(model->coefficients, &features->data[indices[i] * &features->cols], n_features);
	}

	model->threshold = projections[samples / 2];

	int *left_indices = (int*)malloc(samples * sizeof(int));
        int *right_indices = (int*)malloc(samples * sizeof(int));
        int left_size = 0, right_size = 0;

	for(int i=0; i < samples; i++){
		if(projections[i] <= model->threshold){
			left_indices[left_size++] = indices[i];
		} else {
			right_indices[right_size++] = indices[i];
		}
	}

	free(projections);

	model->left = BuildObliqueTree(features, labels, left_indices, left_size, n_features, depth + 1, max_depth);
        model->right = BuildObliqueTree(features, labels, right_indices, right_size, n_features, depth + 1, max_depth);

        free(right_indices);
        free(left_indices);

        return model;
}

ObliqueTrees *TrainObliqueTrees(Matrix *features, double *labels, int samples, int n_features, int max_depth)
{
	ObliqueTree *model = (ObliqueTrees*)malloc(sizeof(ObliqueTrees));

	model->max_depth = max_depth;
	int *indices = (int*)malloc(samples * sizeof(int));
	for(int i=0; i < samples; i++){
		indices[i] = i;
	}
	model->root = BuildObliqueTree(features, labels, indices, samples, n_features, 0, max_depth);
	free(indices);

	return model;
}

double PredictTreeNode(ObliqueTree *model, Matrix *features, int sample_index)
{
	if(model->is_leaf){
		returm model->prediction;
	}

	double *sample = &features->data[sample_index * feature->cols];
	double projection = DotProduct(model->coefficients, sample, features->cols);

	if(projection <= model->threshold){
		return PredictTreeNode(model->left, features, sample_index);
	} else {
		 return PredictTreeNode(model->right, features, sample_index);
	}
}

double PredictTrees(ObliqueTrees *model, Matrix *features, int sample_index)
{
	return PredictTreeNode(model->root, features, sample_index);
}

void FreeObliqueTreeNode(ObliqueTree *model)
{
	if(!model) return;
	if(!model->is_leaf){
		FreeObliqueTreeNode(model->left);
		FreeObliqueTreeNode(model->right);
	}
	if(model->coefficients){
		free(model->coefficients);
	}

	free(model);
}

void FreeObliqueTrees(ObliqueTrees *model)
{
	FreeObliqueTreeNode(model->root);
	free(model);
}
