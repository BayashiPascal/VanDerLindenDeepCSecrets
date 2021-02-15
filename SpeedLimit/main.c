#include "stdio.h"
#include "stdlib.h"
#include "time.h"

// Declare the number of numbers in the file
#define NB 10000

// Declare the inverse of the number of numbers in the file
#define INV_NB 0.0001

// Declare an array to memorise the numbers in the file
int arr[NB];

// Structure to hold the array (for measurement purpose only)
struct structArr {
  int* arr;
};

// Declare the name of the file containing the data
#define FILENAME "nb.dat" 

int funA(void) {

  // Create the structure to hold the array
  struct structArr sArr = { .arr = arr };

  // Open the file containing the number
  FILE* fp =
    fopen(
      FILENAME,
      "rb");

  // Loop on the numbers
  for (
    int i = NB;
    i--;) {

    // Read the number
    int ret =
      fread(
        sArr.arr + i,
        1,
        sizeof(int),
        fp);

  }

  // Close the file containing the number
  fclose(fp);

  // Declare a variable to memorise the sum of the numbers
  long sum = 0;

  // Loop on the numbers
  for (
    int i = NB;
    i--;) {

    // Calculate the sum
    sum += sArr.arr[i];

  }

  // Calculate the average
  double avg = ((double)sum) / ((double)NB);

  // Return the result
  return avg;

}

int funB(void) {

  // Create the structure to hold the array
  struct structArr sArr = { .arr = arr };

  // Open the file containing the number
  FILE* fp =
    fopen(
      FILENAME,
      "rb");

  // Read the whole file at once
  int ret =
    fread(
      sArr.arr,
      NB,
      sizeof(int),
      fp);

  // Close the file containing the number
  fclose(fp);

  // Declare a variable to memorise the sum of the numbers
  long sum = 0;

  // Loop on the number
  for (
    int i = NB;
    i--;) {

    // Calculate the sum
    sum += sArr.arr[i];

  }

  // Calculate the average
  double avg = ((double)sum) / ((double)NB);

  // Return the result
  return avg;

}

int funC(void) {

  // Create the structure to hold the array
  struct structArr sArr = { .arr = arr };

  // Declare a shortcut to the property of the structure
  int* sArrArr = sArr.arr;

  // Open the file containing the number
  FILE* fp =
    fopen(
      FILENAME,
      "rb");

  // Loop on the numbers
  for (
    int i = NB;
    i--;) {

    // Read the number
    int ret =
      fread(
        sArrArr + i,
        1,
        sizeof(int),
        fp);

  }

  // Close the file containing the number
  fclose(fp);

  // Declare a variable to memorise the sum of the numbers
  long sum = 0;

  // Loop on the numbers
  for (
    int i = NB;
    i--;) {

    // Calculate the sum
    sum += sArrArr[i];

  }

  // Calculate the average
  double avg = ((double)sum) / ((double)NB);

  // Return the result
  return avg;

}

int funD(void) {

  // Create the structure to hold the array
  struct structArr sArr = { .arr = arr };

  // Open the file containing the number
  FILE* fp =
    fopen(
      FILENAME,
      "rb");

  // Loop on the numbers
  for (
    int i = 0;
    i < NB;
    i++) {

    // Read the number
    int ret =
      fread(
        sArr.arr + i,
        1,
        sizeof(int),
        fp);

  }

  // Close the file containing the number
  fclose(fp);

  // Declare a variable to memorise the sum of the numbers
  long sum = 0;

  // Loop on the numbers
  for (
    int i = 0;
    i < NB;
    i++) {

    // Calculate the sum
    sum += sArr.arr[i];

  }

  // Calculate the average
  double avg = ((double)sum) / ((double)NB);

  // Return the result
  return avg;


}

int funE(void) {

  // Create the structure to hold the array
  struct structArr sArr = { .arr = arr };

  // Open the file containing the number
  FILE* fp =
    fopen(
      FILENAME,
      "rb");

  // Declare a pointer to scan the array elements
  int* ptr = sArr.arr;

  // Loop on the numbers
  for (
    int i = NB;
    i--;) {

    // Read the number
    int ret =
      fread(
        ptr,
        1,
        sizeof(int),
        fp);

    // Move to the next element
    ptr++;

  }

  // Close the file containing the number
  fclose(fp);

  // Declare a variable to memorise the sum of the numbers
  long sum = 0;

  // Reset the pointer to scan the array elements
  ptr = sArr.arr;

  // Loop on the numbers
  for (
    int i = NB;
    i--;) {

    // Calculate the sum
    sum += sArr.arr[i];

    // Move to the next element
    ptr++;

  }

  // Calculate the average
  double avg = ((double)sum) / ((double)NB);

  // Return the result
  return avg;

}

int funF(void) {

  // Create the structure to hold the array
  struct structArr sArr = { .arr = arr };

  // Open the file containing the number
  FILE* fp =
    fopen(
      FILENAME,
      "rb");

  // Loop on the numbers
  for (
    int i = NB;
    i--;) {

    // Read the number
    int ret =
      fread(
        sArr.arr + i,
        1,
        sizeof(int),
        fp);

  }

  // Close the file containing the number
  fclose(fp);

  // Declare a variable to memorise the sum of the numbers
  long sum = 0;

  // Loop on the numbers
  for (
    int i = NB;
    i--;) {

    // Calculate the sum
    sum += sArr.arr[i];

  }

  // Calculate the average
  double avg = ((double)sum) * INV_NB;

  // Return the result
  return avg;

}

int funG(void) {

  // Create the structure to hold the array
  struct structArr sArr = { .arr = arr };

  // Open the file containing the number
  FILE* fp =
    fopen(
      FILENAME,
      "rb");

  // Loop on the numbers
  for (
    register int i = NB;
    i--;) {

    // Read the number
    int ret =
      fread(
        sArr.arr + i,
        1,
        sizeof(int),
        fp);

  }

  // Close the file containing the number
  fclose(fp);

  // Declare a variable to memorise the sum of the numbers
  long sum = 0;

  // Loop on the numbers
  for (
    register int i = NB;
    i--;) {

    // Calculate the sum
    sum += sArr.arr[i];

  }

  // Calculate the average
  double avg = ((double)sum) / ((double)NB);

  // Return the result
  return avg;

}

double evaluate(
  int (* const fun)(void),
  char const* const name, 
  char const* const desc,
  double const* const baseTime) {

  struct timespec tval_before;
  struct timespec tval_after;

  // Measure time before execution
  clock_gettime(
    CLOCK_REALTIME,
    &tval_before);

  // Execute the function 1000 times
  for (
    int i = 0;
    i < 1000;
    i++) {

    int ret = (*fun)();

  }

  // Measure time after execution
  clock_gettime(
    CLOCK_REALTIME,
    &tval_after);

  // Convert to seconds
  double delay =
    (double)(tval_after.tv_sec - tval_before.tv_sec) +
    ((double)(tval_after.tv_nsec - tval_before.tv_nsec)/1000000000L);

  // If there is no given base time
  if (baseTime != NULL) {

    double perf = delay / (*baseTime) * 100.0;

    // Print the info about the evaluated function
    printf(
      "%06.3lf%% %s: %s\n",
      perf,
      name,
      desc);

  }

  // Return the measure time
  return delay;

} 

void generateData() {

  // Open the file containing the data
  FILE* fp =
    fopen(
      FILENAME,
      "wb");

  // Loop on the numbers
  for (
    int i = 0;
    i < NB;
    i++) {

    // Get a random int
    int v = rand();

    // Save the random int in the file
    fwrite(
      &v,
      sizeof(int),
      1,
      fp);

  }

  // Close the file containing the data
  fclose(fp);

}

int main() {

  // Initialise the random generator
  srandom(0);

  // Generate the file containing the numbers
  generateData();

  // Evaluate the functions
  double baseTime =
    evaluate(
      funA,
      "funA",
      "Baseline",
      NULL);

  double time =
    evaluate(
      funB,
      "funB",
      "Read all data at once instead of one by one",
      &baseTime);

  time =
    evaluate(
      funC,
      "funC",
      "Use a shortcut to access the property of the structure",
      &baseTime);

  time =
    evaluate(
      funD,
      "funD",
      "for loop forward instead of backward",
      &baseTime);

  time =
    evaluate(
      funE,
      "funE",
      "Scan elements of the array using a pointer instead of by index",
      &baseTime);

  time =
    evaluate(
      funF,
      "funF",
      "Multiply by hardcoded value of 1/NB instead of dividing by NB",
      &baseTime);

  time =
    evaluate(
      funG,
      "funG",
      "Use register for the index in the for loop",
      &baseTime);

  return 0;

}
