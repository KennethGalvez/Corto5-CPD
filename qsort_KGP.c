#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define THRESHOLD 100

void insertion_sort(int *data, int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key)
        {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

void par_qsort(int *data, int lo, int hi)
{
    if (lo > hi)
        return;

    if (hi - lo < THRESHOLD)
    {
        insertion_sort(data + lo, hi - lo + 1);
        return;
    }

    int l = lo;
    int h = hi;
    int p = data[(hi + lo) / 2];

    while (l <= h)
    {
        while ((data[l] - p) < 0)
            l++;
        while ((data[h] - p) > 0)
            h--;
        if (l <= h)
        {
            int tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            l++;
            h--;
        }
    }

    #pragma omp task
    par_qsort(data, lo, h);

    par_qsort(data, l, hi); // Continue with the current thread
}

int main()
{
    int N;

    printf("Cantidad de numeros a generar: ");
    scanf("%d", &N);

    int *x = (int *)malloc(N * sizeof(int));
    if (!x)
    {
        perror("Error memory para x");
        exit(1);
    }

    srand(time(NULL));

    for (int j = 0; j < N; j++)
    {
        x[j] = rand() % 100000;
    }

    FILE *csv_file = fopen("input.csv", "w");
    if (!csv_file)
    {
        perror("Error creando CSV");
        exit(1);
    }

    for (int j = 0; j < N; j++)
    {
        fprintf(csv_file, "%d\n", x[j]);
    }

    fclose(csv_file);

    double start_time = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single nowait
        par_qsort(x, 0, N - 1);
    }

    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;

    csv_file = fopen("output.csv", "w");
    if (!csv_file)
    {
        perror("Error creando CSV");
        exit(1);
    }

    for (int j = 0; j < N; j++)
    {
        fprintf(csv_file, "%d\n", x[j]);
    }

    fclose(csv_file);

    for (int i = 1; i < N; i++)
    {
        if (x[i] < x[i - 1])
        {
            printf("Sorting failed.\n");
            break;
        }
    }

    printf("Sortiado y Escritura Completa.\n");
    printf("Tiempo: %lf segundos\n", execution_time);

    free(x);

    return 0;
}

