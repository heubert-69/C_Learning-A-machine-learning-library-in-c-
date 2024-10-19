#ifndef GRADIENT_TREES_H
#define GRADIENT_TREES_H


#include<stdlib.h>
#include<stdio.h>
#include "../ExtraTrees/include/extra_trees.h"

//tree structure for the tree cause this one is built diffrent
typedef struct Tree {
	int is_leaf;
	int feature_index;
	double threshold;
	double leaf_value;
	struct Tree *left;
	struct Tree *right;
} Tree;

Tree* BuildGBTTree(Matrix *features, double *residuals, int samples, int n_features, int max_depth);
double PredictGBTTree(Tree *node, Matrix *features, int sample_index);
void FreeGBTTree(Tree *node);

//when used by gradient boost
typedef struct {
	int n_trees;
	ExtraTrees trees;
	double learning_rate;
} GradientBoostedTrees;



GradientBoostedTrees *TrainGBT(Matrix *features, int *labels, int samples, int n_features, int trees,  double learning_rate);
double PredictGBT(GradientBoostedTrees *model, Matrix *features, int sample_index);
void FreeGBT(GradientBoostedTrees *model);



//helper function in secret
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





#endif //GRADIENT_TREES_H
