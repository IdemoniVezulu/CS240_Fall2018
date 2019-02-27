/*
 * Name, hw8.c, CS 24000, Fall 2018
 * Last updated October 18, 2018
 */

/*
 * #Includes below
 */

#include "hw8.h"

#include <malloc_debug.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*
 * #Defines below
 */



/*
 * Function definitions below
 */

/*
 * This function accepts the first name, last name, and age and creates a new node
 * of a single list using these values.
 * It returns the address of the new node.
 */

single_list *create_single(char *first_name, char *last_name, int age) {
  assert((first_name != NULL) && (last_name != NULL));

  // Allocate memory for the new single list node

  struct single_list *new_node =
        (struct single_list *)malloc(sizeof(struct single_list));
  assert(new_node != NULL);

  // Allocate memory for the members of the new node and assign values to them
  // using the arguments of the function

  new_node->first_name = (char *)malloc((strlen(first_name) + 1) * sizeof(char));
  assert(new_node->first_name != NULL);
  strcpy(new_node->first_name, first_name);

  new_node->last_name = (char *)malloc((strlen(last_name) + 1) * sizeof(char));
  assert(new_node->last_name != NULL);
  strcpy(new_node->last_name, last_name);

  new_node->age = age;

  new_node->next_ptr = NULL;

  return new_node;
}/*create_single()*/

/*
 * This function accepts the first name, last name, and age for a new node of a
 * double list using these values.
 * It returns the address of the new node.
 */

double_list *create_double(char *first_name, char *last_name, int age) {
  assert((first_name != NULL) && (last_name != NULL));

  // Allocate memory for the new single list node

  struct double_list *new_node =
        (struct double_list *)malloc(sizeof(struct double_list));
  assert(new_node != NULL);

  // Allocate memory for the members of the new node and assign values to them
  // using the arguments of the function

  new_node->first_name = (char *)malloc((strlen(first_name) + 1) * sizeof(char));
  assert(new_node->first_name != NULL);
  strcpy(new_node->first_name, first_name);

  new_node->last_name = (char *)malloc((strlen(last_name) + 1) * sizeof(char));
  assert(new_node->last_name != NULL);
  strcpy(new_node->last_name, last_name);

  new_node->age = age;

  new_node->next_ptr = NULL;
  new_node->prev_ptr = NULL;

  return new_node;
}/*create_double()*/

/*
 * This function accepts a pointer to a single list and a solitary signle list.
 * It then appends the second argument to the first list.
 */

void append_single(single_list *node_at, single_list *node_add) {
  assert((node_at != NULL) && (node_add != NULL) && (node_add->next_ptr == NULL));

  // Temp stores the next node pointed to by the first argument
  // Then, the second argument is made as the next node to the first
  // Finally, the value in temp is stored as the value after the new node is added

  struct single_list *temp = node_at->next_ptr;
  node_at->next_ptr = node_add;
  node_add->next_ptr = temp;

  return;
}/*append_single()*/

/*
 * This function takes in two double_list nodes and inserts the second argument
 * after the first.
 */

void append_double(double_list *node_at, double_list *node_add) {
  assert((node_at != NULL) && (node_add != NULL) && (node_add->next_ptr == NULL));

  // Temp creates a temporary double_list node to insert the new node.
  // Then, the first argument's next_ptr is made to point to the new node.
  // The new node's node pointers are made to point to their respective nodes.
  // Finally, the temporary node's prev_ptr is made to point to the new node.

  struct double_list *temp = node_at->next_ptr;

  node_at->next_ptr = node_add;
  node_add->prev_ptr = node_at;
  node_add->next_ptr = temp;
  if (temp != NULL) {
    temp->prev_ptr = node_add;
  }

  return;
}/*append_double()*/

/*
 * This function accepts a pointer to a single_list and converts the list into a
 * double_list.
 * It returns the head of the new double_list.
 */

double_list *single_to_double(single_list *node) {
  
  struct double_list *new_node = NULL;

  if (node == NULL) {
    return new_node;
  }
  else {
    struct single_list *temp = node;
    new_node = create_double(temp->first_name, temp->last_name, temp->age);
    new_node->prev_ptr = NULL;

    // Make the head of the new double list

    //struct double_list *head = new_node;

    // Store the head in a variable to iterate through the list

    struct double_list *current = new_node;

    // Create the second element of the double list

    if (temp->next_ptr != NULL) {
      temp = temp->next_ptr;
    }
    else {
      return new_node;
    }

    current->next_ptr =
      create_double(temp->first_name, temp->last_name, temp->age);
    struct double_list *next = current->next_ptr;
    append_double(current, next);

    while (temp != NULL) {
      current = next;
      if (temp->next_ptr != NULL) {
        temp = temp->next_ptr;
      }
      else {
        break;
      }
      if (current != NULL) {
        current->next_ptr =
            create_double(temp->first_name, temp->last_name, temp->age);
        next = current->next_ptr;
        append_double(current, next);
      }
      else {
        break;
      }
    }

    current->next_ptr = NULL;
  }

  return new_node;
}/*single_to_double()*/

/*
 * This function accepts a node and deallocates the node's pointer members and
 * the node.
 */

void delete_single(single_list *node) {
  assert(node != NULL);

  // Free the node's string pointers and then the node itself

  free(node->first_name);
  free(node->last_name);
  free(node);

  return;
}/*delete_single()*/

/*
 * This function accepts a node of a double_list and delete the data that's its
 * string pointers point to and then the node itself.
 */

void delete_double(double_list *node) {
  assert(node != NULL);

  // Free the node's string pointers and then the node itself

  free(node->first_name);
  free(node->last_name);
  free(node);

  return;
}/*delete_double()*/

/*
 * This function takes in a node of a single_list and deletes the linked list b
 * calling delete_single().
 */

void delete_single_list(single_list *head) {

  // Function does nothing if the argument is NULL

  if (head == NULL) {
    return;
  }
  else {

    // Loop iterates through the single_list and deletes all the nodes

    while (head != NULL) {
      struct single_list *next = head->next_ptr;
      delete_single(head);
      head = next;
    }

    //head = NULL;
  }

  return;
}/*delete_single_list()*/

/*
 * This function accepts a node to a double_list and deletes all the nodes of
 * the linked list.
 */

void delete_double_list(double_list *head) {

  // Function does nothing if the argument passed is NULL

  if (head == NULL) {
    return;
  }
  else {

    // Loop iterates through the double_list and deletes all the nodes

    while (head != NULL) {
      struct double_list *next = head->next_ptr;
      delete_double(head);
      head = next;
    }
  }

  return;
}/*delete_double()*/
