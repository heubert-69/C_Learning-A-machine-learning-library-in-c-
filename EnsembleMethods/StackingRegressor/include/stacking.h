#ifndef STACKING_H
#define STACKING_H

#include "matrix.h"


//generic model to stack any kind of model to train a meta learner
typedef struct {
	void *model;
	void (*train)(void *model, Matrix *features, double *labels, int n_samples);
	double (*predict)(void *model, Matrix *features, int sample_index);
	void (*free_model)(void *model);
} GenericRegressorM;



typedef struct {
	GenericRegressorM *stacking_r;
	int stacks;
	GenericRegressorM meta_regressor
} StackingRegressor;

StackingRegressor* CreateStackingRegressor(GenericRegressorM *stacked_model, int stacks, GenericRegressorM meta_regressor);
void TrainStackingRegressor(StackingRegressor *model, Matrix *features, double *labels, int n_samples);
double PredictStackingRegressor(StackingRegressor *model, Matrix *features, int sample_index);
void FreeStackingRegressor(StackingRegressor *model);


#endif //STACKING_H
