#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROWS 1000
#define MAX_COLS 30
#define TRAIN_SPLIT 0.8

// Estrutura para armazenar os dados
typedef struct {
    float features[MAX_COLS];
    int label;
} DataPoint;

// Função para ler um CSV
int read_csv(const char *filename, DataPoint data[], int *num_samples, int num_features) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    
    char line[2048];
    int row = 0;
    while (fgets(line, sizeof(line), file) && row < MAX_ROWS) {
        char *token = strtok(line, ",");
        int col = 0;

        while (token && col-1 < num_features) {
            if (col != 0) {
                data[row].features[col-1] = atof(token);
            } else {
                data[row].label = atoi(token);
            }
            token = strtok(NULL, ",");
            col++;
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
