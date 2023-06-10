/* COMP 211 Homework 9:  Using linked lists to implement an editor buffer
 *
 * .Ndebele Prosper
 */

/* 
 * Abstractly, an editor buffer can be viewed as a collection of characters
 * that are distributed on a rectangular grid, where each position is
 * identified by its row and column (both of which are 0-indexed).  The buffer
 * can consist of any characters except the newline character ('\n').  If there
 * is a character in position (r, c) (i.e., row r and column c), then there
 * must be characters in positions (r, 0),...,(r, c-1).  A "line" in the buffer
 * consists of the characters at positions (r, 0),...,(r, c-1) for some fixed
 * r, where each position (r, i) for 0 â‰¤ i < c has a character and no position
 * (r, i) for c â‰¤ i has a character.  We say that c is the length of the line,
 * or the number of characters in that line.  It need not be the case that all
 * the lines in the buffer have the same length.
 *
 * An editor buffer also has a notion of an insertion mark, which is always at
 * the beginning of a line, between two adjacent characters in a line, or at
 * the end of a line.  The insertion mark can be moved left, right, up, and
 * down.  Characters can be inserted to the left of the insertion mark, and the
 * character to the left or right of the mark can be deleted.
 */
struct buffer ;

/* A struct pos value represents a position in a buffer.  The intent is that if
 * p is of type struct pos, the p represents the position (p.row, p.column).
 */
struct pos {
    int row ;
    int column ;
} ;

/* This line just makes 'buffer' an abbreviation for 'struct buffer'.
 */
typedef struct buffer buffer ;

/* empty = an empty buffer.
 */
buffer* buf_empty() ;

/* insert(buf, c) inserts c to the left of the insertion point of buf.  After
 * the insertion, the insert mark is to the right of c.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_insert(buffer *, char) ;

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
void buf_insert_newline(buffer *) ;

/* delete_left(buf) deletes the character to the left of the insert mark.  If
 * there is no character to the left of the insert mark, this function has no
 * effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_delete_left(buffer *) ;

/* delete_right(buf) deletes the character to the right of the insert mark.  If
 * there is no character to the right of the insert mark, this function has no
 * effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_delete_right(buffer *) ;

/* move_left(buf) moves the insert mark one character to the left.  If there is
 * no character to the left of the insert mark, this functdion has no effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_move_left(buffer *) ;

/* move_right(buf) moves the insert mark one character to the right.  If there
 * is no character to the right of the insert mark, this functdion has no
 * effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME!
 */
void buf_move_right(buffer *) ;

/* move_up(buf) moves the insertion mark to the line above the current line.
 * The horizontal position of the insertion mark will be min(h, l), where h is
 * the horizontal position of the insert mark in buf, and l is the length of
 * the line above the current line.  In other words, the horizontal position is
 * unchanged, unless the line above is too short, in which case the insertion
 * mark is placed at the end of the line above.  If there is no line above,
 * this function has no effect.  
 */
void buf_move_up(buffer *) ;

/* move_down(buf) moves the insertion mark to the line below the current line.
 * The horizontal position of the insertion mark will be min(h, l), where h is
 * the horizontal position of the insert mark in buf, and l is the length of
 * the line below the current line.  In other words, the horizontal position is
 * unchanged, unless the line below is too short, in which case the insertion
 * mark is placed at the end of the line below.  If there is no line below,
 * this function has no effect.  
 */
void buf_move_down(buffer *) ;

/* set(buf, pos) sets the insert mark so that it is to the left of the
 * character at position pos.  It is permissible for pos.col to be equal the
 * number of characters in row pos.row, in which case the insert mark is set to
 * be to at the end of the line at p.row.
 *
 * Pre-conditions:  
 * - 0 â‰¤ pos.row < the number of lines in the buffer.
 * - 0 â‰¤ pos.col â‰¤ the number of characters in row pos.row.
 */
void buf_set(buffer *, struct pos) ;

/* If p = get_position(buf), then the insert mark is just to the left of the
 * character at position (p.row, p.column).  It is possible that p.column is
 * equal to the number of characters in pos.row, in which case this means that
 * the insert mark is at the end of the line.
 */
struct pos buf_get_position(buffer *) ;

/* buf_get_nlines(buf) = the number of lines in the buffer.
 */
int buf_get_nlines(buffer*) ;

/* buf_get_line_size(buf, lineno) = the number of characters in line lineno.
 */
int buf_get_line_size(buffer*, int) ;

/* get_line(buffer*, r, line):  get the line of text at row r.
 *
 * Pre-condition:  the length of line is at least one greater than the number
 * of characters in row r.  
 *
 * Post-condition:  line is a NULL-terminated string corresponding to the line
 * at row r.
 */
void buf_get_line(buffer*, int, char[]) ;

/* buf_free(buf):  free all resources associated to buf, including buf itself.
 */
void buf_free(buffer*) ;

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
void buf_print(buffer *) ;
