#ifndef HIST_CLASSIFIER_H
#define HIST_CLASSIFIER_H

#include "matrix.h"
#include "../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"
#include<cmath.h>
#include<stdlib.h>


typedef struct {
	DecisionTree **trees;
	int n_estimators;
	double learning_rate;
	int max_depth;
	int n_bins;
	Matrix *gradients;
	Matrix *residuals;
} HistGradientClassifier;

HistGradientClassifier *CreateHistClassifier(int n_estimators, double learning_rate);
void TrainHistClassifier(HistGradientClassifier *model, Matrix *X, Matrix *Y);
Matrix* PredictHistClassifier(HistGradientClassifier *model, Matrix *X);
void FreeHistClassifier(HistGradientClassifier *model);

#endif //HIST_CLASSIFIER_H



