#include "Puzzle.h"


/****************************************************/
/****              *** PUZZLE ***                ****/

/* Contsructor : It takes three integers in the following order:
    The first argument is size of the puzzle which is 2^N for some N >= 0.
    The second argument is the initial row id of the puzzle.
    The third argument is the initial column id of the puzzle.
    
    After initialization with the above arguments, the necessary 
    pointers should be initialized to NULL.
    Note that the final row id in this puzzle is equal to: 
       initial_row_id + size - 1.
    Note that the final column id in this puzzle is equal to: 
       initial_column_id + size - 1.
       
    ATTENTION: If a sub-puzzle member does not include any Piece, namely, 
    if its piece is NULL, then do NOT construct it. In other words, do 
    NOT construct subpuzzles unless they are required. 
*/
Puzzle::Puzzle(int size, int first_row , int first_column) {
    this->size=size;
    this->first_row=first_row;
    this->first_column=first_column;
    top_left=NULL;
    top_right=NULL;
    bottom_left=NULL;
    bottom_right=NULL;  

}

/* Destructor  : It should destruct its sub-puzzles, yet it is NOT responsible
   of the destruction of its Piece member.
*/
Puzzle::~Puzzle() {
    if(top_left != NULL)
    delete top_left;
    if(top_right != NULL)
    delete top_right;
        if(bottom_left != NULL)
    delete bottom_left;
        if(bottom_right != NULL)
    delete bottom_right;
    // TODO
}

/* Copy constructor : It totally applies deep copy except the Piece
   member part. Do not deep copy Piece objects of the puzzle, instead
   do shallow copy for Pieces.
*/
Puzzle::Puzzle(const Puzzle& p) {
    this->size=p.size;
    this->first_column=p.first_column;
    this->first_row=p.first_row;
    this->piece=p.piece;
    if(p.bottom_left != NULL){
        this->bottom_left = new Puzzle(*p.bottom_left);
    }
    else 
        this->bottom_left = NULL;
    if(p.bottom_right != NULL){
        this->bottom_right = new Puzzle(*p.bottom_right);
    }
    else 
        this->bottom_right = NULL;
    if(p.top_left != NULL){
        this->top_left = new Puzzle(*p.top_left);
    }
    else 
        this->top_left = NULL;
    if(p.top_right != NULL){
        this->top_right = new Puzzle(*p.top_right);
    }
    else 
        this->top_right = NULL;

}


Puzzle& Puzzle::operator=(const Puzzle& p) {
    if(this != &p) {
        this->size=p.size;
        this->first_column=p.first_column;
        this->first_row=p.first_row;
        this->piece=p.piece;
        this->bottom_left=p.bottom_left;
        this->bottom_right=p.bottom_right;
        this->top_left=p.top_left;
        this->top_right=p.top_right;
    }
    return *this;
}

/* This method directly assigns (without copying) the given Piece object to
   the corresponding sub-puzzle (cell) whose indices given in the second 
   argument in array form as follows <row_id, column_id>.
   
    ATTENTION: If a sub-puzzle member does not include any Piece, namely, 
    if its piece is NULL, then do NOT construct it. In other words, do 
    NOT construct subpuzzles unless they are required.
*/
void Puzzle::placePiece(const Piece& p , int aa[2] ) {

    if(size==1){
        this->piece=&p;
    }
    else{
        if(aa[0]<((size/2)+first_row) && aa[1]<((size/2)+first_column) ){
   
            if(this->top_left!=NULL)
                this->top_left->placePiece(p,aa);
            else{
                Puzzle *b = new Puzzle(size/2,this->first_row,this->first_column);
                this->top_left=b;
                b->placePiece(p,aa);
            }
        }
        else if(aa[0]<((size/2)+first_row) && aa[1]>=((size/2)+first_column) ){

            if(this->top_right!=NULL)
                this->top_right->placePiece(p,aa);
            else{
                Puzzle *b = new Puzzle(size/2,this->first_row,this->first_column+(size/2));
                this->top_right=b;
                b->placePiece(p,aa);
            }
        }
        else if(aa[0]>=((size/2)+first_row) && aa[1]<((size/2)+first_column) ){
 
            if(this->bottom_left!=NULL)
                this->bottom_left->placePiece(p,aa);
            else{
                Puzzle *b = new Puzzle(size/2,this->first_row+(size/2),this->first_column);
                this->bottom_left=b;
                b->placePiece(p,aa);
            }
        }
        else if(aa[0]>=((size/2)+first_row) && aa[1]>=((size/2)+first_column) ){
 
            if(this->bottom_right!=NULL)
                this->bottom_right->placePiece(p,aa);
            else{
                Puzzle *b = new Puzzle(size/2,this->first_row+(size/2),this->first_column+(size/2));
                this->bottom_right=b;
                b->placePiece(p,aa);
            }
        }

    }
}

const Piece& Puzzle::getPiece(int aa[2]) const {
    if(size == 1 && !piece)
        throw EmptyCellRequest();
    else if(size == 1 && piece)
        return *piece;
    else{
        if(aa[0]<((size/2)+first_row) && aa[1]<((size/2)+first_column) ){
            if(this->top_left!=NULL)
                return (top_left->getPiece(aa));
            else
                throw EmptyCellRequest();
        }
        else if(aa[0]<((size/2)+first_row) && aa[1]>=((size/2)+first_column) ){
            if(this->top_right!=NULL)
                return (top_right->getPiece(aa));
            else
                throw EmptyCellRequest();
        }
        else if(aa[0]>=((size/2)+first_row) && aa[1]<((size/2)+first_column) ){
            if(this->bottom_left!=NULL)
                return (bottom_left->getPiece(aa));
            else
                throw EmptyCellRequest();
        }
        else if(aa[0]>=((size/2)+first_row) && aa[1]>=((size/2)+first_column) ){
            if(bottom_right!=NULL)
                return (bottom_right->getPiece(aa));
            else
                throw EmptyCellRequest();
        }

    }
}

Puzzle Puzzle::crop(int from[2], int to[2]) const {
    if((to[0]-from[0]+1)==(size))
        return *this;
    else if((to[0]-from[0]+1)==(size/2)){
        if(first_row == from[0] && first_column==from[1]&& top_left)
            return *top_left;
        else if(first_row == from[0] && (first_column+(size/2))==from[1]&&top_right)
            return *top_right;
        else if((first_row+(size/2)) == from[0] && first_column==from[1]&&bottom_left)
            return *bottom_left;
        else if((first_row+(size/2)) == from[0] && (first_column+(size/2))==from[1] &&bottom_right)
            return *bottom_right;
        else{
            Puzzle puzzle = Puzzle(to[0]-from[0]+1,from[0], from[1]);
            return puzzle;
        }
    }
    else{
        if(from[0]<((size/2)+first_row) && from[1]<((size/2)+first_column)&&top_left)
                return (top_left->crop(from,to));
        else if(from[0]<((size/2)+first_row) && from[1]>=((size/2)+first_column) &&top_right)
                return (top_right->crop(from,to));
        else if(from[0]>=((size/2)+first_row) && from[1]<((size/2)+first_column)&&bottom_left)
                return (bottom_left->crop(from,to));
        else if(from[0]>=((size/2)+first_row) && from[1]>=((size/2)+first_column)&& bottom_right)
                return (bottom_right->crop(from,to));
        else{
                Puzzle puzzle = Puzzle(to[0]-from[0]+1,from[0], from[1]);
                return puzzle;
            }

    }

}

