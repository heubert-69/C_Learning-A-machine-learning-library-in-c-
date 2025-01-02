#ifndef STACKINGC_H
#define STACKINGC_H

#include "matrix.h"

typedef struct {
	void *model;
	void (*train)(void *model, Matrix  *features, double *labels, int samples);
	void (*predict)(void *model, Matrix *features, int sample_index);
	void (*free_model)(void *model);
} GenericClassifierM;

typedef struct {
	GenericClassifierM *base_classifier;
	int stacks;
	GenericClassifierM *meta_classifier;
} StackingClassifier;


StackingClassifier* CreateStackingClassifier(GenericClassifierM *base_classifier, int stacks, GenericClassifierM meta_classifier);
void TrainStackingClassifier(StackingClassifier *model, Matrix *features, double *labels, int samples);
double PredictStackingClassifier(StackingClassifier *model, Matrix *features, int sample_index);
void FreeStackingClassifier(StackingClassifier *model);


#endif //STACKINGC_H
