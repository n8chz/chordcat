/*

 Copyleft 2012, Lorraine Lee
 All rights reversed.
 http://www.cypherspace.org/CPL/

 Calculate the number of chord forms,
 in which two chords are of one form,
 if one can be transposed to the other.

 There are 352, including the "rest."

 Then generalize to numbers of scale degrees
 other than 12.

 music theory and number theory

 This is the brute force way.

 See http://jbdowse.com/poib/all/chords
 See also http://oeis.org/A000031

*/

#include <stdio.h>
#include <stdlib.h>

#define N_SCALE_DEG 12

/*
 Use of binary tree is overkill,
 but the idea is to count each pattern once.
*/
struct tree {
 long el;
 struct tree *left, *right;
};

struct tree *insert(long el, struct tree *tree);
unsigned long enumerate(struct tree *tree);
int main(int argc, char **argv);
void erase(struct tree *tree);

struct tree *insert(long el, struct tree *tree) {
 struct tree *rv;

 if (tree) {
  if (el < tree->el) {
   tree->left = insert(el, tree->left);
  }
  else if (el > tree->el) {
   tree->right = insert(el, tree->right);
  }
  /*
   if el == tree->el
   nothing happens,
   so no pattern is counted more than once.
  */
  rv = tree;
 }
 else {
  rv = (struct tree *)malloc(sizeof(struct tree));
  rv->el = el;
  rv->left = rv->right = NULL;
 }
 return (rv);
}

/*
Count the number of unique entries.
*/
unsigned long enumerate(struct tree *tree) {
 if (tree) {
  return (1+enumerate(tree->left)+enumerate(tree->right));
 }
 else {
  return (0);
 }
}

/*
Free up the memory from a tree
that is no longer needed.
*/
void erase(struct tree *tree) {
 if (tree) {
  erase(tree->left);
  erase(tree->right);
  free(tree);
 }
}

int main(int argc, char **argv) {
 unsigned long ceiling;
 unsigned long k, q, minrot;
 unsigned short j, n;
 struct tree *tree;

 /*
  n is a number of scale degrees.
  the scale is an n-bit binary word.
  a 1 represents a played note,
  while a 0 is an unplayed note.
 */
 for (n = 0; n < 8*sizeof(long); n++) {
  ceiling = 1 << n; /* i.e. 2^n */
  tree = NULL;
  /*
   each value of k
   represents one of the 2^n
   possible combinations
   of played and unplayed notes.
  */
  for (k=0; k<ceiling; k++) {
   minrot = ceiling;
   q = k;
   /*
    each value of j
    is a rotation of k.
    whichever one has
    the smallest value
    is taken to represent
    all n of them.
   */
   for (j=0; j<n; j++) {
    if (q & 1) q |= ceiling;
    q >>= 1;
    if (q < minrot) minrot = q;
   }
   /*
    each value of minrot
    represents a unique chord pattern.
   */
   tree = insert(minrot, tree);
  }
  /*
   print one line of table,
   number of scale degrees,
   followed by number of patterns
  */
  printf("%2d %10ld\n", n, enumerate(tree));
  erase(tree);
 }
}

