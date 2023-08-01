#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int arr[], int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *left_arr = (int *)malloc(n1 * sizeof(int));
    int *right_arr = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        left_arr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        right_arr[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (left_arr[i] <= right_arr[j])
        {
            arr[k] = left_arr[i];
            i++;
        }
        else
        {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = right_arr[j];
        j++;
        k++;
    }

    free(left_arr);
    free(right_arr);
}

void sequential_merge_sort(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        sequential_merge_sort(arr, left, mid);
        sequential_merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void parallel_merge_sort(int arr[], int left, int right, int threads)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        #pragma omp parallel num_threads(threads)
        {
            #pragma omp sections
            {
                #pragma omp section
                parallel_merge_sort(arr, left, mid, threads / 2);

                #pragma omp section
                parallel_merge_sort(arr, mid + 1, right, threads / 2);
            }
        }

        merge(arr, left, mid, right);
    }
}

void print_array(int arr[], int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void read_file(const char *filename, int arr[], int size)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        exit(1);
    }

    for (int i = 0; i < size; i++)
    {
        if (fscanf(file, "%d", &arr[i]) != 1)
        {
            printf("Error reading from the file.\n");
            fclose(file);
            exit(1);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <0 or 1>\n", argv[0]);
        return 1;
    }

    int parallel = atoi(argv[1]);
    if (parallel != 0 && parallel != 1)
    {
        printf("Invalid argument. Please provide 0 for sequential or 1 for parallel.\n");
        return 1;
    }

    const char *filename = "input.txt";
    int num_threads = omp_get_max_threads();

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }
    int num_elements = 0;
    int num;
    while (fscanf(file, "%d", &num) == 1)
        num_elements++;
    fclose(file);

    int *arr = (int *)malloc(num_elements * sizeof(int));
    if (arr == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    read_file(filename, arr, num_elements);

    // printf("Original array: \n");
    // print_array(arr, num_elements);

    if (parallel == 0)
        sequential_merge_sort(arr, 0, num_elements - 1);
    else if (parallel == 1)
        parallel_merge_sort(arr, 0, num_elements - 1, num_threads);

    // printf("\nSorted array: \n");
    // print_array(arr, num_elements);

    free(arr);

    return 0;
}