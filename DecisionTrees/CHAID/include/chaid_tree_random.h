#ifndef CHAID_TREE_RANDOM_H
#define CHAID_TREE_RANDOM_H

#include "matrix.h"

// Tree node structure
typedef struct TreeNode {
    int is_leaf;             // 1 if it's a leaf node, 0 otherwise
    int feature_index;       // Index of the feature used for the split
    double threshold;        // Threshold for the split
    int class_label;         // Class label if it's a leaf
    struct TreeNode *left;   // Pointer to left subtree
    struct TreeNode *right;  // Pointer to right subtree
} TreeNode;

// Function prototypes
TreeNode *build_chaid_tree_with_random_splits(Matrix *features, int *labels, int num_samples, int num_features);
int classify_chaid(TreeNode *node, Matrix *features, int sample_index);
void free_chaid_tree(TreeNode *node);

#endif // CHAID_TREE_RANDOM_H
