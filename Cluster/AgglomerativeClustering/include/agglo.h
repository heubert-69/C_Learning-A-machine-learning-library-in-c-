#ifndef AGGLO_H
#define AGGLO_H

#include "matrix.h"

typedef struct {
	int *labels;
	int n_samples;
} AgglomerativeClustering;

AgglomerativeClustering *CreateAggolmerativeCluster(Matrix X, int n_clusters, char *linkage);
void FreeAgglomerativeClustering(AgglomerativeClustering *model);


#endif //AGGLO_H
