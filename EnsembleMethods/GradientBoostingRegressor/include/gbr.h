#ifndef GBR_H
#define GBR_H


#include<iostream>
#include "../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"

typedef struct
{
	DecisionTree **trees;
	double learning_rate;
	int max_depth;
	int n_estimators;
} GBR;


GBR* CreateGBR(int n_estimators, double learning_rate, int max_depth);
void TrainGBR(GBR* model, Matrix* X, double *Y, int samples);
double PredictGBR(GBR *model, Matrix *X, int sample_index);
void FreeGBR(GBR *model);


#endif //GBR_H
