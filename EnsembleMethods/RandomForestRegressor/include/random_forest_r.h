#ifndef RANDOM_FOREST_R
#define RANDOM_FOREST_R

#include "matrix.h"
#include "../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"

typedef struct {
	DecisionTree **trees
	int n_trees;
	int max_depth;
	int samples_split; //number of samples to split
	double feature_fraction; //fractions of features to consider for each split
} RandomForestRegressor;


RandomForestRegressor *CreateForestRegressor(int n_trees, int max_depth, int samples_split, double feature_fraction);
void TrainForestRegressor(RandomForestRegressor *model, Matrix *X, int *labels);
double PredictForestRegressor(RandomForestRegressor *model, Matrix *X, int sample_index);
void FreeForestRegressor(RandomForestRegressor *model);


#endif //RANDOM_FOREST_R

