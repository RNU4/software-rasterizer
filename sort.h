#ifndef _SORT_H
#define _SORT_H
#include "init.h"
#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_thread.h"
#include "draw.h"
#define MAX_THREADS 8
#define MULTI_THREAD 0


#if MULTI_THREAD == 1

typedef struct {
    shape_triangle_t* arr;
    int low;
    int high;
} ThreadData;

int partition(shape_triangle_t* arr, int low, int high) {
    float pivot = (arr[high].p[0].z + arr[high].p[1].z + arr[high].p[2].z) / 3.0f;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if ((arr[j].p[0].z + arr[j].p[1].z + arr[j].p[2].z) / 3.0f > pivot) {
            i++;
            shape_triangle_t temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    shape_triangle_t temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return (i + 1);
}

void quicksort(shape_triangle_t* arr, int low, int high);

int quicksort_thread(void* data) {
    ThreadData* threadData = (ThreadData*)data;
    shape_triangle_t* arr = threadData->arr;
    int low = threadData->low;
    int high = threadData->high;

    quicksort(arr, low, high);

    free(threadData);

    return 0;
}

void quicksort(shape_triangle_t* arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);

        if (SDL_GetThreadID(NULL) != SDL_ThreadID()) {
            SDL_Thread* thread;
            ThreadData* threadData;

            thread = SDL_CreateThread(quicksort_thread, "QuicksortThread", (void*)threadData);

            if (thread == NULL) {
                printf("SDL_CreateThread failed: %s\n", SDL_GetError());
                return;
            }

            threadData = (ThreadData*)malloc(sizeof(ThreadData));
            threadData->arr = arr;
            threadData->low = low;
            threadData->high = pivotIndex - 1;

            quicksort(arr, pivotIndex + 1, high);

            SDL_WaitThread(thread, NULL);
        } else {
            quicksort(arr, low, pivotIndex - 1);
            quicksort(arr, pivotIndex + 1, high);
        }
    }
}
#else
int partition(shape_triangle_t *arr, int low, int high)
{
    float pivot = (arr[high].p[0].z + arr[high].p[1].z + arr[high].p[2].z) / 3.0f;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if ((arr[j].p[0].z + arr[j].p[1].z + arr[j].p[2].z) / 3.0f > pivot)
        {
            i++;
            // Swap arr[i] and arr[j]
            shape_triangle_t temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // Swap arr[i+1] and arr[high] (pivot)
    shape_triangle_t temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return (i + 1);
}

void quicksort(shape_triangle_t *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

#endif
#endif