#ifndef RANDOM_FOREST_C
#define RANDOM_FOREST_C

#include "matrix.h"
#include "../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"

typedef struct {
	DecisionTree **trees;
	int n_trees;
	int max_depth;
	int min_samples_split;
	double feature_fraction;
} RandomForestClassifier;


RandomForestClassifier *CreateRandomForestClassifier(int n_trees, int max_depth, int min_samples_split, double feature_fraction);
void TrainRandomForestClassifier(RandomForestClassifier *model, Matrix *features, double *labels, int n_classes);
int  TrainRandomForestClassifier(RandomForestClassifier *model, Matrix *X, int n_classes, int sample_index);
void FreeRandomForestClassifier(RandomForestClassifier *model);

#endif //RANDOM_FOREST_C
