/* Name, hw0.c, CS 24000, Fall 2018
 * Last updated August 16, 2018 
 */

/* Add any includes here */

#include "hw0.h"

/* Define compute_hailstone here */

/*
 *  This function makes a hailstone with the data that is passed into it,
 *  The sum of the hailstone sequence made is then returned.
*/

int compute_hailstone(int n, int l) {	
  int sum = 0;

  // Makes the hailstone sequence if the user has input valid numbers
	 
  if (n > 0 && l > 0) {
    // Stores user's number as the first number of the array
		
    g_hailstone_array[0] = n;

    sum = g_hailstone_array[0];

    // Loop to make hailstone sequence with user's input
		
    for (int i = 1; i < l; i++) {
      // Check if number is even or odd and calculate next number
		
      if (n % 2 == 0) n /= 2;
        else n = 3 * n + 1;

      // Write newly calculated number into array
		
      g_hailstone_array[i] = n;

      sum += g_hailstone_array[i];
    }
	return sum;

  }
  // Return error  if user inputs invalid numbers
	
  else return HAILSTONE_ERROR;
} // compute_hailstone()

/* Define check_hailstone here */

/*
 *  This function checks the validity of the hailstone sequence the user enters
 *  It returns HAILSTONE_CORRECT if the sequence is found to be valid.
*/

int check_hailstone(int n) {
  // Hailstone sequence check is done if user has input valid numbers
	
  if (n >= 1 && g_hailstone_array[0] >= 1) {
    int temp_array[ARRAY_SIZE];
		
    // 1 - Equal, 0 - Unequal

    int is_equal = 0;
    int index_of_wrong_num = 0;
		
    // Copy first element from g_hailstone_array to temp_array

    temp_array[0] = g_hailstone_array[0];
		
    // Loop to create correct hailstone sequence

    for (int j = 0; j < ARRAY_SIZE; j++) {			
      if (temp_array[j] % 2 == 0) temp_array[j + 1] = temp_array[j] / 2;
        else temp_array[j + 1] = 3 * temp_array[j] + 1;
    }
		
    // Loop to check if the contents of the two arrays are the same
		
    for (int i = 0; i < n; i++) {
      if (temp_array[i] == g_hailstone_array[i]) is_equal = 1;
        else {
          is_equal = 0;
          index_of_wrong_num = i;
          break;
        }
    }

    if (is_equal == 1) return HAILSTONE_CORRECT;
      else return index_of_wrong_num;
  }
  else return HAILSTONE_ERROR;
} // check_hailstone()

/* Remember, you don't need a main function!
 * it is provided by hw0_main.c or hw0_test.o
 */

