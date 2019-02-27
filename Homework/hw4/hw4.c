/*
 * Name, hw4.c, CS 24000, Fall 2018
 * Last updated September 21, 2018
 */

/*
* Include files are below
*/

#include "hw4.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

/*
* #Defines below
*/

#define MAX_LENGTH (50)
#define ENUM_UND ('U')
#define ENUM_GRA ('G')
#define NOT_FOUND_VALUE (-1)

/*
* Global variables are below
*/

extern student_t g_student_array[MAX_STUDENTS];
extern unsigned int g_student_count;

student_t g_student_array[MAX_STUDENTS];
unsigned int g_student_count;

/*
* Function definitions are below
*/

/*
*
* This function will read a list of students from the file specified by
* the first argument. The students should be stored into the
* g_student_array.
* Appropriate errors are returned whenever they are encountered.
* It returns the number of records read.
*
*/
int read_students(char *filename) {
  assert(filename != NULL);

  FILE *file = fopen(filename, "r");
  int tot_read = 0;

  // Checks if file is null and returns an error if it is null

  if(file == NULL) {
    return NON_READABLE_FILE;
  }
  else {
    char temp_schedule[MAX_LENGTH];
    char enum_read;
    int rec_read = fscanf(file, "%u,%[^,],%[^,],%c,$%f,%hu,%s\n",
                            &g_student_array[0].id_number,
                            g_student_array[0].last_name,
                            g_student_array[0].first_name,
                            &enum_read,
                            &g_student_array[0].money,
                            &g_student_array[0].dorm_room,
                            temp_schedule);

    /* Store the classification depending on the input
     * if it is an invalid, it returns READ_BAD_RECORD
    */

    if ((enum_read == ENUM_UND) || (enum_read == ENUM_GRA)) {
      if (enum_read == ENUM_UND) {
        g_student_array[0].classification = UNDERGRADUATE;
      }
      else if (enum_read == ENUM_GRA) {
        g_student_array[0].classification = GRADUATE;
      }
    }
    else {
      fclose(file);
      file = NULL;
      return READ_BAD_RECORD;
    }

    // If fscanf reads less than 7 records or
    // If it reaches the EOF an error is thrown

    if ((rec_read < 7) && (rec_read != EOF)) {
      fclose(file);
      file = NULL;
      return READ_BAD_RECORD;
    }

    // Loop reads the number of elements in temp_schedule

    int num_rec = 0;
    while(num_rec < MAX_LENGTH) {
      if(isdigit(temp_schedule[num_rec])) {
        num_rec++;
      }
      else {
        fclose(file);
        file = NULL;
        return  READ_BAD_RECORD;
      }
    }

    // The schedule is written into the global array if it has 50 characters
    // If it does not have 50 characters, it returns an error

    if(num_rec == MAX_LENGTH) {
      for(int j = MONDAY, exp = 0; j <= FRIDAY; j++, exp += 10) {
        for(int k = HOUR_7; k <= HOUR_16; k++) {
          g_student_array[0].schedule[j][k] = temp_schedule[exp + k] - '0';
        }
      }
    }
    else {
      fclose(file);
      file = NULL;
      return READ_BAD_RECORD;
    }
      
    tot_read++;

    // This loop iterates through each of the arrays and fills them
    // with the data from the file

    for (int i = 1; (i < MAX_STUDENTS) && (rec_read != EOF); i++) {

      rec_read = fscanf(file, "%u,%[^,],%[^,],%c,$%f,%hu,%s\n",
                          &g_student_array[i].id_number,
                          g_student_array[i].last_name,
                          g_student_array[i].first_name,
                          &enum_read,
                          &g_student_array[i].money,
                          &g_student_array[i].dorm_room,
                          temp_schedule);

      // Store the enum value depending on the input
      // if it is invalid, it returns READ_BAD_RECORD

      if ((enum_read == ENUM_UND) || (enum_read == ENUM_GRA)) {
        if (enum_read == ENUM_UND) {
          g_student_array[i].classification = UNDERGRADUATE;
        }
        else if (enum_read == ENUM_GRA) {
          g_student_array[i].classification = GRADUATE;
        }
      }
      else {
        fclose(file);
        file = NULL;
        return READ_BAD_RECORD;
      }

      // The loop breaks if the EOF is reached

      if (rec_read == EOF) {
        break;
      }
      else if (rec_read != 7) {
        fclose(file);
        file = NULL;
        return READ_BAD_RECORD;
      }

      // Loop reads the number of elements in temp_schedule

      num_rec = 0;
      while(num_rec < MAX_LENGTH) {
        if(isdigit(temp_schedule[num_rec])) {
          num_rec++;
        }
        else {
          fclose(file);
          file = NULL;
          return READ_BAD_RECORD;
        }
      }

      // The schedule is written into the global array if it has 50 characters
      // If it does not have 50 characters, it returns an error

      if(num_rec == MAX_LENGTH) {
        for(int j = MONDAY, exp = 0; j <= FRIDAY; j++, exp += 10) {
          for(int k = HOUR_7; k <= HOUR_16; k++) {
            g_student_array[i].schedule[j][k] = temp_schedule[exp + k] - '0';
          }
        }
      }
     else {
        fclose(file);
        file = NULL;
        return READ_BAD_RECORD;
      }

      tot_read++;
    }
  }

  fclose(file);
  file = NULL;
  g_student_count = tot_read;

  return g_student_count;
}/*read_students()*/

/*
*
* This function will write a list of students to the file specified by
* the first argument. The second argument specifies the number of
* entries from g_student_array to write.
* It returns the number of records written successfully.
*
*/

int write_students(char *filename, int num_entries) {
  assert(filename != NULL);
  assert(num_entries >= 0);

  int rec_written = 0;
  FILE *file = fopen(filename, "w");

  if (file == NULL) {
    return NON_WRITABLE_FILE;
  }
  else {

    // The file contents are written if it is not null

    char temp_schedule[MAX_LENGTH + 1] = "";
    char enum_val = ' ';

    for (int i = 0; i < num_entries; i++) {
      if (g_student_array[i].classification == 0) {
        enum_val = 'U';
      }
      else {
        enum_val = 'G';
      }
      fprintf(file, "%u,%s,%s,%c,$%.2f,%hu,",
                          g_student_array[i].id_number,
                          g_student_array[i].last_name,
                          g_student_array[i].first_name,
                          enum_val,
                          g_student_array[i].money,
                          g_student_array[i].dorm_room);

      // Write schedule into the file after parsing it

      for(int j = MONDAY, exp = 0; j <= FRIDAY; j++, exp += 10) {
        for(int k = HOUR_7; k <= HOUR_16; k++) {
          temp_schedule[k + exp] = g_student_array[i].schedule[j][k] + '0';
        }
      }
 
      fprintf(file, "%s\n", temp_schedule);

      rec_written++;
    }
  }

  fclose(file);
  file = NULL;
  return rec_written;
}/*write_students()*/

/*
*
* This function is used to search for a student by their id.
* It returns the array index of that student_t in the array.
* If the student cannot be found, it returns a NOT_FOUND error.
*
*/

int find_student_by_id(unsigned int id_num) {
  int index = NOT_FOUND_VALUE;

  // Loop propogates through the g_student_array and breaks when
  // the student is found

  for (int i = 0; i < g_student_count; i++) {
    if (g_student_array[i].id_number == id_num) {
      index = i;
      break;
    }
  }

  if (index != NOT_FOUND_VALUE) {
    return index;
  }
  else {
    return NOT_FOUND;
  }
}/*find_student_by_id()*/

/*
*
* This function propogates through the g_student_array and searched
* for the student_t with the first arg as first name and the second arg
* as last name.
* It returns the index of the first occurence of this record if it exists, and
* NOT_FOUND error if the record does not exist.
*/

int find_student_by_name(char *f_name, char *l_name) {
  assert((f_name != NULL) && (l_name != NULL));

  int index = NOT_FOUND_VALUE;

  // Loop iterates through g_student_array and finds the first student
  // who's first name and last name match the arguments of the function.
  
  if (f_name != NULL && l_name != NULL) {
    for (int i = 0; i < g_student_count; i++) {
      if ((strcmp(g_student_array[i].first_name, f_name) == 0) &&
            (strcmp(g_student_array[i].last_name, l_name) == 0)) {
        index = i;
        break;
      }
    }
  }

  if (index != NOT_FOUND_VALUE) {
    return index;
  }
  else {
    return NOT_FOUND;
  }
}/*find_student_by_name()*/

/*
*
* This function iterates through g_student_array and 
* returns the percentage of students who have classification as undergraduate.
* 
*/

float compute_undergraduate_percent() {
  int num_und = 0;

  // Loop iterates through g_student_array and calculates the number of
  // students classified as undergraduates.

  for (int i = 0; i < g_student_count; i++) {
    if(g_student_array[i].classification == 0) {
      num_und++;
    }
  }

  // Percentage of undergraduate students is calculated

  return (((float) num_und / (float) g_student_count) * 100);
}/*compute_undergraduate_percent()*/

/*
*
* This function finds the number of students with the least amount of money and
* gives this student one dollar from all the other students' money.
* It returns NOT_FOUND if there are two or more students with the least amount
* of money, otherwise, it returns the index of the student.
*
*/

int grant_scholarship() {
  int index = NOT_FOUND_VALUE;
  float temp_mon = g_student_array[0].money;
  int num_sub = 0;

  // Loop to find the student with the least money

  for (int i = 1; i < g_student_count; i++) {
    if (g_student_array[i].money < temp_mon) {
      temp_mon = g_student_array[i].money;
      index = i;
    }
  }

  // Loop to check if there are more than 1 students with the same amount of
  // money

  for (int k = 0; k < g_student_count; k++) {
    if ((k != index) && (g_student_array[index].money ==
                          g_student_array[k].money)) {
      return NOT_FOUND;
    }
  }

  // Loop to subtract $1 from each student and give it to the poorest student

  for (int j = 0; j < g_student_count; j++) {
    if (j != index) {
      g_student_array[j].money--;
      num_sub++;
    }
  }


  // Gives the student the total of the money subtracted from the others

  g_student_array[index].money += num_sub;


  if (index != NOT_FOUND_VALUE) {
    return index;
  }
  else {
    return NOT_FOUND;
  }
}/*grant_scholarship()*/

/*
*
* This function scans the students and finds the dorm room with three or more
* people.
* It returns NOT_FOUND if no room like this exists, otherwise, it returns the
* room number.
*
*/

int find_full_dorm_room() {
  int room_num = NOT_FOUND_VALUE;

  // Loop iterates through the array from an element and then iterates through
  // the array from the position of the previous element to find students with
  // the same dorm room and increments the num_people  variable
  
  for (int i = 0; i < g_student_count; i++) {
    int temp_num = 0;
    int temp_room = 0;
    for (int j = 0; j < g_student_count; j++) {
      if (g_student_array[i].dorm_room == g_student_array[j].dorm_room) {
        temp_num++;
        if (temp_num >= 3) {
          temp_room = g_student_array[i].dorm_room;
        }
      }
    }

    // If the number of people in a dorm room is atleast 3,
    // the dorm's room number is stored in the room_num variable.

    if ((temp_room < room_num) && (temp_num >= 3)) {
      room_num = temp_room;
    }
    else if ((room_num == -1) && (temp_num >= 3)) {
      room_num = temp_room;
    }
  }

  if (room_num == NOT_FOUND_VALUE) {
    return NOT_FOUND;
  }
  else {
    return room_num;
  }
}/*find_full_dorm_room()*/

/*
*
* This function takes in the ID number of two students and finds the first time
* during a week that neither of them have scheduling conflicts.
* If returns NOT_FOUND if either of the students aren't found, NOT_SCHEDULABLE
* if they have no free time in common. If there is no error, it returns the day
* of the week times 100 plus the hour of the day.
*
*/

unsigned int find_earliest_meeting(unsigned int num_id_one,
                                    unsigned int num_id_two) {
  unsigned int free_hour = NOT_FOUND_VALUE;
  int index_one = NOT_FOUND_VALUE;
  int index_two = NOT_FOUND_VALUE;

  // Check if the ID numbers passed to the function exists in the list
  // Return NOT_FOUND if they don't exist.

  for (int i = 0; i < g_student_count; i++) {
    if (g_student_array[i].id_number == num_id_one) {
      index_one = i;
    }
    else if (g_student_array[i].id_number == num_id_two) {
      index_two = i;
    }
  }

  if ((index_one == NOT_FOUND_VALUE) || (index_two == NOT_FOUND_VALUE)) {
    return NOT_FOUND;
  }
  else {

    // If the ID numbers do exist, their schedule is checked for availability

    int is_found = NOT_FOUND_VALUE;
    for (int i = MONDAY; i <= FRIDAY; i++) {
      for (int j = HOUR_7; j <= HOUR_16; j++) {
        if ((g_student_array[index_one].schedule[i][j] == 0) &&
              (g_student_array[index_two].schedule[i][j] == 0)) {
          free_hour = (i * 100) + (j + 7); // Add 7 to j to get the hour
          is_found = 1;
          break;
        }
        if (is_found == 1) {
          break;
        }
      }
      if (is_found == 1) {
        break;
      }
    }
  }

  if (free_hour == NOT_FOUND_VALUE) {
    return NOT_SCHEDULABLE;
  }
  else {
    return free_hour;
  }
}/*find_earliest_meeting()*/
