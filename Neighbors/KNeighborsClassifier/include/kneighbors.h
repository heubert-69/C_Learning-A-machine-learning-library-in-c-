#ifndef KNEIGHBORS_H
#define KNEIGHBORS_H

#include "matrix.h"

typedef struct {
	Matrix *features;
	int *labels;
	int n_samples;
	int n_features;
	int k;
	char distance_metric;
} KNeighborsClassifier;

KNeighborsClassifier *CreateNeighborsClassifier(int k, char distance_metric);
void TrainNeighborsClassifier(KNeighborsClassifier *model, Matrix *X, int *labels);
int PredictSingle(KNeighborsClassifier *model, Matrix *X,  int sample_index); //what this means is it will predict the label of a single sample
int *PredictKNeighborsClassifier(KNeighborsClassifier *model, Matrix *X);
void FreeKNeighborsClassifier(KNeighborsClassifier *model);

#endif //KNEIGHBORS_H
