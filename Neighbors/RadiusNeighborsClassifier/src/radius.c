#include<stdlib.h>
#include<math.h>
#include "../include/radius.h"


static double EuclideanDistance(double *p1, double *p2, int dim)
{
	double dist = 0.0;
	for(int i=0; i < dim; i++){
		double diff = p1[i] - p2[i];
		dist += diff * diff;
	}
	return sqrt(dist);
}

RadiusNeighborsClassifier *CreateRadiusClassifier(double radius)
{
	RadiusNeighborsClassifier *model = (RadiusNeighborsClassifier*)malloc(sizeof(RadiusNeighborsClassifier));
	model->features = NULL; //set during training
	model->labels = NULL; //set during training
	model->radius = radius;
	return model;
}


void TrainRadiusClassifier(RadiusNeighborsClassifier *model, Matrix *X, Matrix *Y)
{
	model->features = X;
	model->labels = Y;
}

int PredictSingleRadius(RadiusNeighborsClassifier *model, Matrix *X, int sample_index)
{
	int n_class = 0;
	for(int i=0; i < model->labels->rows; i++){
		if((int)model->labels->data[i][0] + 1 > n_class){
			n_class = (int)model->labels->data[i][0] + 1;
		}
	}

	int *class_counts = (int*)calloc(n_class, sizeof(int));

	for(int i=0; i < model->features->rows; i++){
		double dist = EuclideanDistance(model->features->data[i], X->data[sample_index], model->features->cols);
		if(dist <= model->radius){
			int label = (int)model->labels->data[i][0];
			class_counts[label]++;
		}
	}

	int max_class = 0;
	for(int i = 1; i < n_class; i++){
		if(class_counts[i] > class_counts[max_class]){
			max_class = i;
		}
	}

	free(class_counts);
	return max_class;
}

Matrix *PredictRadiusClassifier(RadiusNeighborsClassifier *model, Matrix *X)
{
	Matrix *predictions = CreateMatrix(X->rows, 1);
	for(int i=0; i < X->rows; i++){
		predictions->data[i][0] = (double)PredictSingleRadius(model, X, i);
	}
	return predictions;
{

void FreeRadiusClassifier(RadiusNeighborsClassifier *model)
{
	if(model){
		FreeMatrix(model->features);
	}
	if(model->labels){
		free(labels);
	}
	free(model);
}
