#ifndef BAGGINGR_H
#define BAGGINGR_H

#include "matrix.h"
#include "../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"

typedef struct {
	DecisionTree **trees;
	int n_estimators;
	int max_samples;
	int max_depth;
} BaggingRegressor;


BaggingRegressor *CreateBaggingRegressor(int n_estimators, int max_samples, int max_depth);
void TrainBGRegressor(BaggingRegressor *model, Matrix *X, Matrix *Y);
Matrix *PredictBGRegressor(BaggingRegressor *model, Matrix *X);
void FreeBGRegressor(BaggingRegressor *model);

#endif //BAGGINGR_H
