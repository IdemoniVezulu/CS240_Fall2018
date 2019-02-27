/*
 * Name, hw12.c, CS 24000, Fall 2018
 * Last updated November 25, 2018
 */

/*
 * Include files below
 */

//#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc_debug.h>

#include "object.h"
#include "graphics.h"

/*
 * #Defines below
 */

#define VALID (1)
#define ZER_IND (0)
#define FIR_IND (1)
#define SEC_IND (2)
#define BUFFER_SIZE (20)

/*
 * Functions below
 */

/*
 * This function accepts a filename and reads all the data from the file.
 * It dynamically allocates space for an object and stores all the data that is
 * read from the file in the object.
 */

object *read_object(const char *filename) {

  if (filename == NULL) {
    return NULL;
  }
  else {
    object *new_obj = (object *)malloc(sizeof(object));
    FILE *file = fopen(filename, "r");

    char *name = (char *)malloc(sizeof(char) * BUFFER_SIZE);
    int num_points = 0;
    int num_polys = 0;

    // Read name from file

    fscanf(file, "%s", name);
    free(name);
    name = NULL;

    // Read the number of points and polygons and store in object

    fscanf(file, "%d %d\n", &num_points, &num_polys);
    new_obj->num_points = num_points;
    new_obj->num_polygons = num_polys;

    // Allocate space for points and polygons

    new_obj->points = (point *)malloc(sizeof(point) * num_points);
    new_obj->polygons = (polygon *)malloc(sizeof(polygon) * num_polys);

    // Read all the point
    
    for (int i = 0, ret_val = VALID; i < num_points; i++) {
      int point_num = 0;
      ret_val = fscanf(file, "%d %lf %lf %lf\n",
                          &point_num, &(new_obj->points[i].arr[ZER_IND]),
                          &(new_obj->points[i].arr[FIR_IND]),
                          &(new_obj->points[i].arr[SEC_IND]));

      if (ret_val == EOF) {
        break;
      }
    }

    // Read all the polygons
    // Allocate space for the array of point pointer in the polygon member

    for (int i = 0, ret_val = VALID; (i < num_polys) && (ret_val != EOF); i++) {
      char *color = (char *)malloc(sizeof(char) * BUFFER_SIZE);
      char *pname = (char *)malloc(sizeof(char) * BUFFER_SIZE);
      int num_pts = 0;

      ret_val = fscanf(file, "%[^ ] ", pname);
      free(pname);
      pname = NULL;

      ret_val = fscanf(file, "%[^ ] ", color);
      name_to_rgb(color,
                      &(new_obj->polygons[i].r),
                      &(new_obj->polygons[i].g),
                      &(new_obj->polygons[i].b));
      free(color);
      color = NULL;

      ret_val = fscanf(file, "%d ", &num_pts);
      new_obj->polygons[i].num_points = num_pts;

      // Read the points of the polygon

      new_obj->polygons[i].point_arr =
                    (point **)malloc(sizeof(point) * num_pts);
      int pt_ind = 0;
      for (int j = 0; j < num_pts; j++) {
        if (j < (num_pts - 1)) {
          ret_val = fscanf(file, "%d ", &pt_ind);
        }
        else {
          ret_val = fscanf(file, "%d\n", &pt_ind);
        }

        new_obj->polygons[i].point_arr[j] = &(new_obj->points[pt_ind]);
      }
    }

    fclose(file);
    file = NULL;

    return new_obj;
  }
}/*read_object()*/

/*
 * This function accepts a object pointer.
 * It frees all the contents of the object.
 */

void free_object(object *var) {

  if (var == NULL) {
    return;
  }
  else {
    free(var->polygons);
    var->polygons = NULL;

    free(var->points);
    var->points = NULL;

    free(var);
    var = NULL;
  }

  return;
}/*free_object()*/
