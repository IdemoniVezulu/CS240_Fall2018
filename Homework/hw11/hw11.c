/*
 * Name, hw11.c, CS 24000, Fall 2018
 * Last updated November 12, 2018
 */

/*
 * #Includes below
 */

#include "hw11.h"

#include <malloc_debug.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
 * #Defines Below
 */

#define SMALLER (-1)
#define EQUAL (0)
#define GREATER (1)
#define NOT_FOUND (-1)
#define FOUND (1)

/*
 * Global variables below
 */

//struct node *g_occurences = NULL;

/////////////////////////////////////////////////
//  TREE  FUNCTIONS
/////////////////////////////////////////////////

/*
 *  This function prints out the values stored in a tree given
 *  a pointer to the tree's root node.
 */

void print_tree(struct node *root) {
  if (!root) {
    return;
  }

  print_tree(root->left);
  print_node(root);
  print_tree(root->right);
}/*print_tree()*/

/*
 * This function accepts a pointer to pointer that points to the root node of a
 * tree.
 * It iterates through the tree deletes the entire tree.
 */

void delete_tree(struct node **root) {
  assert(root != NULL);

  if ((*root) == NULL) {
    return;
  }
  else {

    // Iterate through the node in postfix order (L-R-N)

    if ((*root)->left != NULL) {
      delete_tree(&(*root)->left);
    }

    if ((*root)->right != NULL) {
      delete_tree(&(*root)->right);
    }

    delete_node(&(*root));
    //free(*root);
  }

  *root = NULL;

  return;
}/*delete_tree()*/

/////////////////////////////////////////////////
//  GENERIC  NODE  FUNCTIONS
/////////////////////////////////////////////////

/*
 *  This function prints the values associated with a node's data.
 */

void print_node(struct node *n_ptr) {
  assert(n_ptr && n_ptr->data && n_ptr->print);

  (n_ptr->print)(n_ptr->data);
}/*print_node()*/

/*
 * This function accepts a pointer to a pointer that points to a tree node and
 * various other pointers.
 * It allocates space for the node that is passed and copies the data from the
 * other arguments into it.
 */

void create_node(struct node **node_ptr, void *data_struct,
                  void (*print_fn)(void *), void (*delete_fn)(void **),
                  int (*compare_fn)(void *, void *)) {
  assert((node_ptr != NULL) && (data_struct != NULL) && (print_fn != NULL) &&
         (delete_fn != NULL) && (compare_fn != NULL));
  assert((*node_ptr) == NULL);

  // Allocate space for the node

  struct node *new_node = (struct node *)malloc(sizeof(struct node));
  assert(new_node != NULL);

  // Store data in node's members

  new_node->data = data_struct;

  new_node->print = print_fn;

  new_node->delete = delete_fn;

  new_node->compare = compare_fn;

  // Set node pointers to NULL

  new_node->left = NULL;
  new_node->right = NULL;

  *node_ptr = new_node;

  return;
}/*create_node()*/

/*
 * This function accepts a node.
 * It then deletes the node using the function pointer that is a member of the
 * node.
 */

void delete_node(struct node **node) {
  assert((node != NULL) && ((*node) != NULL));
  assert(((*node)->left == NULL) && ((*node)->right == NULL));

  // Call the delete function member

  ((*node)->delete)(&((*node)->data));
  free((*node));
  (*node) = NULL;

  return;
}/*delete_node()*/

/*
 * This function accepts a pointer to a pointer that points to the root of a
 * tree, and a new node.
 * The new node is then inserted into the tree with the help of the compare
 * function of the nodes.
 */

void insert_node(struct node **root, struct node *new_node) {
  assert((root != NULL) && ((new_node) != NULL));

  // Check if the new node is smaller or bigger than the root element and
  // insert the new node in the appropriate place

  if ((*root) == NULL) {
    (*root) = new_node;
    return;
  }

  if (((*root)->data == NULL) || (new_node->data == NULL) ||
      ((*root)->compare == NULL)) {
    return;
  }


  if ((((*root)->compare)((new_node->data), ((*root)->data))
            == SMALLER) ||
      (((*root)->compare)((new_node->data), ((*root)->data))
            == EQUAL)) {

    // Call insert function again if the left leaf is not empty otherwise
    // Store the node in the left leaf if it is empty

    if ((*root)->left != NULL) {

      insert_node(&(*root)->left, new_node);
    }
    else {

      (*root)->left = new_node;
    }
  }
  else {

    // Call insert function again if the right leaf is not empty otherwise
    // Store the node in the right leaf if it is not empty

    if ((*root)->right != NULL) {

      insert_node(&(*root)->right, new_node);
    }
    else {

      (*root)->right = new_node;
    }
  }

  return;
}/*insert_node()*/

/*
 * This function accepts a pointer to a root node of a tree and data.
 * It counts the number of times a given value occurs in a tree.
 * It increments g_count if the data is found in the tree
 */

int count_nodes(struct node *root, void *data_struct) {

  // Traverse entire tree to find the nodes that are equal

  int count = 0;

  if (root->compare(data_struct, root->data) == EQUAL) {
    count++;
  }

  if (root->left != NULL) {
    count += count_nodes(root->left, data_struct);
  }

  if (root->right != NULL) {
    count += count_nodes(root->right, data_struct);
  }

  return count;
}/*count()*/

/*
 * This function accepts a struct node pointer, and a pointer to an array and
 * populates the pointer with the node's address
 */

void populate_array(struct node *node, void *data,
                        struct node **array, int *index) {

  if (node->compare(data, node->data) == EQUAL) {
    array[(*index)++] = node;
  }

  if (node->left != NULL) {
    populate_array(node->left, data, array, index);
  }

  if (node->right != NULL) {
    populate_array(node->right, data, array, index);
  }

  return;
}/*populate_array()*/

/*
 * This function accepts a pointer to a struct node root, a data structure, and
 * an interger pointer.
 * It traverses through a tree to find all the occurences of the data and stores
 * the addresses of the node that contains the data in an array of pointers.
 * It returns the array of pointers.
 */

struct node **find_nodes(struct node *root, void *data_struct, int *num_dup) {
  assert((root != NULL) && (data_struct != NULL) && (num_dup != NULL));

  int count = 0;

  // Count the number of occurences of the data_struct in the tree

  count = count_nodes(root, data_struct);
  *num_dup = count;

  // Check if the data_struct is in the tree

  if (count == 0) {
    return NULL;
  }

  // Allocate memeory for array it is in the tree

  struct node **occurences =
              (struct node **)malloc(sizeof(struct node *) * count);
  struct node **head = occurences;

  // Store the address of all the node's that contain the data_struct in the
  // array

  int index = 0;
  populate_array(root, data_struct, occurences, &index);

  return head;
}/*find_nodes()*/

/*
 * This function accepts a struct node pointer.
 * It traverses the tree to find the least value of that subtree.
 * It returns the node that has the least value
 */

struct node *find_minimum(struct node *node) {

  while(node->left != NULL) {
    node = node->left;
  }

  return node;

}/*find_minimum()*/

/*
 * This function accepts a pointer to the root of a tree and a node that is to
 * be deleted from the tree.
 * It traverses the tree and removes the node from it.
 */

void remove_node(struct node **node, struct node *old_node) {
  assert((node != NULL) && (old_node != NULL));

  // Check for empty tree

  if (*node == NULL) {
    return;
  }

/*
  if ((*node)->compare(old_node->data, (*node)->data) == SMALLER) {
    remove_node(&((*node)->left), old_node);
  }
  else if ((*node)->compare(old_node->data, (*node)->data) == GREATER) {
    remove_node(&((*node)->right), old_node);
  }
  else {

    struct node *current = *node;

    // Check the number of children the node has

    if ((current->left == NULL) && (current->right == NULL)) {

      // Node has no children

      current = NULL;
      *node = NULL;
    }
    else if (current->left == NULL) {
      remove_node(&(current->right), old_node);
    }
    else if (current->right == NULL) {
      remove_node(&(current->left), old_node);
    }
    else {
      struct node *temp = find_minimum(current->right);
      current->data = temp->data;
      remove_node(&(current->right), old_node);
    }
  }
  */

  return;
}/*remove_node()*/

/////////////////////////////////////////////////
//  STRUCT  PERSON  FUNCTIONS
/////////////////////////////////////////////////

/*
 *  This function prints the values associated with a person.
 */

void print_person_data(void *person) {
  struct person *person_ptr = (struct person *) person;

  assert(person_ptr && person_ptr->first_name &&
         person_ptr->last_name && person_ptr->occupation);

  printf("%15s %15s %15s %4d %9d\n",
         person_ptr->first_name, person_ptr->last_name, person_ptr->occupation,
         person_ptr->age, person_ptr->ssn);
}/*print_person_data()*/

/*
 * This function accepts a pointer to pointer that points to void, three
 * char pointers, the age, and the ssn.
 * It then creates and allocates space for a struct person node and stores the
 * data that is passed as an argument.
 */

void create_person_data(void **node, const char *f_name,
                        const char *l_name, const char *occupation, int age,
                        int ssn) {
  assert((node != NULL) && (f_name != NULL) &&
         (l_name != NULL) && (occupation != NULL));
  assert(*node == NULL);

  // Typecast and allocate space for the void pointer

  struct person *new_node = (struct person *)malloc(sizeof(struct person));
  assert(new_node != NULL);

  // Allocate space for all the pointer members

  new_node->first_name = (char *)malloc(strlen(f_name) + 1);
  assert(new_node->first_name != NULL);
  strcpy(new_node->first_name, f_name);

  new_node->last_name = (char *)malloc(strlen(l_name) + 1);
  assert(new_node->last_name != NULL);
  strcpy(new_node->last_name, l_name);

  new_node->occupation = (char *)malloc(strlen(occupation) + 1);
  assert(new_node->occupation != NULL);
  strcpy(new_node->occupation, occupation);

  new_node->age = age;
  new_node->ssn = ssn;

  *node = (void *)(new_node);

  return;
}/*create_person_data()*/

/*
 * This function accepts a pointer to pointer that points to void.
 * It typecasts the void pointer to a struct person type and deletes all of its
 * contents.
 */

void delete_person_data(void **node) {
  assert((node != NULL) && (*node != NULL));

  // Typecast the void pointer to a struct student type

  struct person *conv_node = (struct person *)(*node);

  // Free all of the memebers of the pointers

  if (conv_node->first_name != NULL) {
    free(conv_node->first_name);
  }

  if (conv_node->last_name != NULL) {
    free(conv_node->last_name);
  }

  if (conv_node->occupation != NULL) {
    free(conv_node->occupation);
  }

  free(conv_node);
  conv_node = NULL;
  *node = NULL;

  return;
}/*delete_person_data()*/

/*
 * This function accepts to nodes.
 * It compares them using the SSN fieds and returns the appropraite value;
 */

int compare_person_data(void *node_one, void *node_two) {
  assert((node_one != NULL) && (node_two != NULL));

  int comparison = SMALLER;

  struct person *first_node = (struct person *)node_one;
  struct person *second_node = (struct person *)node_two;

  if (first_node->ssn < second_node->ssn) {
    comparison = SMALLER;
  }
  else if (first_node->ssn == second_node->ssn) {
    comparison = EQUAL;
  }
  else {
    comparison = GREATER;
  }

  return comparison;
}/*compare_person_data()*/

/////////////////////////////////////////////////
//  STRUCT  STUDENT  FUNCTIONS
/////////////////////////////////////////////////

/*
 *  This function prints the values associated with a student.
 */

void print_student_data(void *student) {
  struct student *student_ptr = (struct student *)student;
  assert(student_ptr && student_ptr->first_name &&
         student_ptr->last_name && student_ptr->college);

  printf("%15s %15s %15s %3.2f %9d\n",
         student_ptr->first_name, student_ptr->last_name, student_ptr->college,
         student_ptr->gpa, student_ptr->id);
}/*print_student_data()*/

/*
 * This function accepts a pointer to pointer that points to void, three
 * character pointers, a float, and an id.
 * it then typecasts the void pointer to a struct person type and creates a new
 * node using all the arguments.
 */

void create_student_data(void **node, const char *f_name,
                         const char *l_name, const char *college,
                         float n_gpa, int n_id) {
  assert((node != NULL) && (f_name != NULL) &&
         (l_name != NULL) && (college != NULL));
  assert(*node == NULL);

  // Typecast and allocate space for the void pointer

  struct student *new_node = (struct student *)malloc(sizeof(struct student));
  assert(new_node != NULL);

  // Allocate space for all the pointer members

  new_node->first_name = (char *)malloc(strlen(f_name) + 1);
  assert(new_node->first_name != NULL);
  strcpy(new_node->first_name, f_name);

  new_node->last_name = (char *)malloc(strlen(l_name) + 1);
  assert(new_node->last_name != NULL);
  strcpy(new_node->last_name, l_name);

  new_node->college = (char *)malloc(strlen(college) + 1);
  assert(new_node->college != NULL);
  strcpy(new_node->college, college);

  new_node->gpa = n_gpa;
  new_node->id = n_id;

  *node = (void *)(new_node);

  return;
}/*create_student_data()*/

/*
 * This function accepts a pointer to a pointer that points to void.
 * It then casts the pointer to void to a student struct and frees all of its
 * contents.
 */

void delete_student_data(void **node) {
  assert((node != NULL) && (*node != NULL));

  // Typecast the void pointer to a struct student type

  struct student *conv_node = (struct student *)(*node);

  // Free all of the memebers of the pointers

  if (conv_node->first_name != NULL) {
    free(conv_node->first_name);
  }

  if (conv_node->last_name != NULL) {
    free(conv_node->last_name);
  }

  if (conv_node->college != NULL) {
    free(conv_node->college);
  }

  free(conv_node);
  conv_node = NULL;
  *node = NULL;

  return;
}/*delete_student_data()*/

/*
 * This function accepts two nodes.
 * It compares their ID fields and returns the appropriate value.
 */

int compare_student_data(void *node_one, void *node_two) {
  assert((node_one != NULL) && (node_two != NULL));

  int comparison = SMALLER;

  struct student *first_node = (struct student *)node_one;
  struct student *second_node = (struct student *)node_two;

  if (first_node->id < second_node->id) {
    comparison = SMALLER;
  }
  else if (first_node->id == second_node->id) {
    comparison = EQUAL;
  }
  else {
    comparison = GREATER;
  }

  return comparison;
}/*compare_student_data()*/
