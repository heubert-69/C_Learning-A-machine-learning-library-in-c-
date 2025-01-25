#ifndef RADIUS_H
#define RADIUS_H

#include<stdlib.h>
#include<stdio.h>
#include "matrix.h"

typedef struct {
	Matrix *features;
	Matrix *labels;
	double radius;
} RadiusNeighborsClassifier;



RadiusNeighborsClassifier *CreateRadiusClassifier(double radius);
void TrainRadiusClassifier(RadiusNeighborsClassifier *model, Matrix *X, Matrix *Y);
int PredictSingleRadius(RadiusNeighborsClassifier *model, Matrix *X, int sample_index);
Matrix *PredictRadiusClassifier(RadiusNeighborsClassifier *model, Matrix *X);
void FreeRadiusClassifier(RadiusNeighborsClassifier *model);

#endif //RADIUS_H
