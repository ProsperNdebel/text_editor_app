/* COMP 211 HW 9 challenge:  Using linked lists to implement an editor buffer
 *
 * Driver program.
 *
 * N. Danner
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "editbuffer.h"

#define BUF_LEN 48

struct menu_opt {
    char* desc ;
    void (*act)(void) ;
} ;

/* Yes, I know I have said to not use global variables, except for constants.
 * But this program does in fact require a single global variable for the
 * (pointer to the) editor buffer, because it defines many different functions
 * that all need to modify it.
 *
 * But in fact, the way we use buf, it *is* actually constant, because the
 * value of buf is a heap location, and that never changes as this program
 * executes.   The contents of the heap at that location do change, but not
 * the location itself.
 *
 * We could get away without making this global if we were to define all the
 * do_ functions below locally to main.  But that is not permitted by the C
 * standard (though it is allowed by most modern compilers).
 */
buffer *buf = NULL ;

void do_exit() {
    buf_free(buf) ;
    exit(0) ;
}

void do_make_empty() {
    if (buf != NULL) free(buf) ;
    buf = buf_empty() ;
} ;

/* Get a string from the user and then call buf_insert on each of the
 * characters in that string.
 */
void do_insert() {
    char s[BUF_LEN] ;
    printf("Enter characters (max %d, ends at whitespace): ", BUF_LEN) ;
    scanf(" %s", s) ;
    for (int i=0; s[i] != '\0'; i+=1) buf_insert(buf, s[i]) ;
    return ;
}

void do_insert_newline() {
    buf_insert_newline(buf) ;
}

void do_delete_left() {
    buf_delete_left(buf) ;
} ;

void do_delete_right() {
    buf_delete_right(buf) ;
} ;

void do_move_left() {
    buf_move_left(buf) ;
} ;

void do_move_right() {
    buf_move_right(buf) ;
} ;

void do_move_up() {
    buf_move_up(buf) ;
} ;

void do_move_down() {
    buf_move_down(buf) ;
} ;

void do_set() {
    struct pos p ;

    printf("Enter row: ") ;
    scanf("%d", &(p.row)) ;
    printf("Enter column: ") ;
    scanf("%d", &(p.column)) ;
    buf_set(buf, p) ;
}

void do_print() {
    buf_print(buf) ;
}

int main(void) {


    struct menu_opt menu[] = {
        {"Exit", do_exit},
        {"Create empty buffer", do_make_empty},
        {"Insert character", do_insert},
        {"Insert newline", do_insert_newline},
        {"Delete left", do_delete_left},
        {"Delete right", do_delete_right},
        {"Move insert mark left", do_move_left},
        {"Move insert mark right", do_move_right},
        {"Move insert mark up", do_move_up},
        {"Move insert mark down", do_move_down},
        {"Set insert mark position", do_set} ,
        {"Print buffer", do_print}
    } ;

    // WARNING:  this method for computing the size of an array only works when
    // the array is declared on the stack.  In particular, it is not valid when
    // the array is allocated using malloc, calloc, etc.
    int n_menu = sizeof(menu)/sizeof(struct menu_opt) ;

    int choice ;
    // buffer_contents cnt ;

    while (true) {

        /* Prompt the user for an action and do it.
         */
        for (int i=0; i<n_menu; ++i) {
            printf("(%d) %s\n", i, menu[i].desc) ;
        }
        printf("Enter choice: ") ;
        scanf("%u", &choice) ;
        menu[choice].act() ;

        /* Print out the contents of the buffer.  The characters in the buffer
         * are printed with spaces between them.  That way we can also print
         * the insertion mark "between" two characters without messing up the
         * vertical spacing.
         */
        printf("Buffer contents:\n") ;
        int nlines = buf_get_nlines(buf) ;
        struct pos ins = buf_get_position(buf) ;


        for (int r=0; r<nlines; r+=1) {
            int line_len = buf_get_line_size(buf, r);
            char line[line_len+1] ;
            buf_get_line(buf, r, line) ;

            /* If this isn't the row with the insert mark, print the characters
             * separated with spaces.
             */
            if (r != ins.row) {
                for (int i=0; line[i] != '\0'; i+=1) printf(" %c", line[i]) ;
            }

            /* Otherwise...
             */
            else {
                /* Print the characters before the insert mark with a space
                 * before each one.
                 */
                for (int i=0; i<ins.column; i+=1) printf(" %c", line[i]) ;

                /* If the insert mark is at the end of the line, just print it
                 * and be done.  Otherwise, print it followed by the character
                 * to its right, then print the remaining characters separated
                 * with spaces.
                 */
                if (line[ins.column] == '\0') printf("|") ;
                else {
                    printf("|%c", line[ins.column]) ;
                    for (int i=ins.column+1; line[i] != '\0'; i+=1)
                        printf(" %c", line[i]) ;
                }
            }
            printf("\n") ;
            
        }
    }
    return 0 ;
}