#ifndef VOTINGC_H
#define VOTINGC_H

#include "matrix.h"

//function pointers for generic models used (a free generic model to use the funtions made in a model) 
//can be used for linear models, support vector classifiers and etc.
typedef struct {
	void *model;
	void (*train)(void *model, Matrix *features, int *labels, int samples);
	int (*predict)(void *model, Matrix *features, int sample_index);
	void (*free_model)(void *model);
} GenericC;


typedef struct {
	GenericC *classifiers;
	int n_classifiers;
} VotingClassifier;

VotingClassifier* CreateVotingClassifier(GenericC *classifiers, int n_classifiers);
void TrainVotingClassifier(VotingClassifier *model, Matrix *features, int *labels, int n_samples);
int PredictVotingClassifier(VotingClassifier *model, Matrix *features, int sample_index);
void FreeVotingClassifier(VotingClassifier *model);

#endif //VOTINGC_H
