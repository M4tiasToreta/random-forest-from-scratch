#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROWS 1000
#define MAX_COLS 31
#define TRAIN_SPLIT 0.8

// Estrutura para armazenar os dados
typedef struct {
    float features[MAX_COLS-1];
    int label;
} DataPoint;

// Função para ler um CSV
int read_csv(const char *filename, DataPoint data[], int *num_samples, int num_columns) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    
    char line[2048];
    int row = 0;
    while (fgets(line, sizeof(line), file) && row < MAX_ROWS) {
        char *token = strtok(line, ",");
        int feat = 0;

        while (token && feat-1 < num_columns) {
            if (feat != 0) {
                data[row].features[feat-1] = atof(token);
            } else {
                // not a feature, a label
                data[row].label = atoi(token);
            }
            token = strtok(NULL, ",");
            feat++;
        }
        row++;
    }
    
    fclose(file);
    *num_samples = row;
    return 0;
}


void split_data(DataPoint data[], int num_samples, DataPoint train[], DataPoint test[], int *train_size, int *test_size) {
    int train_count = (int)(num_samples * TRAIN_SPLIT);
    int test_count = num_samples - train_count;
    
    for (int i = 0; i < train_count; i++) {
        train[i] = data[i];
    }
    for (int i = 0; i < test_count; i++) {
        test[i] = data[train_count + i];
    }
    
    *train_size = train_count;
    *test_size = test_count;
}
