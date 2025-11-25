#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DATA 1000     
#define FEATURES 8      
#define K 5               

typedef struct {
    double x[FEATURES];
    int label;
} Sample;

double distance(double a[], double b[]) {
    double sum = 0.0;
    for (int i = 0; i < FEATURES; i++) {
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(sum);
}

int knn_predict(Sample train[], int train_size, Sample test) {
    double dist[MAX_DATA];
    int index[MAX_DATA];

    for (int i = 0; i < train_size; i++) {
        dist[i] = distance(train[i].x, test.x);
        index[i] = i;
    }

    for (int i = 0; i < train_size; i++) {
        for (int j = i+1; j < train_size; j++) {
            if (dist[i] > dist[j]) {
                double tmpd = dist[i];
                dist[i] = dist[j];
                dist[j] = tmpd;

                int tmpi = index[i];
                index[i] = index[j];
                index[j] = tmpi;
            }
        }
    }

    int count0 = 0, count1 = 0;
    for (int i = 0; i < K; i++) {
        if (train[index[i]].label == 1)
            count1++;
        else
            count0++;
    }

    return (count1 > count0) ? 1 : 0;
}

int main() {
    Sample train[MAX_DATA], test[MAX_DATA];
    int train_size = 0, test_size = 0;

    FILE *fp = fopen("diabetes_subset.csv", "r");
    if (!fp) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    while (!feof(fp)) {
        for (int i = 0; i < FEATURES; i++)
            fscanf(fp, "%lf,", &train[train_size].x[i]);
        fscanf(fp, "%d\n", &train[train_size].label);
        train_size++;
        if (train_size >= MAX_DATA) break;
    }
    fclose(fp);

    int correct = 0;
    for (int i = 0; i < train_size; i++) {
        int pred = knn_predict(train, train_size, train[i]);
        if (pred == train[i].label) correct++;
    }

    double accuracy = (double)correct / train_size * 100.0;
    printf("Accuracy: %.2f%%\n", accuracy);

    return 0;
}
