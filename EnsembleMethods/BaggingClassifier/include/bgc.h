#ifndef BGC_H
#define BGC_H

#include "matrix.h"
#include "../DecisionTrees/DecisionTreeClassifier/decision_tree.h"
#include<stdlib.h>

typedef struct {
	DecisionTree **trees;
	int max_depth;
	int n_estimators;
} BGC;

BGC* CreateBGC(int n_estimators, int max_depth);
void TrainBGC(BGC *model, Matrix *X, int *Y, int samples);
int PredictBGC(BGC *model, Matrix *X, int sample_index);
void FreeBGC(BGC *model);


#endif //BGC_H

