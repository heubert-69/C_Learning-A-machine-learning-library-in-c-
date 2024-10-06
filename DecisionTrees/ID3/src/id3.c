#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"
#include "../include/id3.h"



double Entrophy(int *labels, int samples)
{
	int count_0 = 0, count_1 = 0;
	for(int i=0; i < samples; i++){
		if(labels[i] == 0){count_0++;}
		else {count_1++;}
	}

	double p0 = (double)count_0/samples;
	double p1 = (double)count_1/samples;

	if(count_0 == 0 || count_1 == 0){return 0.0;}
	return -(p0 * log2(p0) + p1 * log2(p1));
}

double InformationGain(Matrix *features, int *labels, int feature_index, int samples)
{
	int left_count = 0, right_count = 0;
	int *left_labels = (*int)malloc(samples * sizeof(int));
	int *right_labels = (*int)malloc(samples * sizeof(int));


	for(int i=0; i < samples; i++){
		double feature_value = get_value(features, i, feature_index);
		if(feature_value == 0){
			left_labels[left_count++] = labels[i];
		} else {
			right_labels[right_count++] = labels[i];
		}
	}

	double ent_left =  Entrophy(left_labels, left_count);
	double ent_right = Entrophy(right_labels, right_count);
	double total_entrophy = Entrophy(labels, samples);


	double gain = total_entrophy - ((left_entrophy / (double)samples) * ent_left + (right_count / (double)samples) * ent_right);
	free(left_labels);
	free(right_labels);

	return gain;
}



int BestFeatureToSplit(Matrix *features, int *labels, int samples, int features)
{
	int best_feature = 0;
	double best_gain = -1;

	for(int i=0; i < samples; i++){
		double gain = InformationGain(features, labels, i, samples);
		if(gain > best_gain){
			best_gain = gain;
			best_feature =  i;
		}
	}
	return best_feature;
}



//building the tree
ID3* BuildID3(Matrix *features, int *labels, int samples, int n_features)
{
	int count_0 = 0, count_1 = 0; 
	for(int i=0; i < samples; i++){
		if(labels[i] == 0){count_0++;}
		else{count_1++;}
	}

	//all samples belong to the same classified class, create a leaf node
	if(count_0 == samples || count_1 == samples){
		ID3 *node = (*ID3)malloc(sizeof(ID3));
		node->feature = -1;
		node->label = (count_0 == samples) ? 0 : 1;
		node->left = NULL;
		node->right = NULL;
		return node;
	}


	//if not generate the best feature to split
	int best_feature = BestFeatureToSplit(features, labels, samples, n_features)

	ID3 *node = (*ID3)malloc(sizeof(ID3));
	node->feature = best_feature;

	//split the data into left and right based on the given dataset
	int *left_indices = (*int)malloc(samples * sizeof(int));
	int *right_indices = (*int)malloc(samples * sizeof(int));

	int count_left = 0, count_right = 0;

	for(int i=0; i < samples; i++){
		double feature_values = get_value(features, i, best_feature);
		if(feature_value == 0){
			left_indices[count_left++] = i;
		} else {
			right_indices[count_right++] = i;
		}
	}

	Matrix *left_features = CreateMatrix(count_left, n_features);
	Matrix *right_features = CreateMatrix(count_right, n_features);

	int *left_labels = (*int)malloc(count_left * sizeof(int));
	int *right_labels = (*int)malloc(count_right * sizeof(int));

	for(int i=0; i < count_left; i++){
		for(int x=0; x < n_features; x++){
			set_value(left_features, i, x, get_value(features, left_indices[i], x));
		}
		left_labels[i] = labels[left_indices[i]];
	}

	for(int i=0; i < count_right; i++){
                for(int x=0; x < n_features; x++){
                        set_value(right_features, i, x, get_value(features, right_indices[i], x));
                }
                right_labels[i] = labels[right_indices[i]];
        }

	//using recursion to build left and right subtrees
	node->left = BuildID3(left_features, left_labels, count_left, n_features);
	node->right = BuildID3(right_features, right_labels, count_right, n_features);

	free(left_labels);
	free(right_labels);
	free(left_indices);
	free(right_indices);
	FreeMatrix(left_features);
	FreeMatrix(right_features);

	return node;
}


int ClassifyID3(ID3 *node, Matrix *features, int sample_index)
{
	if(node->feature == -1){
		return node->label;
	}

	double feature_value = get_value(features, sample_index, node->feature);
	if(feature_value == 0){
		return ClassifyID3(node->left, features, sample_index);
	} else {
		return ClassifyID3(node->right, features, sample_index);
	}
}



void FreeID3(ID3 *node)
{
	if(node == NULL){return;}
	FreeID3(node->left);
	FreeID3(node->right);
	free(node);
}
