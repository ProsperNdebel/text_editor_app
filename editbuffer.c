/* .COMP 211 Homework 9:  Using linked lists to implement an editor buffer
 *
 * .Ndebele Prosper
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"editbuffer.h"

/*
*A structure to represents an editor double linked node. Abstractly, a double
*linked node consists of a value which is an ASCII charecter along with addresses for 
* the double linked node previous to it and the double linked node after it
*
*/
struct dl_node {
    //this is the ASCII charectar contained by the particular
    char val;
    //this is a pointer to the node that comes after the current node
    struct dl_node* next;
    //this is the address that comes before the current node
    struct dl_node* prev;
};

struct dline_node {
    //this is the number of charectars in a line
    int size_of_line;
    //this is the head node within this node or its start of the line
    struct dl_node* val;
    //this represents the line that is next 
    struct dline_node* next;
    //this represents the line that is before 
    struct dline_node* prev;
};

/*This line makes 'dl_node' an abbreviation for 'struct dl_node'
*/
typedef struct dl_node dl_node;

typedef struct dline_node dline_node;


/* A structure to represent an editor buffer.  Abstractly, an editor buffer
 * consists of a sequence of characters along with an insertion point, which
 * is a location that is either between two adjacent characters or at the
 * beginning or end of the sequence.  We often write something like
 * "abcd|efg" to describe a buffer where the sequence of characters is
 * a,b,c,d,e,f,g and the insertion point is between the d and e characters.
 *
 * You may (and will need to) add fields to this structure definition.
 *
 * Be sure to explain what abstract buffer is represented by a concrete
 * value of this type!!!
 */
struct buffer {
    /* Characters in the buffer.
     */
    //this is where the buffer starts, it is the head node, gives you the access to all the nodes
    dline_node* main_node;
    //this points/equal to a node in the buffer that the | follows/where the cursor position is
    struct dl_node* cursor_posn;
    //this keeps track of the row that the cursor is in, update it on moving up or doen
    struct dline_node* current_row; 
    //this keeps track of where the cursor position is
    struct pos* position;
    //this is the number of rows in the buffer
    int lines;//keeps track of how many line are there, keeps increasing if a new_line is added
} ;


/* This line just makes 'buffer' an abbreviation for 'struct buffer'.
 */
//typedef struct buffer buffer ;




/* empty = an empty buffer.
 */
buffer* buf_empty(){
    struct buffer* buf = malloc(sizeof(struct buffer));
    //this node will be the head of the main node
    dl_node* n = malloc(sizeof(dl_node));
    n->next = NULL;
    n->prev = NULL;
    n->val = '\0';
    //creating the outer bigger node
    struct dline_node* m = malloc(sizeof(struct dline_node));
    m->size_of_line = 0;
    m->next = NULL;
    m->prev = NULL;
    m->val = n;
    //this will be assigned to main node
    struct dline_node* p = m;
    //initial position
    struct pos* q = malloc(sizeof(struct pos));
    q->row = 0;
    q->column = 0;
    //the initial state of the buffer
    buf->main_node = m;
    buf->cursor_posn = n;
    buf->current_row = m;
    buf->position = q;
    buf->lines = 1;
    return buf;
} ;

/* insert(buf, c) inserts c to the left of the insertion point of buf.  After
 * the insertion, the insert mark is to the right of c.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_insert(buffer * b, char c){
    //assigning memory in the global heap for the node to be created
    dl_node* ins = malloc(sizeof(struct dl_node));
    ins->val = c;  
    //this inserts charectars when the cursor is at the end                                                                                                      
    if(b->cursor_posn->next != NULL){
        b->cursor_posn->next->prev = ins;
        ins->next = b->cursor_posn->next;
        b->cursor_posn->next = ins;
        ins->prev = b->cursor_posn;
    }
    //this inserts charectars when the cursor is within charectars
    else{
        ins->next = b->cursor_posn->next;
        b->cursor_posn->next = ins;
        ins->prev = b->cursor_posn;
    }
    b->cursor_posn = ins;
    //incrementing the column we are at
    b->position->column += 1;
    //updating sizoof of whatever the current line is
    b->current_row->size_of_line += 1;
    return;
} ;

/* insert_newline(buffer *):  starts a new line just after the current line.
 *
 * Pre-conditions:  insert is to the left of position (r, c).
 * Post-condition:  lines r+1,..., are moved to lines r+2,....
 *                  line r consists of characters (r, 0),...,(r, c-1).
 *                  line r+1 consists of characters (r, c), (r, c+1),...
 *                  insert is at the beginning of line r+1.
 *
 * THIS FUNCTION MUST BE CONSTANT SPACE!  That is, the amount of memory
 * allocated by this function must always be the same, no matter what the
 * contents of the buffer.
 */
void buf_insert_newline(buffer * b){
    struct dline_node* new_row = malloc(sizeof(struct dline_node));
    struct dl_node* m = malloc(sizeof(struct dl_node));
    m->val = '\0';
    m->next = NULL;
    m->prev = NULL;
    new_row->val = m;
    new_row->next = NULL;
    new_row->size_of_line = 0;
    //if twe are at the last row
    if(b->current_row->next == NULL){
        b->current_row->next = new_row;
        new_row->next = NULL;
        new_row->prev = b->current_row;
        b->current_row = new_row;
        b->cursor_posn = b->current_row->val;
        b->position->column = 0;
        b->position->row += 1;
    }
    //that is if we are in middle of rows
    else{
        new_row->next = b->current_row->next;
        b->current_row->next->prev = new_row;
        b->current_row->next = new_row;
        new_row->prev = b->current_row;
        b->cursor_posn = new_row->val;
        b->position->column = 0;
        b->position->row += 1;
        b->current_row = new_row;
    }
    //increasing the number of lines in the buffer
    b->lines += 1;
    return;
} ;

/* delete_left(buf) deletes the character to the left of the insert mark.  If
 * there is no character to the left of the insert mark, this function has no
 * effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_delete_left(buffer * b){
    struct dl_node* del = b->cursor_posn;
    //making sure it wont delete the head of any row
    if(del->val == '\0'){
        return;
        }
        //when we are at the end of a line
    else if(del->next == NULL){
        del->prev->next = del->next;
    }
    //when the cursor is in between charactars
    else{
            //this rearranges node connections when the cursor is in the middle of other charectars
            del->prev->next = del->next;
            del->next->prev = del->prev;
        }
        b->cursor_posn = del->prev;
        free(del);
        b->position->column -= 1;
        b->current_row->size_of_line -=1;
        return;
    }    

/* delete_right(buf) deletes the character to the right of the insert mark.  If
 * there is no character to the right of the insert mark, this function has no
 * effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_delete_right(buffer *b){
    //bad code
    struct dl_node* n = NULL;
    struct dl_node* del = b->cursor_posn->next;
    //making sure it wont delete the NULL at the end
    if(del != n){
          b->cursor_posn->next = del->next;
          if(del->next == NULL){
              free(del);
          }
          else{
           del->next->prev = b->cursor_posn;
            free(del);
          }
          b->current_row->size_of_line -=1;
    }
    return;
    };

/* move_left(buf) moves the insert mark one character to the left.  If there is
 * no character to the left of the insert mark, this functdion has no effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_move_left(buffer *b){
    struct dl_node* del = b->cursor_posn;
    //making sure it wont go beyond head otherwise it will end up at NULL and cant be able to move or delete right
    if(del->val != '\0'){
    b->cursor_posn = b->cursor_posn->prev;
    b->position->column -= 1;
    }
    return;
} ;

/* move_right(buf) moves the insert mark one character to the right.  If there
 * is no character to the right of the insert mark, this functdion has no
 * effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_move_right(buffer *b){
    //there are wo conditions for moving right: 1 when there is a NULL at the end 
   //                                           2 when there is a node with val '\0' in the next
    struct dl_node* n = NULL;
    struct dl_node* del = b->cursor_posn->next;
    //making sure that it wont move beyond the NULL at the end of the list otherwise it will give an error
    if(del != NULL){
    b->cursor_posn = b->cursor_posn->next;
    b->position->column += 1;
    };
    return;
};

/* move_up(buf) moves the insertion mark to the line above the current line.
 * The horizontal position of the insertion mark will be min(h, l), where h is
 * the horizontal position of the insert mark in buf, and l is the length of
 * the line above the current line.  In other words, the horizontal position is
 * unchanged, unless the line above is too short, in which case the insertion
 * mark is placed at the end of the line above.  If there is no line above,
 * this function has no effect.  
 */
void buf_move_up(buffer *b){
    //if we are at the first row, no need to go up and do nothing to the cursor position
    if(b->current_row->prev == NULL){
        return;
    }
    //then get its head and loop to some node where we want to place the cursor position
    else{
        //first get the column
        int m = b->position->column;
        //switch the row
        b->current_row = b->current_row->prev;
        //get the head of that new current row
        struct dl_node* n = b->current_row->val;
        for(int i = 0; i < m; i++){
             n =  n->next;
             //that is if the line above is shorter
             if(n->next == NULL){
                 b->cursor_posn = n;
                 b->position->column = (i+1);
                 b->position->row -= 1;
                 return;
             }
    }
    //set cursor postion to that node
    b->cursor_posn = n;
    //update positions but column is unchanged
    b->position->row -= 1;
    return;
    }
    
};

/* move_down(buf) moves the insertion mark to the line below the current line.
 * The horizontal position of the insertion mark will be min(h, l), where h is
 * the horizontal position of the insert mark in buf, and l is the length of
 * the line below the current line.  In other words, the horizontal position is
 * unchanged, unless the line below is too short, in which case the insertion
 * mark is placed at the end of the line below.  If there is no line below,
 * this function has no effect.  
 */
void buf_move_down(buffer *b){
    
    //cant move down if on the last row
    if(b->current_row->next == NULL){
        return;
    }
    else{
        //getting the column at the cursor position
        int m = b->position->column;
        //updatting the current row to the next row
        b->current_row = b->current_row->next;
        //getting the head node of the new current row
        struct dl_node* n = b->current_row->val;
        for(int i = 0; i < m; i++){
            n = n->next;
            //that is if the line below is shorter
            if(n->next == NULL){
                b->cursor_posn = n;
                b->position->column = (i+1);
                b->position->row += 1;
                return;
            }
            }
            b->cursor_posn = n;
            b->position->row += 1;
        }
    return;
    }

/* set(buf, pos) sets the insert mark so that it is to the left of the
 * character at position pos.  It is permissible for pos.col to be equal the
 * number of characters in row pos.row, in which case the insert mark is set to
 * be to at the end of the line at p.row.
 *
 * Pre-conditions:  
 * - 0 â‰¤ pos.row < the number of lines in the buffer.
 * - 0 â‰¤ pos.col â‰¤ the number of characters in row pos.row.
 */
void buf_set(buffer *b, struct pos p){
    //first get the values from the input pos
    int cursor_row = p.row;
    int cursor_col = p.column;
    //then get the main node
    struct dline_node* k = b->main_node;
    for(int i = 0; i < cursor_row; i++){
        k = k->next;
    }
    b->current_row = k;

    //get the first value in that row so you can loop till you rach it
    struct dl_node* l = b->current_row->val;
    for(int i = 0; i < cursor_col; i++){
        l = l->next;
    }
    //set the cursor position to that current dl_node
    b->cursor_posn = l ;
    //then update the colun then the row
    b->position->column = cursor_col;
    b->position->row = cursor_row;
    return;
}  ;

/* If p = get_position(buf), then the insert mark is just to the left of the
 * character at position (p.row, p.column).  It is possible that p.column is
 * equal to the number of characters in pos.row, in which case this means that
 * the insert mark is at the end of the line.
 */
struct pos buf_get_position(buffer *b){
    struct pos cursor = {.row = b->position->row, .column = b->position->column};
    return cursor;
} ;

/* buf_get_nlines(buf) = the number of lines in the buffer.
 */
int buf_get_nlines(buffer* b){
    return b->lines;
} ;

/* buf_get_line_size(buf, lineno) = the number of characters in line lineno.
 */
int buf_get_line_size(buffer* b, int x){
    struct dline_node* mov = b->main_node;
    for(int i = 0; i  < x; i++){
        mov = mov->next;
    }
    return mov->size_of_line;
} ;

/* get_line(buffer*, r, line):  get the line of text at row r.
 *
 * Pre-condition:  the length of line is at least one greater than the number
 * of characters in row r.  
 *
 * Post-condition:  line is a NULL-terminated string corresponding to the line
 * at row r.
 */
void buf_get_line(buffer* b, int y, char xs[]){
    //first get to the line/row
    struct dline_node* mov =  b->main_node;
    for(int i = 0; i < y; i++){
        mov = mov->next;
    }
    //get the head node at that line 
    struct dl_node* n = mov->val;
    //get the size of that row then use it to file the array
    int x = mov->size_of_line;
    //then loop through all the nodes and input them into an array xs
    int i = 0;
    for(i = 0; i<x; i++){
        n = n->next;
        xs[i] = n->val;
    }
    //inputing the terminating character at the end of the array
    xs[i] = '\0';
    return;
} ;

/* buf_free(buf):  free all resources associated to buf, including buf itself.
 */
void buf_free(buffer*b){
    struct dline_node* mov = b->main_node;
    struct dline_node* store = mov;
    while(mov != NULL){
        struct dl_node* n = mov->val;
        struct dl_node* m = n;
        while(n != NULL){
            n = n->next;
            free(m);
            m = n;
        }
        mov = mov->next;
        free(store);
        store = mov;
    }
    free(b);
    return;
} ;

/*  print_buffer(buf).
 *
 *  Post-condition:  Some information about buf will be printed to ther
 *  terminal.  
 *
 *  The implementation of this function is entirely up to you.  It is here to
 *  help you with debugging.  We will never use it when testing your code, but
 *  there is an option in the driver program that will call this function with
 *  the current buffer.
 */
void buf_print(buffer *b){

    return;
} ;
