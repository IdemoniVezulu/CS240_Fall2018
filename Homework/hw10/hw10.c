/*
 * Name, hw10.c, CS 24000, Fall 2018
 * Last updated November 1, 2018
 */

/*
 * #Includes below
 */

#include "hw10.h"

#include <malloc_debug.h>
#include <assert.h>
#include <string.h>

/*
 * #Defines below
 */

#define EQUAL (0)

/*
 * Function definitions below
 */

/*
 * This function accepts the name, title, and id as arguments and
 * creates a new tree node using the values in the argument.
 * It returns the address of the new node
 */

tree *tree_create(char *name, char *title, unsigned int id) {
  assert((name != NULL) && (title != NULL));

  // Allocate space for the new tree node

  tree* new_node = (tree *)malloc(sizeof(tree));
  assert(new_node != NULL);

  // Allocate space and assign values to the new node's name

  new_node->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
  assert(new_node->name != NULL);
  strcpy(new_node->name, name);

  // Allocate space and assign values to the new node's title

  new_node->title = (char *)malloc((strlen(title) + 1) * sizeof(char));
  assert(new_node->title != NULL);
  strcpy(new_node->title, title);

  new_node->id_number = id;

  // Make the new node a solitary node (not point to anything)

  new_node->left_ptr = NULL;
  new_node->right_ptr = NULL;

  return new_node;
}/*tree_create()*/

/*
 * This function accepts a pointer to a pointer that points to a tree's root and
 * the new node to be inserted. It inserts the new node at it's respective
 * position.
 * It returns nothing.
 */

void tree_insert(tree **root, tree *new_node) {
  assert((root != NULL) && (new_node != NULL));
  assert((new_node->left_ptr == NULL) && (new_node->right_ptr == NULL));

  // Make the new node as the tree root if it is an empty tree

  if (*root == NULL) {
    *root = new_node;
  }
  else {

    // Check the value to be inserted with the leaf's value and proceed to the
    // left or right leaf accordingly

    if (strcmp(new_node->name, (*root)->name) <= EQUAL) {
      tree_insert(&(*root)->left_ptr, new_node);
    }
    else {
      tree_insert(&(*root)->right_ptr, new_node);
    }
  }

  return;
}/*tree_insert()*/

/*
 * This function accepts a pointer to a tree's root and the value to be searched
 * for. If the root's name is smaller than the second argument, it proceeds to
 * the node pointed to by the root's left pointer. If it is larger, it proceeds
 * to the node pointed to by right pointer.
 * It returns the root node when the name and value are equal.
 */

tree *tree_find(tree *root, char *value) {
  assert(value != NULL);

  // If the root of the tree is NULL or if the tree's name is the value to be
  // found, return the root

  if ((root == NULL) || (strcmp(root->name, value) == EQUAL)) {
    return root;
  }

  // Check if the value in the argument is smaller or bigger than the current
  // leaf. Proceed to the left leaf if it is smaller, otherwise proceed to the
  // right node

  if (strcmp(value, root->name) < EQUAL) {
    return tree_find(root->left_ptr, value);
  }
  else {
    return tree_find(root->right_ptr, value);
  }
}/*tree_find()*/

/*
 * This function accepts a pointer to a pointer that points to a root.
 * It iterates through the tree and deletes the node and all of the its
 * contents.
 */

void tree_delete(tree **root) {
  assert(root != NULL);

  // Return if the root of the tree is NULL

  if (*root == NULL) {
    return;
  }
  else {

    // Traverse the tree using postfix (L-R-N) and delete its contents

    if ((*root)->left_ptr != NULL) {
      tree_delete(&(*root)->left_ptr);
    }

    if ((*root)->right_ptr != NULL) {
      tree_delete(&(*root)->right_ptr);
    }

    free((*root)->name);
    free((*root)->title);
    free(*root);

  }

  *root = NULL;

  return;
}/*tree_delete()*/
