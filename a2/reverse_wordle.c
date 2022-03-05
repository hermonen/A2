#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordle.h"
#include "constraints.h"

/* Read the wordle grid and solution from fp.
 * Return a pointer to a wordle struct.
 * See sample files for the format. Assume the input file has the correct
 * format.  In other words, the word on each is the correct length, the
 * words are lower-case letters, and the line ending is either '\n' (Linux,
 * Mac, WSL) or '\r\n' (Windows)
 */
struct wordle *create_wordle(FILE *fp) {
    struct wordle *w = malloc(sizeof(struct wordle));
    char line[MAXLINE];
    w->num_rows = 0;

    while(fgets(line, MAXLINE, fp ) != NULL) {

        // remove the newline character(s)
        char *ptr;
        if(((ptr = strchr(line, '\r')) != NULL) ||
           ((ptr = strchr(line, '\n')) != NULL)) {
            *ptr = '\0';
        }

        strncpy(w->grid[w->num_rows], line, SIZE);
        w->grid[w->num_rows][SIZE - 1] = '\0';
        w->num_rows++;
    }
    return w;
}


/* Create a solver_node and return it.
 * If con is not NULL, copy con into dynamically allocated space in the struct
 * If con is NULL set the new solver_node con field to NULL.
 * Tip: struct assignment makes copying con a one-line statements
 */
struct solver_node *create_solver_node(struct constraints *con, char *word) {
    struct solver_node* solveNode = (struct solver_node*)malloc(sizeof(struct solver_node));
    strncpy(solveNode->word, word, SIZE);

    solveNode->next_sibling = NULL;
    solveNode->child_list = NULL;

    if (con == NULL){
      solveNode->con = NULL;
    }
    else{
      solveNode->con = con;
    }
    return solveNode;
}

/* Return 1 if "word" matches the constraints in "con" for the wordle "w".
 * Return 0 if it does not match
 */
int match_constraints(char *word, struct constraints *con,
struct wordle *w, int row) {

    for (i =0; i < WORDLEN; i++){
      if (word[i] != con->must_be[i]){
        return 0;
      }
      if (con->must_be[i] == '' && con->cannot_be[word[i] - 'a'] ==1 ){
        return 0;
      }
    }
    return 1;
}

/* remove "letter" from "word"
 * "word" remains the same if "letter" is not in "word"
 */
void remove_char(char *word, char letter) {
    char *ptr = strchr(word, letter);
    if(ptr != NULL) {
        *ptr = word[strlen(word) - 1];
        word[strlen(word) - 1] = '\0';
    }
}

/* Build a tree starting at "row" in the wordle "w".
 * Use the "parent" constraints to set up the constraints for this node
 * of the tree
 * For each word in "dict",
 *    - if a word matches the constraints, then
 *        - create a copy of the constraints for the child node and update
 *          the constraints with the new information.
 *        - add the word to the child_list of the current solver node
 *        - call solve_subtree on newly created subtree
 */

void solve_subtree(int row, struct wordle *w,  struct node *dict,
                   struct solver_node *parent) {


    if (row == MAX_GUESSES){
      return;
    }

    if(verbose) {
        printf("Running solve_subtree: %d, %s\n", row, parent->word);
    }

    struct node *dictptr = dict;

    while(dictptr != NULL) {
      if (match_constraints(dictptr->word, parent->con ,w, row) == 1){
        struct constraints* constra = init_constraints();

        for (int i = 0 ; i < WORDLEN; i++){
          if (w[row][i] == 'g'){
            set_green(dictptr->word[i], i, constra);
          }
          set_yellow(i, w[row + 1], w[row], dictptr->word, constra);
          add_to_cannot_be(dictptr->word[i], constra);
        }

        struct solver_node* solved = create_solver_node(constra, dictptr->word);
        if (parent->child_list == NULL){
          parent->child_list = solved;
        }
        else{
          parent->child_list->next_sibling = solved;
        }
        solve_subtree(row + 1, w, dict, solved);
      }

      dictptr = dictptr->next;
    }



}

/* Print to standard output all paths that are num_rows in length.
 * - node is the current node for processing
 * - path is used to hold the words on the path while traversing the tree.
 * - level is the current length of the path so far.
 * - num_rows is the full length of the paths to print
 */

void print_paths(struct solver_node *node, char **path,
                 int level, int num_rows) {

    //TODO

}

/* Free all dynamically allocated memory pointed to from w.
 */
void free_wordle(struct wordle *w){
    free(w);
}

/* Free all dynamically allocated pointed to from node
 */
void free_tree(struct solver_node *node){
  struct solver_node *freeptr = node;

  if (node->child_list != NULL){
    free_tree(node->child_list);
  }
  else if(node->next_sibling != NULL){
    free_tree(node->next_sibling);
  }

  free_constraints(node->con);
  free(node);
}
