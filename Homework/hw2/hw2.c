/* Name, hw2.c, CS 24000, Fall 2018
 * Last updated September 6, 2018
 */

/* Add any includes here */

#include "hw2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define any global variables here */

extern int g_product_count;
extern char g_product_name[MAX_RECORDS][MAX_PRODUCT_LEN];
extern float g_unit_price[MAX_RECORDS];
extern int g_availability[MAX_RECORDS][STORES];

int g_product_count;
char g_product_name[MAX_RECORDS][MAX_PRODUCT_LEN];
float g_unit_price[MAX_RECORDS];
int g_availability[MAX_RECORDS][STORES];

/* Define your functions here */

/*
* This function takes in the filename as an argument and fills the
* g_product_name[], g_unit_price[], and g_availability[] arrays.
* It returns the total number of records that have been read.
*/

int input_tables(char *filename) {
  int total_records = 0;
  FILE *file = fopen(filename, "r");

  // Check if file exists

  if (file != NULL) {

    // Scan the fie and read input

    // rec_read holds the number of records that are read

    int rec_read = fscanf(file, "%[^,],$%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                            g_product_name[0], &g_unit_price[0],
                            &g_availability[0][0], &g_availability[0][1],
                            &g_availability[0][2], &g_availability[0][3],
                            &g_availability[0][4], &g_availability[0][5],
                            &g_availability[0][6], &g_availability[0][7],
                            &g_availability[0][8], &g_availability[0][9]); 

    // If fscanf reads less than 12 records or
    // If it reaches the EOF an error is thrown

    if ((rec_read < 12) && (rec_read != EOF)) {
      fclose(file);
      file = NULL;
      return RECORD_ERROR;
    }

    total_records++;

    // This loop iterates through each of the arrays and fills them
    // with the data from the file

    for (int i = 1; (i < MAX_RECORDS) && (rec_read != EOF); i++) {

      rec_read = fscanf(file, "%[^,],$%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                            g_product_name[i], &g_unit_price[i],
                            &g_availability[i][0], &g_availability[i][1],
                            &g_availability[i][2], &g_availability[i][3],
                            &g_availability[i][4], &g_availability[i][5],
                            &g_availability[i][6], &g_availability[i][7],
                            &g_availability[i][8], &g_availability[i][9]);

      // The loop breaks if the EOF is reached

      if (rec_read == EOF) {
        break;
      }
      else if (rec_read != 12) {
        fclose(file);
        file = NULL;
        return RECORD_ERROR;
      }

      total_records++;
    }
  }
  else {
    return FILE_READ_ERROR;
  }

  fclose(file);
  file = NULL;
  g_product_count = total_records;

  return total_records;
}/*input_tables()*/

/*
* This function takes the product name as an argument and returns its
* unit price.
*/

float unit_price(char *product_name) {
  float p_unit_price = 0.0f;

  // This loop iterates through the g_product_name array
  // It returns the unit price once the product is found

  for (int i = 0; i < g_product_count; i++) {
    if (strcmp(product_name, g_product_name[i]) == 0) {
      p_unit_price = g_unit_price[i];
      break;
    }
  }

  if (p_unit_price == 0.0f) {
    return NO_SUCH_PRODUCT;
  }

  return p_unit_price;
}/*unit_price()*/

/*
* This function takes the store number as the first argument and returns
* its net worth.
* Net worth is determined by summing the result of multiplying
* each products availability with its unit price.
*/

float net_worth(int store_num) {
  float net_worth = 0.0f;

  if ((store_num < 0) || (store_num > STORES)) {
    return OUT_OF_BOUNDS;
  }
  else {
    for (int i = 0; i < g_product_count; i++) {
      net_worth += g_availability[i][store_num] * g_unit_price[i];
    }
  }

  return net_worth;
}/*net_worth()*/

/*
* This function takes the store number as the first argument and returns
* the maximum net product value.
* The net product value is determined by multiplying a
* products availability by its unit price.
*/

float max_net_product(int store_num) {
  float max_net_value = 0.0f;

  if ((store_num < 0) || (store_num > STORES)) {
    return OUT_OF_BOUNDS;
  }
  else {
    for (int i = 0; i < g_product_count; i++) {
      if (max_net_value < (g_availability[i][store_num] * g_unit_price[i])) {
        max_net_value = g_availability[i][store_num] * g_unit_price[i];
      }
    }
  }

  return max_net_value;
}/*max_net_product()*/

/*
* This function takes the product name as the first argument and returns
* the store number corresponding to the store that has the largest
* availability for that particular product.
*/

int max_prod_availability(char *product_name) {
  int store_num = -1;
  int temp_store_availability;

  for (int i = 0; i < g_product_count; i++) {
    if (strcmp(product_name, g_product_name[i]) == 0) {
      temp_store_availability = g_availability[i][0];
      store_num = 0;

      for (int j = 1; j < STORES; j++) {
        if (g_availability[i][j] > temp_store_availability) {
          temp_store_availability = g_availability[i][j];
          store_num = j;
        }
      }
    }
  }

  if (store_num == -1) {
    return NO_SUCH_PRODUCT;
  }

  return store_num;
}/*max_prod_availability()*/

/*
* This function takes the store number as the first argument and returns
* the index corresponding to the product name that has the largest
* availability in the given store.
*/

int max_store_availability(int store_num) {
  int index_product_name = -1;

  if ((store_num < 0) || (store_num > STORES)) {
    return OUT_OF_BOUNDS;
  }
  else {
    int temp_availability = g_availability[0][store_num];
    index_product_name = 0;

    for (int i = 1; i < g_product_count; i++) {
      if (temp_availability < g_availability[i][store_num]) {
        temp_availability = g_availability[i][store_num];
        index_product_name = i;
      }
    }
  }

  if (index_product_name == -1) {
    return NO_SUCH_PRODUCT;
  }

  return index_product_name;
}/*max_store_availability()*/

/*
* This function accepts a character string that represents a filename to
* open for writing and two integers representing the start and end
* stores to be output.
* It will return OK if no errors occur and output to the specified
* file the product names, unit prices, and product availabilities between
* the start and end stores.
*/

int output_tables(char *filename, int start_store, int end_store) {
  FILE *write_file = fopen(filename, "w");

  if ((start_store < 0) || (start_store > STORES) ||
        (end_store < 0) || (end_store > STORES)) {
    if (write_file != NULL) {
      fclose(write_file);
      write_file = NULL;
    }

    return OUT_OF_BOUNDS;
  }
  else {
    if (write_file != NULL) {
      for (int i = 0; i < g_product_count; i++) {
        fprintf(write_file, "%s,$%.2f,", g_product_name[i], g_unit_price[i]);

        for (int j = start_store; j <= end_store; j++) {
          fprintf(write_file, "%d", g_availability[i][j]);
          if (j < end_store) {
            fprintf(write_file, ",");
          }
        }
        if (i < MAX_RECORDS) {
          fprintf(write_file, "\n");
        }
      }
    }
    else {
      return FILE_WRITE_ERROR;
    }
  }

  fclose(write_file);
  write_file = NULL;

  return OK;
}/*output_tables()*/

