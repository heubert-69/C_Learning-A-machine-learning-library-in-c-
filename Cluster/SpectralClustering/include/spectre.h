#ifndef SPECTRE_H
#define SPECTRE_H

#include "matrix.h"

typedef struct {
	int *labels;
	int n_clusters;
	int n_samples;
} SpectreClustering;

SpectralClustering *CreateSpectre(Matrix *X, int n_clusters, double sigma);

void FreeSpectralClustering(SpectralClustering *model);

#endif //SPECTRE_H
