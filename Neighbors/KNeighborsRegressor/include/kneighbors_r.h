#ifndef KNEIGHBORS_R_H
#define KNEIGHBORS_R_H


#include "matrix.h"





typedef struct {
	Matrix *features;
	Matrix *targets;
	int n_neighbors;
} KNeighborsRegressor;


KNeighborsRegressor *CreateNeighborsRegressor(int n_neighbors);
void TrainNeighborsRegressor(KNeighborsRegressor *model, Matrix *X, Matrix *Y);
double PredictSingleNeighborsRegressor(KNeighborsRegressor *model, Matrix *X, char metric, int sample_index);
Matrix *PredictNeighborsRegressor(KNeighborsRegressor *model, Matrix *X, char metric);
void FreeNeighborsRegressor(KNeighborsRegressor *model);

#endif //KNEIGHBORS_R_H
