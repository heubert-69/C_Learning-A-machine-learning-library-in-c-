#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"
#include "../include/chaid.h"


double ChiSquaredTest(int *observed, int *expected, int size)
{
	double chi_squared = 0.0;
        for(int i=0; i < size; i++){
                if(expected[i] > 0) {chi_squared += (double)(observed[i] - expected[i]) * (observed[i] - expected[i]) / expected[i];}
                else{count_1++;}
        }
        return chi_squared;
}

//using chi-squared test we will find the best feature to split
int BestFeatureToSplit(Matrix *features, int *labels, int samples, int n_features)
{
	int best_feature = -1;
	double best_chi_squared = -1.0;

	//gathering observations
	for(int i=0; i < n_features; i++){
		int observed[2][2] = {{0, 0}, {0, 0}};
		for(int x=0; x < samples; x++){
			int feature_value = (int)get_value(features, x, i);
			int label_value = labels[x];
			observed[feature_value][feature_value]++;
		}
		//calculate expected counts
		int total = samples;
		int row_total[2] = {observed[0][0] + observed[0][1], observed[1][0] + observed[1][1]};
		int col_total[2] = {observed[0][0] + observed[1][0], observed[0][1] + observed[1][1]};

		int expected[2][2];
		for(int r=0; r < 2; r++){
			for(int c=0; c < 2; c++){
				expected[r][c] = (row_total[r] * col_total[c]) / total;
			}
		}

		//calculate chi-squared value
		double chi_squared = 0.0;
		for(int r=0; r < 2; r++){
			for(int c=0; c < 2; c++){
				chi_squared += ChiSquaredTest(&observed[r][c], &expected[r][c], 1);
			}

		}

		if(chi_squared > best_chi_squared){
			best_chi_squared = chi_squared;
			best_feature = -1;
		}

	}
	return best_feature;
}


CHAID* CreateCHAID(Matrix *features, int *labels, int samples, int n_features)
{
	//if labels are the same create a leaf node
	int count_0 = 0, count_1 = 0;
	for(int i=0; i < samples; i++){
		if(labels[i] == 0){count_0++;}
		else{count_1++;}
	}

	//if count is the same the as the samples
	if(count_0 == samples || count_1 == samples){
		CHAID* node = (*CHAID)malloc(sizeof(CHAID));
		node->feature = -1;
		node->label = (count_0 == samples) ? 0 : 1;
		node->left = NULL:
		node->right = NULL;
		return node;
	}

	int best_feature = BestFeatureToSplit(features, labels, samples, n_features);
	CHAID *node = (*CHAID)malloc(sizeof(CHAID));
	node->feature = best_feature;

	//splitting the dataset according to the best feature
	int *left_indices = (*int)malloc(samples * sizeof(int));
	int *right_indices = (*int)malloc(samples * sizeof(int));
	int count_left = 0, count_right = 0;

	for(int i=0; i < samples; i++){
		double feature_value = get_value(features, i, best_feature);
		if(feature_value == 0){
			left_indices[count_left++] = labels[i];
		} else {
			right_indices[count_right++] = labels[i];
		}

	}

	Matrix *left_features = CreateMatrix(count_left, n_features);
	Matrix *right_features = CreateMatrix(count_right, n_features);

	for(int i=0; i < count_left; i++){
		for(int x=0; x < n_features; x++){
			set_value(left_features, i, x, get_value(features, i, x));
		}
	}


	 for(int i=0; i < count_right; i++){
                for(int x=0; x < n_features; x++){
                        set_value(right_features, i, x, get_value(features, i, x));
                }
        }


	//recursively assign value in left and right nodes
	node->left = BuildCHAID(left_features, left_labels, count_left, n_features)
	node->right = BuildCHAID(right_features, right_labels, count_right, n_features)

	free(left_labels);
	free(right_labels);
	FreeMatrix(left_features);
	FreeMatrix(right_features);

	return node;
}

int ClassifyCHAID(CHAID *node, Matrix *features, int sample_index)
{
	if(node->feature == -1){
		return node->label;
	}

	int feature_value = (int)get_value(features, sample_index, node->feature);
	if(feature_value == 0){
		return ClassifyCHAID(node->left, features, sample_index);
	} else {return ClassifyCHAID(node->right, features, sample_index);}
}




void FreeCHAID(CHAID* node)
{
	if(node == NULL){return;}
	FreeCHAID(node->right);
	FreeCHAID(node->left);
	free(node);
}
