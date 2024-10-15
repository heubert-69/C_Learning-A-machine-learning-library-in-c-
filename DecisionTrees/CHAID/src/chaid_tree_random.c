#include "../include/chaid_tree_random.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

// Helper function to calculate the most common label in a subset
int get_most_common_label(int *labels, int num_samples) {
    int count[2] = {0, 0};
    for (int i = 0; i < num_samples; i++) {
        count[labels[i]]++;
    }
    return (count[0] > count[1]) ? 0 : 1;
}

// Helper function to generate a random threshold for splitting
double get_random_threshold(Matrix *features, int feature_index, int num_samples) {
    double min_value = GetMatrixValue(features, 0, feature_index);
    double max_value = GetMatrixValue(features, 0, feature_index);

    for (int i = 1; i < num_samples; i++) {
        double value = GetMatrixValue(features, i, feature_index);
        if (value < min_value) min_value = value;
        if (value > max_value) max_value = value;
    }

    // Generate a random threshold between the min and max values
    return min_value + ((double)rand() / RAND_MAX) * (max_value - min_value);
}

// Helper function to split data based on a feature and threshold
void split_data(Matrix *features, int *labels, int num_samples, int feature_index, double threshold, int *left_indices, int *right_indices, int *left_size, int *right_size) {
    *left_size = 0;
    *right_size = 0;
    for (int i = 0; i < num_samples; i++) {
        if (GetMatrixValue(features, i, feature_index) <= threshold) {
            left_indices[(*left_size)++] = i;
        } else {
            right_indices[(*right_size)++] = i;
        }
    }
}

// Recursive function to build the CHAID tree with random splits
TreeNode *build_chaid_tree_with_random_splits(Matrix *features, int *labels, int num_samples, int num_features) {
    // Base case: If all labels are the same, return a leaf node
    int first_label = labels[0];
    int all_same = 1;
    for (int i = 1; i < num_samples; i++) {
        if (labels[i] != first_label) {
            all_same = 0;
            break;
        }
    }
    if (all_same) {
        TreeNode *leaf = (TreeNode *)malloc(sizeof(TreeNode));
        leaf->is_leaf = 1;
        leaf->class_label = first_label;
        leaf->left = leaf->right = NULL;
        return leaf;
    }

    // Otherwise, create an internal node with a random split
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->is_leaf = 0;
    node->feature_index = rand() % num_features;   // Random feature selection
    node->threshold = get_random_threshold(features, node->feature_index, num_samples);

    // Split data into left and right subsets
    int *left_indices = (int *)malloc(num_samples * sizeof(int));
    int *right_indices = (int *)malloc(num_samples * sizeof(int));
    int left_size, right_size;
    split_data(features, labels, num_samples, node->feature_index, node->threshold, left_indices, right_indices
