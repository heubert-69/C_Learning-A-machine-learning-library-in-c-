#ifndef FEARURE_A_H
#define FEATURE_A_H

#include "matrix.h"

typedef struct {
	int *labels;
	int n_features;
	Matrix *reduced; //feature matrix with restrictions on size
} FeatureAgglomeration;


FeatureAgglomeration *CreateFeatureAgglomeration(Matrix *X, int n_clusters, char *linkage);
void FreeFeatureAgglomeration(FeatureAgglomeration *model);

#endif
