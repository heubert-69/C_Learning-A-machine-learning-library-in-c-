#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "../include/kneighbors_r.h"
#include "../include/matrix.h"

KNeighborsRegressor *CreateNeighborsRegressor(int n_neighbors)
{
	KNeighborsRegressor *model = (KNeighborsRegressor*)malloc(sizeof(KNeighborsRegressor));
	model->features = NULL;
	model->targets = NULL;
	model->n_neighbors = n_neighbors;
	return model;
}

//for calculating euclidean distance or the manhattan distance of X and Y labels
double CalculateDistance(Matrix *features, int ind_1, int ind_2, int n_features, char metric)
{
	double distance = 0.0;
	for(int i=0; i < features->cols; i++){
		diff = get_value(features, ind_1, i) - get_value(features, ind_2, i);
		if(metric == 'e'){
			distance += diff * diff;
		}
		if(metric == 'm'){
			distance += fabs(diff);
		}
	}

	return (metric == 'e') ? sqrt(distance) : distance;
}


void TrainNeighborsRegressor(KNeighborsRegressor *model, Matrix *X, Matrix *Y)
{
	model->features = X;
	model->targets = Y;
}

double PredictSingleNeighborsRegressor(KNeighborsRegressor *model, Matrix *X, char metric, int sample_index)
{
	int n_samples = model->features->rows;
	double *distances = (double*)malloc(n_samples * sizeof(double));
	int *indices = (int*)malloc(n_samples * sizeof(int));

	//calculate distances for all samples
	for(int i=0; i < n_samples; i++){
		distances[i] = CalculateDistance(model->features, sample_index, i, model->n_features, metric);
		indices[i] = i;
	}

	//sorting the distances or performing a linear sort rather :c
	for(int i=0; i < n_samples - 1; i++){
		for(int x=i + 1; x < n_samples; x++){
			if(distances[i] > distances[x]){
				double temp = distances[i];
				distances[i] = distances[x];
				distances[x] = temp;

				int idx_temp = indices[i];
				indices[i] = indices[x];
                                indices[x] = idx_temp;
			}
		}
	}

	double prediction = 0.0;
	for(int i=0; i < model->n_neighbors; i++){
		prediction += get_value(targets, indices[i], 0);
	}

	prediction /= model->n_neighbors;

	free(distances);
	free(indices);

	return prediction;
}

Matrix *PredictNeighborsRegressor(KNeighborsRegressor *model, Matrix *X, char metric)
{
	Matrix *predictions = CreateMatrix(X->rows, 1); //need a vector to release predictions
	for(int i=0; i < X->rows; i++){
		set_value(predictions, i, 0, PredictSingleNeighborsRegressor(model, X, i, metric));
	}
	return predictions;
}



void FreeKNeighborsRegressor(KNeighborsRegressor *model)
{
	if(model){
		FreeMatrix(model->features);
	}
	if(model->targets){
		FreeMatrix(model->targets);
	}
	free(model);
}
