#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "constraints.h"

/* Create and initialize a constraints struct.
 * Sets the fields to 0 or empty string.
 * Return a pointer to the newly created constraints struct.
 */
struct constraints *init_constraints() {
    struct constraints *constr = malloc(sizeof(struct constraints));

    for (int i = 0; i<WORDLEN ; i++){
      for (int j = 0; j< SIZE; j++){
        constr->must_be[i][j] = '';
      }
    }

    for(int i = 0; i < ALPHABET_SIZE; i++){
      constr->cannot_be[i] = '';
    }

    return constr;
}

/* Update the "must_be" field at "index" to be a string
 * containing "letter"
 * The tile at this index is green, therefore the letter at "index"
 * must be "letter"
 */
void set_green(char letter, int index, struct constraints *con) {
    assert(islower(letter));
    assert(index >= 0 && index < SIZE);

    con->must_be[index] = letter;
}

/* Update "con" by adding the possible letters to the string at the must_be
 * field for "index".
 * - index is the index of the yellow tile in the current row to be updated
 * - cur_tiles is the tiles of this row
 * - next_tiles is the tiles of the row that is one closer to the solution row
 * - word is the word in the next row (assume word is all lower case letters)
 * Assume cur_tiles and next_tiles contain valid characters ('-', 'y', 'g')
 *
 * Add to the must_be list for this index the letters that are green in the
 * next_tiles, but not green in the cur_tiles or green or yellow in the
 * next_tiles at index.
 * Also add letters in yellow tiles in next_tiles.
 */
void set_yellow(int index, char *cur_tiles, char *next_tiles,
                char *word, struct constraints *con) {

    assert(index >=0 && index < SIZE);
    assert(strlen(cur_tiles) == WORDLEN);
    assert(strlen(next_tiles) == WORDLEN);
    assert(strlen(word) == WORDLEN);

    for (int i =0; i < WORDLEN; i++){
      if (next_tiles[i] == 'g' && cur_tiles[i] != 'g' ){
        if (next_tiles[index] != 'g' or next_tiles[index] != 'y'){
          con->must_be[index] = word[i];
        }
      }
      if (next_tiles[i] == 'y'){
        con->must_be[index] = word[i];
      }
    }


}

/* Add the letters from cur_word to the cannot_be field.
 * See the comments in constraints.h for how cannot_be is structured.
 */
void add_to_cannot_be(char *cur_word, struct constraints *con) {
    assert(strlen(cur_word) <= WORDLEN);

    char *tempChar = cur_word;

    while (*tempChar != '\0'){
      con->cannot_be[*tempChar - 'a'] = 1;
    }
}

void print_constraints(struct constraints *c) {
    printf("cannot_be: ");

    // TODO

    printf("\nmust_be\n");

    // TODO

    printf("\n");
}

/* Free all dynamically allocated memory pointed to by c
 */
void free_constraints(struct constraints *c) {
    free(c);
}
