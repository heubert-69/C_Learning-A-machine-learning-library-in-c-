#ifndef GBC_H
#define GBC_H

#include "matrix.h"
#include ".../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"

typedef struct
{
	DecisionTree **trees;
	int n_estimators;
	double learning_rate;
	int max_depth;
} GBC;

GBC* CreateGBC(int n_estimators, double lerning_rate, int max_depth);
void TrainGBC(GBC *model, Matrix *X, int *y, int num_samples); //X and y is for training
void FreeGBC(GBC *model);
double PredictGBC(GBC *model, Matrix *features, int sample_index);

#endif //GBC_H
