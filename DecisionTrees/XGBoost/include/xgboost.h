#ifndef XGBOOST_H
#define XGBOOST_H

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "matrix.h"

//create a tree structure for gradient boosting
typedef struct XGBTree {
	int is_leaf;
	int feature_index;
	int threshold;
	int leaf_value;
	struct XGBTree *left;
	struct XGBTree *right;
} XGBTree;

typedef struct XGBoost {
	XGBTree **trees;
	int n_trees;
	double learning_rate;
	double lambda;
	double gamma; //minimal loss to make a split in trees
} XGBoost;


XGBTree *BuildXGBTree(Matrix *features, double *gradients, double *hessians, int samples, int n_features, int max_depth, double gamma, double lambda);
XGBoost *TrainXGB(Matrix *features, double *labels, int samples, int n_features, int trees, int max_depth, double learning_rate, double gamma, double lambda);
double PredictXGB(XGBoost *model, Matrix *features, int sample_index);
void FreeXGBTree(XGBTree *model);
void FreeXGB(XGBoost *model);

#endif //XGBOOST_H
