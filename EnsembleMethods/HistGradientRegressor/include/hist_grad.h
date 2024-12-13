#ifndef HIST_GRAD_H
#define HIST_GRAD_H


#include<stdio.h>
#include<cmath.h>
#include<stdlib.h>
#include "../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"
#include "matrix.h"

typedef struct {
	DecisionTree **trees;
	double **x_bins;
	int *bins;
	int n_estimators;
	int n_bins;
	double learning_rate;
	double min_samples_split;
	double max_leaf_node;
	double l2_reg;
} HistGradientRegressor;

HistGradientRegressor *CreateHistGradRegressor(int n_bins, int n_estimators,  double learning_rate, double min_samples_split, int max_leaf_node, double l2_reg);
void TrainHistGradRegressor(HistGradientRegressor *model, Matrix *X, double *Y, double samples);
double PredictHistGradRegressor(HistGradientRegressor *model, Matrix *X, int sample_index);
void FreeHistGradregressor(HistGradientRegressor *model);



#endif //HIST_GRAD_H
