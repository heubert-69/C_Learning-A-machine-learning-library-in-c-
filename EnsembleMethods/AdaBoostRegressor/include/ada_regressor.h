#ifndef ADA_REGRESSOR_H
#define ADA_REGRESSOR_H

#include "matrix.h"
#include "../DecisionTrees/DecisionTreeClassfier/include/decision_tree.h"
#include<stdlib.h>
#include<cmath.h>

typedef struct {
	DecisionTree** WeakTrees;
	int n_etimators;
	int max_depth;
	double *alphas;
} ADARegressor;

ADARegressor* CreateADARegression(int n_estimators, int max_depth);
void TrainADARegressor(ADARegressor *model, Matrix *X, double *Y, int num_samples);
double PredictADARegressor(ADARegressor *model, Matrix *X, int sample_index);
void FreeADARegressor(ADARegressor *model);



#endif //ADA_REGRESSOR_H
