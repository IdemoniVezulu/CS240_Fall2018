/*
 * Name, hw5.c, CS 24000, Fall 2018
 * Last updated September 27, 2018
 */

/*
* Include files below
*/

#include "hw5.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/*
* #Defines below
*/

#define BUFFER_LENGTH (20)
#define VALID (1)
#define INVALID (0)
#define CHILD_NOT_FOUND (0)

/*
* Global variables below
*/

extern human_t g_human_array[MAX_HUMANS];
extern unsigned int g_human_count;

human_t g_human_array[MAX_HUMANS];
unsigned int g_human_count = 0;

/*
* Function definitions below
*/

/*
* This function checks if the array contains anything other than alphabets
* It returns INVALID if a non-alphabetic character is found, and VALID if it is
* only made of characters.
*
*/

int check_validity(char array[BUFFER_LENGTH]) {

  for (int i = 0; i < BUFFER_LENGTH; i++) {
    if (array[i] == ',') {
      return INVALID;
    }
  }

  return VALID;
}/*check_validity()*/

/*
* This function reads the humans from the file in the argument and they are
* stored in the global array if there no errors.
*
*/

int read_humans(char *filename) {
  assert(filename != NULL);

  FILE *file = fopen(filename, "r");
  int total_records = 0;

  if (file == NULL) {
    return NON_READABLE_FILE;
  }
  else {
    char f_first_name[BUFFER_LENGTH] = "";
    char f_last_name[BUFFER_LENGTH] = "";
    char m_first_name[BUFFER_LENGTH] = "";
    char m_last_name[BUFFER_LENGTH] = "";
    char c_first_name[BUFFER_LENGTH] = "";
    char c_last_name[BUFFER_LENGTH] = "";

    // Storing the scanf value in a variable to check EOF and number of records

    int rec_read = fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
                                    c_last_name, c_first_name,
                                    m_last_name, m_first_name,
                                    f_last_name, f_first_name);

    // Check if the last array has two words in it

    if (check_validity(f_first_name) == INVALID) {
      fclose(file);
      file = NULL;
      return READ_BAD_RECORD;
    }

    // Check if any of the contents are null

    if ((strcmp(c_last_name, "") == 0) || (strcmp(c_first_name, "") == 0) ||
        (strcmp(f_last_name, "") == 0) || (strcmp(f_first_name, "") == 0) ||
        (strcmp(m_last_name, "") == 0) || (strcmp(m_first_name, "") == 0)) {
      fclose(file);
      file = NULL;
      return READ_BAD_RECORD;
    }

    // Check if a valid number of records were read from the file

    if ((rec_read < 6) && (rec_read != EOF)) {
      fclose(file);
      file = NULL;
      return READ_BAD_RECORD;
    }

    // Add the '\0' to the end of the temporary strings

    c_last_name[NAME_LENGTH] = '\0';
    c_first_name[NAME_LENGTH] = '\0';
    f_first_name[NAME_LENGTH] = '\0';
    f_last_name[NAME_LENGTH] = '\0';
    m_first_name[NAME_LENGTH] = '\0';
    m_last_name[NAME_LENGTH] = '\0';

    // Write to global array from temporary arrays

    strncpy(g_human_array[0].last_name, c_last_name, NAME_LENGTH - 1);
    strncpy(g_human_array[0].first_name, c_first_name, NAME_LENGTH - 1);
    strncpy(g_human_array[0].father_first, f_first_name, NAME_LENGTH -1);
    strncpy(g_human_array[0].father_last, f_last_name, NAME_LENGTH - 1);
    strncpy(g_human_array[0].mother_first, m_first_name, NAME_LENGTH - 1);
    strncpy(g_human_array[0].mother_last, m_last_name, NAME_LENGTH - 1);

    total_records++;

    // Loop to fill the array with all the other file records

    for (int i = 1; (i < MAX_HUMANS) || (rec_read != EOF); i++) {
      rec_read = fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
                                  c_last_name, c_first_name,
                                  m_last_name, m_first_name,
                                  f_last_name, f_first_name);

      // Check if EOF is reached

      if (rec_read == EOF) {
        break;
      }
      else if (rec_read != 6) {
        fclose(file);
        file = NULL;
        return READ_BAD_RECORD;
      }

      // Check if the last array contains two words

      if (check_validity(f_first_name) == INVALID) {
        fclose(file);
        file = NULL;
        return READ_BAD_RECORD;
      }

      // Add the '\0' to the end of the temporary strings

      c_last_name[NAME_LENGTH] = '\0';
      c_first_name[NAME_LENGTH] = '\0';
      f_first_name[NAME_LENGTH] = '\0';
      f_last_name[NAME_LENGTH] = '\0';
      m_first_name[NAME_LENGTH] = '\0';
      m_last_name[NAME_LENGTH] = '\0';

      // Write to global array from temporary arrays

      strncpy(g_human_array[i].last_name, c_last_name, NAME_LENGTH - 1);
      strncpy(g_human_array[i].first_name, c_first_name, NAME_LENGTH - 1);
      strncpy(g_human_array[i].father_first, f_first_name, NAME_LENGTH -1);
      strncpy(g_human_array[i].father_last, f_last_name, NAME_LENGTH - 1);
      strncpy(g_human_array[i].mother_first, m_first_name, NAME_LENGTH - 1);
      strncpy(g_human_array[i].mother_last, m_last_name, NAME_LENGTH - 1);

      total_records++;
    }
  }

  g_human_count = total_records;

  fclose(file);
  file = NULL;
  return total_records;
}/*read_humans()*/

/*
*
*/

int find_children(char *last_name, char *first_name, int *child_array) {
  assert((last_name != NULL) && (first_name != NULL) && (child_array != NULL));

  int num_children = CHILD_NOT_FOUND;

  // Search for the parents in the global array by iterating through it

  int child_index = 0;
  for (int i = 0; i < g_human_count; i++) {

    // If the arguments match the father's or mother's full name, the index is
    // stored in the array that child_array points to. The number of children
    // found is also incremented

    if (((strcmp(g_human_array[i].father_first, first_name) == 0) &&
         (strcmp(g_human_array[i].father_last, last_name) == 0)) ||
         ((strcmp(g_human_array[i].mother_first, first_name) == 0) &&
          (strcmp(g_human_array[i].mother_last, last_name) == 0))) {
      child_array[child_index] = i;
      child_index++;
      num_children++;
    }
  }

  return num_children;
}/*find_children()*/
