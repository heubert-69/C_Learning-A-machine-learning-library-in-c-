#ifndef ADABOOST_H
#define ADABOOST_H

#include "matrix.h"
#include "../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"


typedef struct {

	DecisionTree **WeakTrees
	double alphas;
	int n_estimators;
	int max_depth;
} ADABoost;


ADABoost *CreateADA(int n_estimators, int max_depth);
void TrainADA(ADABoost *model, Matrix *X, int *Y, int samples);
int PredictADA(ADABoost *model, Matrix *X, int sample_index);
void FreeADA(ADABoost *model);


#endif //ADABOOST_H
