#ifndef CATBOOST_H
#define CATBOOST_H

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "matrix.h"

typedef struct CATBoostTree {
	int is_leaf;
	double leaf_value;
	int feature_index;
	double threshold;
	struct CATBoostTree *left;
	struct CATBoostTree *right;
} CATBoostTree;

typedef struct CATBoost {
	CATBoostTree **trees;
	int trees;
	int *categorical features; //array to store the features of categorical data
	double learning_rate;
	double lambda;
	int c_categorical_features; //categorical features length
} CATBoost;

CATBoostTree *BuildCATBoostTree(Matrix *features, double *gradients, int samples, int n_features, int *ordered_indices, int max_depth, double lambda);
CATBoost *TrainCATBoost(Matrix *features, double *labels, int samples, int n_features, int n_features, int trees, int max_depth, double learning_rate, double lambda, int *categorical_features, int n_categorical_features);
double PredictCATBoost(CATBoost *model, Matrix *features, int sample_index);
void FreeCATBoost(CATBoost *model);
void FreeCATBoostTree(CATBoostTree *model);


#endif //CATBOOST_H
