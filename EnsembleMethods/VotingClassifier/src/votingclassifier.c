#include<stdlib.h>
#include<stdio.h>
#include "../include/votingc.h"



VotingClassifier* CreateVotingClassifier(GenericC *classifiers, int n_classifiers)
{
	VotingClassifier *model = (VotingClassifier*)malloc(sizeof(VotingClassifier));
	model->n_classifiers =  n_classifiers;
	model->classifiers = classifiers;
	return model;
}

void TrainVotingClassifier(VotingClassifier *model, Matrix *features, int *labels, int n_samples)
{
	for(int i=0; i < model->n_classifiers; i++){
		model->classifiers[i].train(model->classifiers[i].model, features, labels, n_samples);
	}
}

//assuming that all outputs must be binary (0 or 1)
int PredictVotingClassifier(VotingClassifier *model, Matrix *features, int sample_index)
{
	int *votes = (int*)calloc(2, sizeof(int));

	for(int i=0; i < model->n_classifiers; i++){
		int prediction = model->classifiers[i].predict(model->classifiers[i].model, features, sample_index);
		votes[prediction]++;
	}
	int final_prediction = (votes[0] > votes[1]) ? 0 : 1;
	free(votes);
	return final_prediction;
}

void FreeVotingClassifier(VotingClassifier *model)
{
	for(int i=0; i < model->n_classifiers; i++){
		model->classifiers[i].free_model(model->classifiers[i].model);
	}
	free(model->classifiers);
	free(model);
}

