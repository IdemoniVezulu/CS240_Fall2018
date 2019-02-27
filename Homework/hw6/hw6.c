/*
 * Name, hw6.c, CS 24000, Fall 2018
 * Last updated October 4, 2018
 */

/*
 * Include files below
 */

#include "hw6.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

/*
 * #Defines below
 */

#define BUFFER_LENGTH (20)
#define NUM_RECORDS (6)
#define VALID (0)
#define INVALID (-1)

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
 * This function will read from the file specified in the argument and populate
 * the g_human_array.
 * It returns the number of humans read from the file.
 */

int read_humans(char *filename) {
  assert(filename != NULL);

  FILE *file = fopen(filename, "r");
  int total_records = 0;

  if (file == NULL) {
    return NON_READABLE_FILE;
  }
  else {

    // Temporary variables to store the file contents in

    char f_first_name[BUFFER_LENGTH];
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

    // Check if a valid number of records were read from the file

    if ((rec_read < NUM_RECORDS) && (rec_read != EOF)) {
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

    // Set the pointers to null

    g_human_array[0].father_ptr = NULL;
    g_human_array[0].mother_ptr = NULL;
    g_human_array[0].child_ptr = NULL;
    g_human_array[0].sibling_ptr = NULL;

    total_records++;

    // Loop to fill the array with the remaining file records

    for (int i = 1; (i < MAX_HUMANS) || (rec_read != EOF); i++) {
      rec_read = fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
                                c_last_name, c_first_name,
                                m_last_name, m_first_name,
                                f_last_name, f_first_name);

      // Check if EOF is reached

      if (rec_read == EOF) {
        break;
      }
      else if (rec_read != NUM_RECORDS) {
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

      // Set the pointers to null

      g_human_array[i].father_ptr = NULL;
      g_human_array[i].mother_ptr = NULL;
      g_human_array[i].child_ptr = NULL;
      g_human_array[i].sibling_ptr = NULL;

      total_records++;
    }
  }

  g_human_count = total_records;

  fclose(file);
  file = NULL;
  return total_records;
}/*read_humans()*/

/*
 * This function makes the family for each human in g_human_array.
 * It iterates through the array to find the mother, father, child, and sibling
 * of each human (if they exist).
 */

void establish_family() {

  // Find mother of each human_t

  for (int i = 0; i < g_human_count; i++) {
    int mother_found = INVALID;

    for (int j = 0; j < g_human_count; j++) {
      if ((strcmp(g_human_array[i].mother_last, g_human_array[j].last_name)
          == 0) &&
          (strcmp(g_human_array[i].mother_first, g_human_array[j].first_name)
          == 0) && (g_human_array[i].mother_ptr == NULL)) {
        g_human_array[i].mother_ptr = &g_human_array[j];
        mother_found = VALID;
        //printf("Mother found...\n");
      }

      if (mother_found == VALID) {
        break;
      }
    }
  }

  // Find father of each human_t

  for (int i = 0; i < g_human_count; i++) {
    int father_found = INVALID;

    for (int j = 0; j < g_human_count; j++) {
      if ((strcmp(g_human_array[i].father_last, g_human_array[j].last_name)
           == 0) &&
          (strcmp(g_human_array[i].father_first, g_human_array[j].first_name)
           == 0) && (g_human_array[i].father_ptr == NULL)) {
        g_human_array[i].father_ptr = &g_human_array[j];
        father_found = VALID;
        //printf("Father found...\n");
      }

      if (father_found == VALID) {
        break;
      }
    }
  }

  // Find child and sibling of each parent

  for (int i = 0; i < g_human_count; i++) {
    if ((g_human_array[i].father_ptr != NULL) &&
        ((*g_human_array[i].father_ptr).child_ptr == NULL)) {
      (*g_human_array[i].father_ptr).child_ptr = &g_human_array[i];
    }

    if ((g_human_array[i].mother_ptr != NULL) &&
        ((*g_human_array[i].mother_ptr).child_ptr == NULL)) {
      (*g_human_array[i].mother_ptr).child_ptr = &g_human_array[i];
    }

    // Loop to find the siblings

    for (int j = i + 1; j < g_human_count; j++) {
      int flag = VALID;
      if ((strcmp(g_human_array[i].father_last,
            g_human_array[j].father_last) == 0) &&
          (strcmp(g_human_array[i].father_first,
            g_human_array[j].father_first) == 0) &&
          (strcmp(g_human_array[i].mother_last,
            g_human_array[j].mother_last) == 0) &&
          (strcmp(g_human_array[i].mother_first,
            g_human_array[j].mother_first) == 0)) {
        struct human_struct *temp_ptr = &g_human_array[i];

        // While loop finds the first instance of a NULL sibling_ptr

        while (flag == VALID) {
          if (temp_ptr->sibling_ptr == NULL) {
            temp_ptr->sibling_ptr = &g_human_array[j];
            flag = INVALID;
          }
          else {
            temp_ptr = temp_ptr->sibling_ptr;
          }
        }
      }

      if (flag == INVALID) {
        break;
      }
    }
  }
}/*establish_family()*/
