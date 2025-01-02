#include<stdlib.h>
#include<stdio.h>
#include "../include/stackingc.h"



StackingClassifier* CreateStackingClassifier(GenericClassifierM *base_classifier, int stacks, GenericClassifierM meta_classifier)
{
	StackingClassifier *model = (StackingClassifier*)malloc(sizeof(StackingClassifier));
	model->stacks = stacks;
	model->base_classifier = base_classifier;
	model->meta_classifier = meta_classifier;
	return model;
}

void TrainStackingClassifier(StackingClassifier *model, Matrix *features, double *labels, int samples)
{
	Matrix *meta_features = CreateMatrix(samples, model->stacks);
	for(int i=0; i < model->stacks; i++){
		model->base_classifier[i].train(model->base_classifier[i].model, features, labels, samples);
		//generating meta features
		for(int x=0; x < samples; x++){
			int prediction = model->base_classifier[i].predict(model->base_classifier, features, x);
			set_value(meta_features, x, i, (double)prediction);
		}
	}
	model->meta_classifier.train(model->meta_classifier.model, meta_features, labels, samples);
	FreeMatrix(meta_features);
}

double PredictStackingClassifier(StackingClassifier *model, Matrix *features, int sample_index)
{
	double *meta_features = (double*)malloc(model->stacks * sizeof(double));

	for(int i=0; i < model->stacks; i++){
		meta_features[i] = (double)model->base_classifier[i].predict(model->base_classifier.model, features, sample_index);
	}
	//predict final value
	Matrix *meta_features_m = CreateMatrix(1, model->stacks);

	for(int i=0; i < model->stacks; i++){
		set_value(meta_features_m, 0, i, meta_features[i]);
	}

	int final_pred = model->meta_classifier.predict(model->meta_classifier.model, meta_features_m, 0);

	free(meta_features);
	FreeMatrix(meta_features_m);

	return final_pred;
}

void FreeStackingClassifier(StackingClassifier *model)
{
	for(int i=0; i < model->stacks; i++){
		model->base_classifier[i].free_model(model->base_classifier[i].model);
	}

	model->meta_classifier.free_model(model->meta_classifier.model);

	free(model->base_classifier);
	free(model);
}

