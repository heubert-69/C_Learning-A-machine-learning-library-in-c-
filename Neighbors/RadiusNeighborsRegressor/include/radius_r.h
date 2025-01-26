#ifndef RADIUS_R_H
#define RADIUS_R_H

#include "matrix.h"

typedef struct {
	Matrix *features;
	Matrix *labels;
	double radius;
} RadiusNeighborsRegressor;

RadiusNeighborsRegressor *CreateRadiusRegressor(double radius);
void TrainRadiusNeighborsRegressor(RadiusNeighborsRegressor *model, Matrix *X, Matrix *Y);
double PredictSingleRadiusNeighborsRegressor(RadiusNeighborsRegressor *model, Matrix *X, int sample_index);
Matrix *PredictRadiusNeighborsRegressor(RadiusNeighborsRegressor *model, Matrix *X);
void FreeRadiusRegressor(RadiusNeighborsRegressor *model);


#endif //RADIUS_R_H


