#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<float.h>
#include "../include/matrix.h"
#include "../include/optics.h"

#define UNDEFINED -1

static double EuclideanDistance(double *a, double *b, int d)
{
	double res = 0.0;
	for(int i=0; i < d; i++){
		double diff = a[i] - b[i];
		res += diff * diff;
	}
	return sqrt(res);
}

static int *RegionQuery(Matrix *X, int point_index, double eps, int *neighbor_count)
{
	int n_samples = X->rows;
	int *neighbors = (int*)malloc(sizeof(int));
	int neighbor = 0;

	for(int i=0; i < n_samples; i++){
		if(i != point_index){
			double dist = EuclideanDistance(X->data[point_index], X->data[i], X->cols);
			if(dist <= eps){
                        	neighbors[(*neighbor_count)++] = i;
                	}
		}
	}
	return neighbors;
}

static void UpdateNeighbors(Matrix *X, int current, int *neighbors, int neighbor_count, double eps, int min_samples. double *reachability, int *processed, int *seeds)
{
	double core_dist = (neighbor_count >= min_samples) ? eps : DBL_MAX;

	for(int i=0; i < neighbor_count; i++){
		int neighbor = neighbor[i];
		if(!processed[neighbor]){
			double new_reachability = fmax(core_dist, EuclideanDistance(X->data[current], X-data[neighbor], X->cols));

			if(reachability[neighbor] == DBL_MAX){
				seeds[neighbor] = 1;

				reachability[neighbor] = new_reachability;
			} else if(new_reachability < reachability[neighbor]){
				reachability[neighbor] = new_reachability;
			}
		}

	}

}


OPTICS *CreateOPTICS(Matrix *X, double eps, int min_samples)
{
	int n_samples = X->rows;

	OPTICS *model = (OPTICS*)malloc(sizeof(OPTICS));
	model->labels = (int*)malloc(sizeof(int));
	model->reachability = (double*)malloc(sizeof(double));
	model->n_samples = n_samples;


	int *processed = (int*)malloc(sizeof(int));
	int *seeds = (int*)malloc(sizeof(int));
	for(int i=0; i < n_samples; i++){
		model->reachability[i] = DBL_MAX;
		model->labels[i] = UNDEFINED;
	}

	int cluster_id = 0;

	for(int i=0; i < n_samples; i++){
		if(processed[i]) continue;

		int neighbor_count;

		int *neighbors = RegionQuery(X, i, eps, &neighbor_count);

		processed[i] = 1;

		if(neighbor_count >= min_samples){
			cluster_id++;
			model->labels = cluster_id;

			UpdateNeighbors(X, i, neighbors, neighbor_count, eps, min_samples, model->reachability, processed, seeds);

			//Process the seeds
			while(1){
				int next_point = -1;

				double min_reachability = DBL_MAX;

				for(int x=0; x < n_samples; x++){
					if(seeds[x] && model->reachability[x] < min_reachability){
						min_reachability = model->reachability[x];
						next_point = x;
					}
				}

				if(next_point == -1){
					break;
				}

				seeds[next_point] = 0;
				processed[next_point] = 1;
			}
				model->labels[next_point] = cluster_id;
				int new_neighbor_count;
				int *new_neighbors = RegionQuery(X, next_point, eps, &new_neighbor_count);

				if(new_neighbor_count >= min_samples){
					UpdateNeighbors(X, next_point, new_neighbors, new_neighbor_count, eps, min_samples, result->reachability, processed, seeds);
				}

				free(new_neighbors);
		}
	}

	free(processed);
	free(seeds);

	return result;
}

void FreeOPTICS(OPTICS *model)
{
	if(model){
		free(model->labels);
		free(model->reachability);
		free(model);
	}
}
