#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordlist.h"

/* Read the words from a filename and return a linked list of the words.
 *   - The newline character at the end of the line must be removed from
 *     the word stored in the node.
 *   - You an assume you are working with Linux line endings ("\n").  You are
 *     welcome to also handle Window line endings ("\r\n"), but you are not
 *     required to, and we will test your code on files with Linux line endings.
 *   - The time complexity of adding one word to this list must be O(1)
 *     which means the linked list will have the words in reverse order
 *     compared to the order of the words in the file.
 *   - Do proper error checking of fopen, fclose, fgets
 */
struct node *read_list(char *filename) {
    FILE* fp = NULL;
    char lineBuffer[MAXLINE];

    struct node* head = NULL;
    struct node* previousNode = head;

    fp = fopen(filename, "r");

    if (fp == NULL){
      printf("could not open file: %s\n", filename);
      return NULL;
    }

    while(fgets(lineBuffer, MAXLINE, fp) != NULL) {
      // remove the newline
      for(int i = 0; i < MAXLINE; i++){
        if(lineBuffer[i] == '\n'){
          lineBuffer[i] = '\0';
        }
      }
      struct node* temp = (struct node*)malloc(sizeof(struct node));
      strncpy(temp->word, lineBuffer, SIZE);
      temp->next = NULL;

      if (head == NULL) {
        head = temp;
      }

      if (previousNode != NULL){
        previousNode->next = temp;
      }

      previousNode = temp;
    }

    if (fclose(fp) != 0){
      printf("could not close file: %s\n", filename);
    }

    return head;
}

/* Print the words in the linked-list list one per line
 */
void print_dictionary(struct node *list) {
    // TODO
}
/* Free all of the dynamically allocated memory in the dictionary list
 */
void free_dictionary(struct node *list) {

  struct node *freeptr = list;

  while(freeptr != NULL) {
      struct node *tempFree = freeptr->next;
      free(freeptr);
      freeptr = tempFree;
  }
}
