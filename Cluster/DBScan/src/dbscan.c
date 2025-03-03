#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<float.h>
#include "../include/dbscan.h"


//Helper function to calculate euclidean distance
static double EuclideanDistance(double *a, double *b, int d)
{
	double sum = 0.0;
	for(int i=0; i < d; i++){
		double diff = a[i] - b[i];
		sum += diff * diff;
	}
	return sqrt(sum);
}


//region query: for a given point in the index return an array (which is dynamically allocated) along all the indices
//of points in x that are within its distance eps, the numbers will be stored in the neighbors_count arr
static int *RegionQuery(Matrix *X, int point_index, double eps, int *neighbor_count)
{
	int n_samples = X->rows;
	int d = X->cols;
	int *neighbors = (int*)malloc(n_samples * sizeof(int));
	*neighbor_count = 0;
	for(int i=0; i < n_samples; i++){
		double dist = EuclideanDistance(X->data[point_index], X->data[i], d);
		if(dist <= eps){
			neighbors[(*neighbor_count)++] = i;
		}
	}
	return neighbors;
}


//expands a new cluster from the seed point
//returns 1 if cluster expansion occurs 0 if non
static int ExpandCluster(DBSCAN *model, Matrix *X, int point_index, int *neighbors, int neighbor_count, int cluster_id, double eps, int mini_pts)
{
	model->labels[point_index] = cluster_id;

	int capacity = neighbor_count;
	int *seeds = (int*)malloc(capacity * sizeof(int));
	int seed_count = 0;


	for(int i=0; i < seed_count; i++){
		int curr_point = seeds[i];

		//if point is NOISE asign it to the cluster
		if(model->labels[curr_point] == NOISE){
			result->labels[curr_point] = cluster_id;
		}

		//only take in unclassified points
		if(model->labels[curr_point] != UNCLASSIFIED){
			continue;
		}

		//assign the point to the cluster
		model->labels[curr_point] = cluster_id;
		int curr_n_count = 0;
		int *curr_neighbors = RegionQuery(X, curr_point, eps, &curr_n_count);
		if(curr_n_count >= minPts){
			//append curr neighbors to seeds
			for(int x=0; x < curr_n_count; x++){
				int candidate = curr_neighbors[x];
				if(model->labels[candidate] == UNCLASSIFIED || model->labels[candidate] == NOISE){
					if(seeds_count >= capacity){
						capacity *= 2;
						seeds = (int*)realloc(seeds, capacity * sizeof(int));
					}
					seeds[seeds_count++] = candidate;
					model->labels[candidate] = cluster_id;
				}
			}
		}
		//tom
		free(curr_neighbors);
	}

	free(seeds);
	return 1;

}


DBSCAN *CreateDBSCAN(Matrix *X, double eps, int min_pts)
{
	int n_samples = X->rows;
	DBSCAN *model = (DBSACN*)malloc(sizeof(DBSCAN));
	model->n_samples = n_samples;
	model->labels = (int*)malloc(sizeof(int));
	for(int i=0; i < n_samples; i++){
		model->labels[i] = UNCLASSIFIED;
	}

	model->n_clusters = 0;

	for(int i=0; i < n_samples; i++){
		if(model->labels[i] != UNCLASSIFIED){
			continue;
		}
		int neighbor_count = 0;
		int *neighbors = RegionQuery(X, i, eps, &neighbor_count);
		if(neighbor_count < min_pts){
			model->labels[i] = NOISE;
			free(neighbors);
		} else {
			model->n_clusters++;
			ExpandCluster(model, X, i, neighbors, neighbor_count, model->n_clusters, eps, min_pts);
			free(neighbors);
		}
	}
	return model;
}

void FreeDBSCAN(DBSCAN *model)
{
	if(model){
		if(model->labels){
			free(labels);
		}
		free(model);
	}

}
