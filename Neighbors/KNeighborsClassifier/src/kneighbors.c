#include<stdlib.h>
#include<math.h>
#include<float.h>
#include "../include/kneighbors.h"


KNeighborsClassifier *CreateNeighborsClassifier(int k, char distance_metric)
{
	KNeighborsClassifier *model = (KNeighborsClassifier*)malloc(sizeof(KNeighborsClassifier));
	model->k = k;
	model->distance_metric = distance_metric;
	model->features = NULL;
	model->labels = NULL; //will be introduced during training
	model->n_samples = 0;
	model->n_features = 0;
	return model;
}


//for calculating euclidean distance or the manhattan distance of X and Y labels
double CalculateDistance(double *a, double *b, int n_features, char metric)
{
	double distance = 0.0;
	if(metric == 'e'){ //for euclidean distance
		for(int i=0; i < n_features; i++){
			double diff = a[i] - b[i];
			distance += diff * diff;
		}
		return sqrt(distance);
	} else if(metric == 'm'){
		for(int x=0; x < n_features; x++){
			distance += fabs(a[i] - b[i]);
		}
		return distance;
	} //for manhattan distance 

	return DBL_MAX;
}

void TrainNeighborsClassifier(KNeighborsClassifier *model, Matrix *X, int *labels)
{
	model->features = X;
	model->labels = labels;
	model->n_samples = X->rows;
	model->n_features = X->cols;
}



int PredictSingle(KNeighborsClassifier *model, Matrix *X,  int sample_index)
{
	double *distances = (double*)malloc(model->n_samples * sizeof(double));
	int *neighbor_indices = (int*)malloc(model->k * sizeof(int));

	//find maximum the class label
	int max_class = 0;
	for(int i=0; i < model->n_samples){
		if(model->labels[i] > max_class){
			max_class = model->labels[i];
		}
	}

	int classes = max_class + 1;
	int *class_count = (int*)calloc(classes, sizeof(int));

	//calculate distances for all samples
	for(int i=0; i < model->n_samples; i++){
		distances[i] = CalculateDistance(X->data[sample_index], model->features->data[i], model->n_features, model->distance_metric);
	}

	//finding the k nearest neighbors
	for(int i=0; i < model->k; i++){
		double min_distance = DBL_MAX;
		int min_index = -1;
		for(int x=0; x < model->n_samples; x++){
			if(distances[i] < min_distance){
				min_distance = distances[x];
				min_index = x;
			}
		}
		neighbor_indices[i] = min_index;
		distances[min_index] = DBL_MAX; //marking this neighbor as used
	}

	//counting the no of labels
	for(int x=0; x < model->k; x++){
		int label = model->labels[neighbor_indices[x]];
		class_count[label]++;

	}

	//determine the majority label vote
	int predicted_label = 0;
	int max_count = 0;
	for(int i=0; i < classes; i++){
		if(class_count[i] > max_count){
			max_count = class_count[i];
			predicted_label = i;
		}
	}
	free(distances);
	free(neighbor_indices);
	free(class_count);

	return predicted_label;
}


int *PredictKNeighborsClassifier(KNeighborsClassifier *model, Matrix *X)
{
	int *predictions = (int*)malloc(X->rows * sizeof(int));
	for(int i=0; i < X->rows; i++){
		predictions[i] = PredictSingle(model, X, i);
	}
	return predictions;
}

void FreeKNeighborsClassifier(KNeighborsClassifier *model)
{
	if(model){
		FreeMatrix(model->features);
	}
	if(model->labels){
		free(labels);
	}
	free(model);
}
