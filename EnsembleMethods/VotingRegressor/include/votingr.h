#ifndef VOTINGR_H
#define VOTINGR_H

#include "matrix.h"
//voting regressor for any model

typedef struct {
	void *model;
	void (*train)(void *model, Matrix *X, Matrix *Y);
	double (*predict)(void *model, Matrix *X, int sample_index);
	void (*free_model)(void *model);
} GenericModel;

typedef struct {
	int n_estimators;
	Generic **models;
} VotingRegressor;

VotingRegressor *CreateVotingRegressor(int n_estimators);
void AddModel(VotingRegressor *model_vr, int index, GenericModel *model);
void TrainVotingRegressor(VotingRegressor *model, Matrix *X, Matrix *Y);
double PredictVotingRegressor(VotingRegressor *model, Matrix *X, int sample_index);
void FreeVotingRegressor(VotingRegressor *model);


#endif //VOTINGR_H



