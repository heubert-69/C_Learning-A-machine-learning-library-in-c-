#ifndef EXTRA_TREES_H
#define EXTRA_TREES_H


#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "matrix.h"
#include "../CHAID/include/chaid.h"

typedef struct {
	CHAID **trees;
	int n_trees;
} ExtraTrees;


ExtraTrees *CreateExtraTrees(Matrix *features, int *labels, int samples, int n_features, int n_trees, int max_features);
int ClassifyExtraTrees(ExtraTrees *forest, Matrix *features, int sample_index);
void FreeExtraTrees(ExtraTrees *forest);


#endif //EXTRA_TREES_H
