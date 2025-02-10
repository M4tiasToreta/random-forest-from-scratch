#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read_csv.h"

typedef struct {
    int feature_index;         // feature to split on
    double threshold;          // threshold value for the split
    int prediction;            // if leaf node, prediction (0 or 1)
    struct TreeNode *left;     // left subtree pointer
    struct TreeNode *right;    // right subtree pointer
    int is_leaf;
} TreeNode;

typedef struct {
    TreeNode *nodes;
    int n_trees;
} RandomForest;

double gini_index(DataPoint *samples, int n_samples) {
    if (n_samples == 0)
        return 0.0;

    int count0 = 0, count1 = 0;
    for (int i = 0; i < n_samples; i++) {
        if (samples[i].label == 0)
            count0++;
        else
            count1++;
    }
    double p0 = (double)count0 / n_samples;
    double p1 = (double)count1 / n_samples;
    return 1.0 - (p0 * p0 + p1 * p1);
}

void find_best_split(DataPoint *samples, int n_samples, int n_features,
                     int *best_feature, double *best_threshold, double *best_gini) {
    *best_gini = 1.0;
    *best_feature = -1;
    *best_threshold = 0.0;

    int feature = random_int(0, n_features - 1);

    double min_val = samples[0].features[feature];
    double max_val = samples[0].features[feature];
    for (int i = 0; i < n_samples; i++) {
        if (samples[i].features[feature] < min_val)
            min_val = samples[i].features[feature];
        if (samples[i].features[feature] > max_val)
            max_val = samples[i].features[feature];
    }

    int num_thresholds = 10;
    for (int i = 0; i < num_thresholds; i++) {
        double threshold = min_val + i * (max_val - min_val) / (num_thresholds - 1);
        DataPoint *left = NULL, *right = NULL;
        int n_left = 0, n_right = 0;
        split_dataset(samples, n_samples, feature, threshold, &left, &n_left, &right, &n_right);
        double gini_left = gini_index(left, n_left);
        double gini_right = gini_index(right, n_right);
        double weighted_gini = (n_left * gini_left + n_right * gini_right) / n_samples;
        if (weighted_gini < *best_gini) {
            *best_gini = weighted_gini;
            *best_feature = feature;
            *best_threshold = threshold;
        }
        free(left);
        free(right);
    }
}

TreeNode* build_tree(DataPoint train[], int depth, int max_depth, int n_samples){
    TreeNode *node;
    DataPoint *left = NULL, *right = NULL;
    node->left = node->right = NULL;
    node->is_leaf = 0;
    int best_feature;
    double best_threshold;
    double best_gini;
    find_best_split(train, n_samples, n_features, &best_feature, &best_threshold, &best_gini);

    // node->left = build_tree(left_data, depth);
    // node->right = build_tree(right_data, depth);
    return node;
}

int build_forest(RandomForest random_forest, DataPoint train[], int max_depth, int n_samples){
    random_forest.nodes = build_tree(train, 0, max_depth, n_samples);
}

int train_model(DataPoint train[], int n_samples, int n_features, int max_depth=10, int n_trees=5) {
    RandomForest random_forest;
    random_forest.n_trees = n_trees;
    build_forest(random_forest, train, max_depth, n_samples, n_features);
    return 0;
}

int* predict(DataPoint test[]) {
    int a[MAX_ROWS];
    return a; 
}