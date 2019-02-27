/*
 * Name, hw7.c, CS 24000, Fall 2018
 * Last updated October 11, 2018
 */

/*
 * Include files below
 */

#include "hw7.h"

#include <malloc_debug.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

/*
 * #Defines below
 */

#define CONTROL_CHAR (32)
#define TERMINATE ('\0')
#define MAX_LENGTH (100)

/*
 * Function definitions below
 */

/*
 * This function accepts a node pointer as an argument and calculates the size
 * of that list.
 * It returns the size.
 */

size_t find_size(struct node *list) {
  struct node *temp = list;
  size_t count = 0;

  // Loop iterates through the list to find the size

  while (temp != NULL) {
    count++;
    temp = temp->next;
  }

  return count;
}/*find_size()*/

/*
 * This function takes the string that is the first argument into a linked list,
 * if the second argument is DUPLICATE, the list is converted into a string. If
 * it is REVERSE, it is converted into a string in reverse order.
 * Once the string is created, it deletes the list.
 * It returns a pointer to the new string.
 */

char *copy_string(char *string, int operation) {
  assert((string != NULL) && (*string != TERMINATE));
  assert ((operation == DUPLICATE) || (operation == REVERSE));

  char *new_string = NULL;

  if (operation == DUPLICATE) {
    struct node *list = string_to_list(string, NO);

    new_string = list_to_string(list);

    free_list(list);
  }
  else if (operation == REVERSE) {
    struct node *list = string_to_list(string, YES);

    new_string = list_to_string(list);

    free_list(list);
  }

  return new_string;
}/*copy_string()*/

/*
 * This function accepts a pointer to a string and converts this string into a
 * list.
 * It returns a pointer to the head of the list
 */

struct node *string_to_list(char *string, int reverse) {
  assert((string != NULL) && (*string != TERMINATE));
  assert ((reverse == YES) || (reverse == NO));

  struct node *new_list = (struct node *)malloc(sizeof(struct node));
  assert(new_list != NULL);

  // Initialises the node's value with the first element of the array

  new_list->val = *string;
  new_list->next = NULL;

  // Increment the array to point to the second element

  string++;

  // Convert the string to a list depending on the value of reverse
  // The loop iterates through the elements of the array and either appends or
  // prepends the linked list with a new node.

  for (char *temp = string; *temp != TERMINATE; temp++) {
    if (reverse == NO) {
      append_char(new_list, *temp);
    }
    else if (reverse == YES) {
      new_list = prepend_char(new_list, *temp);
    }
  }

  return new_list;
}/*string_to_list()*/

/*
 * This function creates a new node that holds the character argument. This node
 * is then appended to the end of the list.
 * It returns a pointer to the head of the list.
 */

struct node *append_char(struct node *list, char val) {
  assert(val > CONTROL_CHAR);

  struct node *new_node = (struct node*) malloc(sizeof(struct node));
  assert(new_node != NULL);

  new_node->val = val;
  new_node->next = NULL;

  // Creates a new list if the list is NULL

  if (list == NULL) {
    list = new_node;
    return list;
  }

  // Finds the tail of list and appends it if the list exists

  struct node *temp = list;

  while (temp->next != NULL) {
    temp = temp->next;
  }

  temp->next = new_node;

  return list;
}/*append_char()*/

/*
 * This function creates a new node that holds the char in the second argument
 * as the value. This node is then prepended to the head of the list and is made
 * into the new head.
 * It returns a pointer to the new head.
 */

struct node *prepend_char(struct node *list, char val) {
  assert(val > CONTROL_CHAR);

  struct node *new_node = (struct node*)malloc(sizeof(struct node));
  assert(new_node != NULL);

  // Assign values to the new node with the function arguments

  new_node->val = val;
  new_node->next = list;

  // Make the head of the list point to the new node

  list = new_node;

  return new_node;
}/*prepend_char()*/

/*
 * This function accepts a pointer to a list head and iterates through the list
 * and converts the list to a string that is NULL terminated.
 * It returns a pointer to the newly made string.
 */

char *list_to_string(struct node *list_head) {
  assert(list_head != NULL);

  // Find the size of the list to make the string

  size_t list_count = find_size(list_head);
  struct node *temp = list_head;

  char *string = malloc((list_count + 1) * sizeof(char));
  assert(string != NULL);

  // Head holds the address of the first element of the string

  char *head = string;

  // Loop iterates through the array and stores the node's value as an element
  // of the list

  for (int i = 0; (i < list_count) && (temp != NULL); i++, string++) {
    *string = temp->val;
    temp = temp->next;
  }

  // Null terminate the string

  *string = '\0';

  return head;
}/*list_to_string()*/

/*
 * This function accepts a pointer to a list head and deallocates space for all
 * the elements of the list.
 */

void free_list(struct node *list_head) {
  struct node *temp = list_head;

  // Loop iterates through the list freeing all the nodes

  while (list_head != NULL) {
    temp = list_head;
    list_head = list_head->next;
    free(temp);
  }
}/*free_list()*/

/*
 * This function converts the two strings into two lists and then connects the
 * two lists together and then makes them into a big list to concatenate them.
 * It returns a pointer to the newly allocated string.
 */

char *concat_strings(char *string_one, char *string_two) {
  assert((string_one != NULL) && (*string_one != TERMINATE));

  // Convert the string into lists

  struct node *list_one = string_to_list(string_one, NO);
  if ((string_two != NULL) && (*string_two != TERMINATE)) {
    struct node *list_two = string_to_list(string_two, NO);

    // Append lists to make one list

    append_lists(list_one, list_two);
  }

  // Convert the big list into a big string

  char *head = list_to_string(list_one);

  free_list(list_one);

  return head;
}/*concat_string()*/

/*
 * This functinon makes the head of the first pointer point to the tail of the
 * second pointer.
 * It does not return anything
 */

void append_lists(struct node *list_one, struct node *list_two) {
  assert(list_one != NULL);

  if (list_two == NULL) {
    return;
  }

  // Loop iterates through list one to find the tail
  // When found, it makes the tail point to list two

  struct node *temp_one = list_one;

  // Loop iterates through the list to find the tail

  while (temp_one->next != NULL) {
    temp_one = temp_one->next;
  }

  temp_one->next = list_two;

  return;
}/*append_lists()*/
