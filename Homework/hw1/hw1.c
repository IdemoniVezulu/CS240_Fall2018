/* Name, hw1.c, CS 24000, Fall 2018
 * Last updated August 30, 2018
 */

/* Add any includes here */

#include "hw1.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* Define get_section_average here */

float get_section_average(char *filename, int section_num) {
  FILE *file = fopen(filename, "r");
  int f_section_num;
  int f_score;
  char f_name[MAX_NAME_LEN] = "";
  int f_id;
  int score_sum = 0;
  int num_students = 0;

  // Check if the user has passed a valid section_num

  if (section_num <= 0 || section_num >= 10) {
    printf("Passed section is out of range...\n");

    if (file != NULL) {
      fclose(file);
    }

    return BAD_SECTION;
  }
  /*else {
    printf("Section Num: %d\n", section_num);
  }*/

  //printf("Before if...\n");

  // Check if the file exists

  if (file != NULL) {

    //printf("Entered if...\n");

    // num_read holds the number of elements that is read by fscanf

    int num_read = fscanf(file, "%[^;];%d;%d%%;%d\n",
                            f_name, &f_id, &f_score, &f_section_num);

    if (num_read < 4 && num_read != EOF) {
      //printf("Too less records...\n");

      fclose(file);
      return BAD_RECORD;
    }

    while (num_read != EOF) {

      //printf("Entered loop...\n");

      //printf("%s;%d;%d%%;%d\n", f_name, f_id, f_score, f_section_num);

      // Checks if the file's section_num is valid

      if (!isdigit((char)(f_section_num + '0'))) {

        //printf("Section is not a digit...\n");

        fclose(file);
        return BAD_RECORD;
      }

      if (f_section_num > 0 && f_section_num < 10) {
        if (f_section_num == section_num) {

         //printf("Calc starts\n");

          score_sum += f_score;
          num_students++;
        }
      }
      else {
        fclose(file);

        //printf("Bad section (outside bounds)...\n");

        return BAD_RECORD;
      }

      num_read = fscanf(file, "%[^;];%d;%d%%;%d\n",
                          f_name, &f_id, &f_score, &f_section_num);

      // Loop ends if num_read reached the end of the file

      if (num_read == EOF) {

        //printf("End reached...\n");

        break;
      }
      else if (num_read != 4) {

        //printf("Too less records...\n");

        fclose(file);
        return BAD_RECORD;
      }
    }
  }
  else {
    //fclose(file);

    //printf("File read error...\n");

    return FILE_READ_ERR;
  }

  fclose(file);

  if (num_students == 0) {
    return NO_STUDENTS;
  }
  else {
    return ((float)score_sum / (float)num_students);
  }
}

/* Define get_top_score here */

int get_top_score(char *filename) {
  int top_score = 0;
  FILE *file = fopen(filename, "r");
  char f_name[MAX_NAME_LEN] = "";
  int f_id;
  int f_score;
  int f_section_num;

  //printf("Outside if...\n");

  if (file != NULL) {

    //printf("Inside if...\n");

    int num_read = fscanf(file, "%[^;];%d;%d%%;%d\n",
                            f_name, &f_id, &f_score, &f_section_num);

    if(num_read != 4) {
      //printf("Too less records...\n");

      fclose(file);
      return BAD_RECORD;
    }

    while (num_read != EOF) {

      //printf("Inside loop..\n");

      //printf("%s;%d;%d%%;%d\n", f_name, f_id, f_score, f_section_num);

      if (top_score == 0 || f_score > top_score) {

        //printf("Top score changed...\n");

        top_score = f_score;
      }

      num_read = fscanf(file, "%[^;];%d;%d%%;%d\n",
                          f_name, &f_id, &f_score, &f_section_num);

      if (num_read == EOF) {

        //printf("End reached...\n");

        break;
      }
      else if(num_read != 4) {

        //printf("Too less records...\n");

        fclose(file);
        return BAD_RECORD;
      }
    }
  }
  else {

    //printf("File read error...\n");

    return FILE_READ_ERR;
  }

  fclose(file);

  //printf("End...\n");

  return top_score;
}

/* Define create_fail_list here */

int create_fail_list(char *in_file, char *out_file, int cutoff) {
  FILE *read_file = fopen(in_file, "r");
  FILE *write_file = fopen(out_file, "w");
  int f_section_num;
  int f_id;
  int f_score;
  char f_name[MAX_NAME_LEN] = "";
  int num_fail = 0;

  //printf("Outside if...\n");

  if (read_file != NULL) {

    //printf("Inside if...\n");

    int num_read = fscanf(read_file, "%[^;];%d;%d%%;%d\n",
                            f_name, &f_id, &f_score, &f_section_num);

    if(num_read != 4) {

      //printf("Too less records...\n");

      fclose(read_file);
      return BAD_RECORD;
    }

    while (num_read != EOF) {

      //printf("Inside loop...\n");

      if (cutoff <= 0 || cutoff >= 100) {
        fclose(read_file);
        //fclose(write_file);

        //printf("Bad cutoff...\n");

        return BAD_CUTOFF;
      }

      if (f_section_num <= 0 || f_section_num >= 10) {
        fclose(read_file);
        //fclose(write_file);

        //printf("Section out of bounds...\n");

        return BAD_RECORD;
      }

      if (write_file != NULL) {

        //printf("Calc started...\n");

        // If the score is less than the cutoff,
        // The id, name, and section is written into the file

        if (f_score < cutoff) {
          fprintf(write_file, "%d - %d - %s\n", f_section_num, f_id, f_name);
          num_fail++;
        }
      }
      else {
        fclose(read_file);
        //fclose(write_file);

        //printf("File write error...\n");

        return FILE_WRITE_ERR;
      }

      num_read = fscanf(read_file, "%[^;];%d;%d%%;%d\n",
                          f_name, &f_id, &f_score, &f_section_num);

      if (num_read == EOF) {

        //printf("End reached...\n");

        break;
      }
      else if(num_read != 4) {

        //printf("Too less records...\n");

        if (read_file != NULL) {
          fclose(read_file);
        }
        if (write_file != NULL) {
          fclose(write_file);
        }

        return BAD_RECORD;
      }
    }
  }
  else {
    if (read_file != NULL) {
      fclose(read_file);
    }
    if (write_file != NULL) {
      fclose(write_file);
    }

    //printf("File read error...\n");

    return FILE_READ_ERR;
  }

  fclose(read_file);
  fclose(write_file);

  if (num_fail == 0) {

    //printf("No students...\n");

    return NO_STUDENTS;
  }
  else {

    //printf("End...\n");

    return num_fail;
  }
}

/* Define create_grade_report here */

int create_grade_report(char *in_file, char *out_file, int section_num) {

  FILE *read_file = fopen(in_file, "r");
  FILE *write_file = fopen(out_file, "w");
  int num_students = 0;
  char f_name[MAX_NAME_LEN] = "";
  int f_id;
  int f_score;
  int f_section_num;

  if (section_num <= 0 || section_num >= 10) {

    //printf("Passed section is out of bounds...\n");

    if(read_file != NULL) {
      fclose(read_file);
    }

    if(write_file != NULL) {
      fclose(write_file);
    }

    return BAD_SECTION;
  }

  //printf("Before if...\n");

  if (read_file != NULL) {

    //printf("Inside if...\n");

    int num_read = fscanf(read_file, "%[^;];%d;%d%%;%d\n",
                            f_name, &f_id, &f_score, &f_section_num);

    if(num_read != 4) {

      //printf("Too less records...\n");

      fclose(read_file);
      if (write_file != NULL) {
        fclose(write_file);
      }

      return BAD_RECORD;
    }

    //printf("Section Number passed: %d\n", section_num);

    while (num_read != EOF) {

      //printf("Inside loop...\n");

      //printf("%s;%d;%d%%;%d\n", f_name, f_id, f_score, f_section_num);

      if (f_section_num <= 0 || f_section_num >= 10) {

        //printf("Bad section error...\n");

        fclose(read_file);
        //fclose(write_file);

        return BAD_RECORD;
      }

      if (write_file != NULL) {

        //printf("Calc starts...\n");

        // Check the scores with the given cutoffs
        // If the student is in the required section, determine final grade

        if (f_section_num == section_num) {
          char letter_grade;

          //printf("Letter Grade Calc...\n");

          if (f_score >= 90) {
            letter_grade = 'A';
          }
          else if (f_score >= 80 && f_score < 90) {
            letter_grade = 'B';
          }
          else if (f_score >= 70 && f_score < 80) {
            letter_grade = 'C';
          }
          else if (f_score >= 60 && f_score < 70) {
            letter_grade = 'D';
          }
          else {
            letter_grade = 'F';
          }

          fprintf(write_file, "%d: %c\n", f_id, letter_grade);
          num_students++;
        }
      }
      else {
        fclose(read_file);
        //fclose(write_file);

        //printf("File write error...\n");

        return FILE_WRITE_ERR;
      }

      num_read = fscanf(read_file, "%[^;];%d;%d%%;%d\n",
                          f_name, &f_id, &f_score, &f_section_num);

      if (num_read == EOF) {

        //printf("End reached...\n");

        break;
      }
      else if(num_read != 4) {

        //printf("Too less records...\n");

        if (read_file != NULL) {
          fclose(read_file);
        }
        if (write_file != NULL) {
          fclose(write_file);
        }

        return BAD_RECORD;
      }
    }
  }
  else {
    //fclose(read_file);
    fclose(write_file);

    //printf("File read error...\n");

    return FILE_READ_ERR;
  }

  fclose(read_file);
  fclose(write_file);

  if(num_students == 0) {

    //printf("No students...\n");

    return NO_STUDENTS;
  }
  else {

    //printf("End...\n");

    return num_students;
  }
}

/* Remember, you don't need a main function!
 * it is provided by hw1_main.c or hw1_test.o
 */

