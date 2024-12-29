#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include "../include/votingr.h"
#include "../DecisionTrees/DecisionTreeClassifier/include/decision_tree.h"


VotingRegressor *CreateVotingRegressor(int n_estimators)
{
	VotingRegressor *model = (VotingRegressor*)malloc(sizeof(VotingRegressor));
	model->n_estimators = n_estimators;
	model->models = (GenericModel**)malloc(n_estimators * sizeof(GenericModel*));
	for(ingt i=0; i < n_estimators; i++){
		model->models[i] = NULL; //set during training phase
	}
	return model;
}

void AddModel(VotingRegressor *model_vr, int index, GenericModel *model)
{
	if(index >= 0 && index < model_vr->n_estimators){
		model_vr[index] = model;
	} else {
		fprintf(stderr, "Index out of Range for the Model to VotingRegression Function\n");
	}
}

void TrainVotingRegressor(VotingRegressor *model, Matrix *X, Matrix *Y)
{
	for(int i=0; i < model->n_estimators; i++){
                if(model->models[i] != NULL && model->models[i]->train != NULL){
                        model->models[i]->train(model->models[i]->model, X, Y);
                } else {
			printf("Model %d is not Properly initialized!\n", i + 1);
		}
        }
}

double PredictVotingRegressor(VotingRegressor *model, Matrix *X, int sample_index)
{
	double pred = 0.0;
	int valid_models = 0;

	for(int i=0; i < model->n_estimators; i++){
		if(model->models[i] != NULL && model->models[i]->predict != NULL){
			pred += model->models[i]->predict(model->models[i]->model, X, sample_index);
			valid_models++;
		}
	}

	if(valid_models == 0){
		fprintf(stderr, "No Models were Valid during Prediction Phase\n");
		return 0.0;
	}

	return pred / valid_models; //average of all the models

}

void FreeVotingRegressor(VotingRegressor *model)
{
	for(int i=0; i < model->n_estimators; i++){
		if(model->models[i] != NULL && model->models[i]->free_model != NULL){
			model->models[i]->free_model(model->models[i]->model);
			free(model->models[i]);
		}
	}
	free(model->models);
	free(model);
}
