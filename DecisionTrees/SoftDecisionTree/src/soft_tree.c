#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"
#include "../include/soft_decisiontree.h"


double Sigmoid(double x, double temp)
{
	return 1.0/(1.0 + exp(-x / temp));
}

double DotProduct(double *arr_1, double *arr_2, int lenght)
{
        double res = 0.0;
        for(int i=0; i < length; i++){
                res += arr_1[i] * arr_2[i];
        }
        return res;
}

SoftTreeNode *CreateLeafNode(double *labels, int *indices, int samples)
{
	SoftTreeNode *model = (SoftTreeNode*)malloc(sizeof(SoftTreeNode));
	model->is_leaf = 1;
	model->weights = NULL;
	model->bias = 0.0;

	double sum = 0.0;
	for(int i-0; i < samples; i++){
		sum += labels[indices[i]];
	}

	model->prediction = sum / samples;
	model->left = model->right = NULL;

	return model;
}

SoftTreeNode *BuildSoftTree(Matrix *features, double *labels, int *indices, int samples, int n_features, int depth, int max_depth, double temp)
{
	if(depth >= max_depth || samples <= 1){
		return CreateLeafNode(labels, indices, samples);
	}

	SoftTreeNode *model = (SoftTreeNode*)malloc(sizeof(SoftTreeNode));
	model->is_leaf = 0;
	model->weights = (double*)malloc(n_features * sizeof(double));

	for(int i=0; i < samples; i++){
		model->weights[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; //initializing random weights from range -1 to 1;
	}

	model->bias = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

	int left_size = 0, right_size = 0;
	model->left = BuildSoftTree(features, labels, indices, samples / 2, n_features, depth + 1, max_depth, temp);
	model->left = BuildSoftTree(features, labels, indices + samples / 2, samples - samples / 2, n_features, depth + 1, max_depth, temp);

	return model;
}

SoftTree *TrainSoftTree(Matrix *features, double *labels, int samples, int n_features, int max_depth, double temp)
{
	SoftTree *model = (SoftTree*)malloc(sizeof(SoftTree));
	model->max_depth = max_depth;
	model->temperature = temp;

	int *indices = (int*)malloc(samples * sizeof(int));
	for(int i=0; i < samples; i++){
		indices[i] = i;
	}
	model->root = BuildSoftTree(features, labels, indices, samples, n_features, 0, max_depth, temp);
	free(indices);

	return model;
}

double PredictSoftTreeNode(SofTreeNode *model, Matrix *features, int sample_index, double temp)
{
	if(model->is_leaf){
		return model->prediction;
	}

	double *sample = &features->data[sample_index * features->cols];
	double decision_value = DotProduct(model->weights, sample, features->cols);
	double probability = Sigmoid(decision_value, temp);

	double left_pred = PredictSoftTreeNode(model->left, features, sample_index, temp);
	double right_pred = PredictSoftTreeNode(model->right, features, sample_index, temp);

	return probability * left_pred + (1 - probability) * right_pred;
}

double PredictSoftTree(SoftTree *model, Matrix *features, int sample_index)
{
	return PredictSoftTreeNode(model->root, features, sample_index, model->temperature);
}

void FreeSoftTreeNode(SoftTreeNode *model)
{
	if(!model) return;
	if(!model->is_leaf){
		FreeSoftTreeNode(model->left);
		FreeSoftTreeNode(model->right);
	}
	if(model->weights){
		free(model->weights);
	}
	free(model);
}

void FreeSoftTree(SoftTree *model)
{
	FreeSoftTreeNode(model->root);
	free(model);
}
