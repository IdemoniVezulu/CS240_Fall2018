/*
 * Name, hw9.c, CS 24000, Fall 2018
 * Last updated October 25, 2018
 */

/*
 * Include files below
 */

#include "hw9.h"

#include <malloc_debug.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

/*
 * #Defines below
 */

#define VALID (1)
#define INVALID (-1)
#define EQUAL (0)

/*
 * Function definitions below
 */

/*
 * This function accepts a pointer to a double_list pointer, the firstname,
 * lastname and the age to be stored in the newly created double.
 */

void create_double (double_list **node, char *fname, char *lname, int age) {
  assert((node != NULL) && (fname != NULL) && (lname != NULL));

  // Allocate memory for the new single list node

  double_list *new_node =
      (struct double_list *)malloc(sizeof(struct double_list));
  assert(new_node != NULL);

  // Allocate memory for the members of the new node and assign
  // values to them
  // using the arguments of the function

  new_node->first_name = (char *)malloc((strlen(fname) + 1) * sizeof(char));
  assert(new_node->first_name != NULL);
  strcpy(new_node->first_name, fname);

  new_node->last_name = (char*)malloc((strlen(lname) + 1) * sizeof(char));
  assert(new_node->last_name != NULL);
  strcpy(new_node->last_name, lname);

  new_node->age = age;

  new_node->next_ptr = NULL;
  new_node->prev_ptr = NULL;

  // Make the node that is passed point to the new node

  *node = new_node;

  return;
}/*create_double*/

/*
 * This function accepts a node to a list and a new node to be appended to that
 * list. It appends the new node right after the node that is passed as the
 * first argument
 */

void append_double (double_list **list_node, double_list *node_add) {
  assert((list_node != NULL) && (node_add != NULL));
  assert((node_add->prev_ptr == NULL) && (node_add->next_ptr == NULL));

  if (*list_node == NULL) {
    *list_node = node_add;
    return;
  }
  else {

    // Temp creates a temporary double_list node to insert the new node.
    // Then, the first argument's next_ptr is made to point to the new node.
    // The new node's node pointers are made to point to their respective nodes.
    // Finally, the temporary node's prev_ptr is made to point to the new node.

    struct double_list *node_at = *list_node;
    struct double_list *temp = node_at->next_ptr;

    node_at->next_ptr = node_add;
    node_add->prev_ptr = node_at;
    node_add->next_ptr = temp;
    if (temp != NULL) {
      temp->prev_ptr = node_add;
    }

    *list_node = node_add;
  }

  return;
}/*append_double*/

/*
 * This function is simillar to append_double except that it traverses the list
 * and puts the new element into the list in a sorted manner.
 * It is sorted in ascending order of lastname with firstname used to break
 * ties.
 */

void insert_in_order (double_list **list_node, double_list *new_node) {
  assert((list_node != NULL) && (new_node != NULL));
  assert((new_node->prev_ptr == NULL) && (new_node->next_ptr == NULL));

  if (*list_node == NULL) {
    *list_node = new_node;
    return;
  }

  // Loop iterates through the list to find the head

  double_list *temp = *list_node;

  while (temp->prev_ptr != NULL) {
    temp = temp->prev_ptr;
  }

  // Loop traverses the list to find the position to insert the new node

  while (temp->next_ptr != NULL) {
    int found = INVALID;
    int compare = strcmp(new_node->last_name, temp->last_name);

    if (compare != EQUAL) {

      // Unequal last names

      while ((temp->next_ptr != NULL) && (compare < EQUAL)) {
        temp = temp->next_ptr;
        compare = strcmp(new_node->last_name, temp->last_name);
      }

      temp = temp->prev_ptr;

      new_node->next_ptr = temp;
      new_node->prev_ptr = temp->prev_ptr;
      temp->prev_ptr = new_node;

      found = VALID;
    }
    else {

      // Equal last names

      int compare_first = strcmp(new_node->first_name, temp->first_name);

      while((temp->next_ptr != NULL) && (compare_first < EQUAL) &&
            (compare == EQUAL)) {
        temp = temp->next_ptr;
        compare = strcmp(new_node->last_name, temp->last_name);
        compare_first = strcmp(new_node->first_name, temp->first_name);
      }

      temp = temp->prev_ptr;

      new_node->prev_ptr = temp->prev_ptr;
      new_node->next_ptr = temp;
      temp->prev_ptr = new_node;

      found = VALID;
    }

    compare = strcmp(new_node->last_name, temp->last_name);

    if (found == VALID) {
      break;
    }
  }

  *list_node = new_node;

  return;
}/*insert_in_order*/

/*
 * This function accepts a pointer to a double_list pointer and a node to be
 * deleted. After deletion, the pointer that the first argument points to is
 * made to point to either the one before/after the node that was removed. If it
 * is a solitary node, then it points to NULL
 */

void remove_double (double_list **list_node, double_list *old_node) {
  assert((list_node != NULL) && (old_node != NULL));

  double_list *old_before = NULL;
  double_list *old_after = NULL;

  if (old_node->prev_ptr != NULL) {
    old_before = old_node->prev_ptr;

    *list_node = old_before;
  }

  if (old_node->next_ptr != NULL) {
    old_after = old_node->next_ptr;

    if (old_before == NULL) {
      *list_node = old_after;
    }
  }

  if ((old_before == NULL) && (old_after == NULL)) {
    *list_node = NULL;
  }

  if (old_before != NULL) {
    old_before->next_ptr = old_node->next_ptr;
  }

  if (old_after != NULL) {
    old_after->prev_ptr = old_node->prev_ptr;
  }

  old_node->next_ptr = NULL;
  old_node->prev_ptr = NULL;

  return;
}/*remove_double*/

/*
 * This function deletes the node that is pointed to by the pointer of the
 * argument. It also deletes the strings that are pointed to by it's fields.
 */

void delete_double (double_list **list_node) {
  assert((list_node != NULL) && (*list_node != NULL));
  assert(((*list_node)->prev_ptr == NULL) && ((*list_node)->next_ptr == NULL));

  // Free the contents of the node and the node itself

  free((*list_node)->first_name);
  free((*list_node)->last_name);
  free((*list_node));

  // Set the pointer that points to the structure to NULL

  *list_node = NULL;

  return;
}/*delete_double*/

/*
 * This function traverses the list pointed to by the first argument and deletes
 * all of it's nodes.
 */

void delete_double_list (double_list **list_node) {

  // Return if the argument is NULL

  if ((list_node == NULL) || (*list_node == NULL)) {
    return;
  }
  else {
    double_list *head = *list_node;

    // Loop iterates through the list to find the head

    while (head->prev_ptr != NULL) {
      head = head->prev_ptr;
    }

   // Loop traverses the list to delete all of its nodes.

    while (head != NULL) {
      double_list *next = head->next_ptr;

      head->next_ptr = NULL;
      head->prev_ptr = NULL;
      delete_double(&head);
      head = next;
    }
  }

  *list_node = NULL;

  return;
}/*delete_double_list*/
