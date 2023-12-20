#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int NUMBERS_SIZE = 50000;
const int CLOCKS_PER_MS = CLOCKS_PER_SEC / 1000;

// Function to generate random integer
int genRandInt(int low, int high) {
    return low + rand() % (high - low + 1);
}

// Function to fill arrays with random integers
void fillArrays(int arr1[], int arr2[], int arr3[]) {
    for (int i = 0; i < NUMBERS_SIZE; ++i) {
        arr1[i] = genRandInt(0, NUMBERS_SIZE);
        arr2[i] = arr1[i];
        arr3[i] = arr1[i];
    }
}

void InsertionSort(int numbers[], int numbersSize) {
    int i, j, key;

    for (i = 1; i < numbersSize; i++) {
        key = numbers[i]; // The element to be inserted into the sorted sequence
        j = i - 1;

        /* Move elements of numbers[0..i-1], that are greater than key, 
           to one position ahead of their current position */
        while (j >= 0 && numbers[j] > key) {
            numbers[j + 1] = numbers[j];
            j = j - 1;
        }
        numbers[j + 1] = key;
    }
}


void Quicksort_midpoint(int numbers[], int i, int k) {
    if (i >= k) {
        return; // Base case: If there are 1 or no elements to sort, partition is already sorted
    }

    // Choose the pivot as the midpoint of the array
    int pivotIndex = (i + k) / 2;
    int pivotValue = numbers[pivotIndex];

    // Swap pivot with last element
    swap(numbers[pivotIndex], numbers[k]);

    // Partitioning index
    int partitionIndex = i;

    for (int j = i; j < k; j++) {
        if (numbers[j] < pivotValue) {
            swap(numbers[partitionIndex], numbers[j]);
            partitionIndex++;
        }
    }

    // Swap pivot back to its correct place
    swap(numbers[partitionIndex], numbers[k]);

    // Recursively sort elements before and after partition
    Quicksort_midpoint(numbers, i, partitionIndex - 1);
    Quicksort_midpoint(numbers, partitionIndex + 1, k);
}

void Quicksort_medianOfThree(int numbers[], int i, int k) {
    if (i >= k) {
        return;
    }

    // Determine the indices of the middle, left, and right elements
    int middle = (i + k) / 2;
    
    // Sort the left, middle, right elements to find the median
    if (numbers[middle] < numbers[i])
        swap(numbers[middle], numbers[i]);
    if (numbers[k] < numbers[i])
        swap(numbers[k], numbers[i]);
    if (numbers[k] < numbers[middle])
        swap(numbers[k], numbers[middle]);

    // Use the middle element as the pivot
    int pivotValue = numbers[middle];
    // Move the pivot to the end (rightmost position)
    swap(numbers[middle], numbers[k]);

    int partitionIndex = i;

    for (int j = i; j < k; j++) {
        if (numbers[j] < pivotValue) {
            swap(numbers[partitionIndex], numbers[j]);
            partitionIndex++;
        }
    }

    // Move the pivot back to its correct place
    swap(numbers[partitionIndex], numbers[k]);

    // Recursively sort elements before and after partition
    Quicksort_medianOfThree(numbers, i, partitionIndex - 1);
    Quicksort_medianOfThree(numbers, partitionIndex + 1, k);
}

void Quicksort_midpoint_wrapper(int arr[], int size) {
    Quicksort_midpoint(arr, 0, size - 1);
}

void Quicksort_medianOfThree_wrapper(int arr[], int size) {
    Quicksort_medianOfThree(arr, 0, size - 1);
}


// Function to measure the time taken by a sort function
void measureTime(void (*sortFunction)(int[], int), int arr[], int size) {
    clock_t start = clock();
    sortFunction(arr, size);
    clock_t end = clock();
    int elapsedTime = (end - start) / CLOCKS_PER_MS;
    cout << "Time: " << elapsedTime << " ms" << endl;
}


int main() {
    srand(time(0)); // Seed for random number generation

    int arr1[NUMBERS_SIZE], arr2[NUMBERS_SIZE], arr3[NUMBERS_SIZE];
    fillArrays(arr1, arr2, arr3);

    cout << "Sorting with Quicksort using midpoint:" << endl;
    measureTime(Quicksort_midpoint_wrapper, arr1, NUMBERS_SIZE);

    cout << "Sorting with Quicksort using median of three:" << endl;
    measureTime(Quicksort_medianOfThree_wrapper, arr2, NUMBERS_SIZE);


    cout << "Sorting with InsertionSort:" << endl;
    measureTime(InsertionSort, arr3, NUMBERS_SIZE);

    return 0;
}
