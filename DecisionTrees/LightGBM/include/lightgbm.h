#ifndef LIGHTGBM_H
#define LIGHTGBM_H

#include "matrix.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>



//making a tree structure for light gbm decision tree variant
typedef struct LightGBMTree {
	int is_leaf;
	int feature_index;
	double threshold;
	double leaf_value;
	struct LightGBMTree *left;
	struct LightGBMTree *right;
} LightGBMTree;

//light gradient boosting machine for a decision tree
typedef struct LightGBM {
	int n_trees;
        double gamma;
        double lambda;
        double learning_rate;
	LightGBMTree **trees;
} LightGBM;

LightGBMTree *BuildLGBMTree(Matrix *features, double *gradients, double hessians, int samples, int n_features, int max_depth, double gamma, double lambda);
LightGBM *TrainLGBM(Matrix *features, double *labels, int samples, int features, int n_trees, int max_depth, double learning_rate, double gamma, double lambda);
double PredictLGBM(LightGBM *model, Matrix *features, int sample_index);
void FreeLGBMTree(LightGBMTree *model);
void FreeLightGBM(LightGBM *model);

#endif //LIGHTGBM_H

