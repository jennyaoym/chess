#include "bishop.h"
using namespace std;
#include "checkerboard.h"

Bishop::Bishop(string colour, Square *sq, Checkerboard *cb): Piece(colour, sq, cb) {
  value = 30;
  if (colour == "B") name = 'b';
  else if (colour == "W") name = 'B';
 }

char Bishop::getname() const {
  return name;
}

int Bishop::getValue() const {
  return value;
}

bool Bishop::can_move(Square *move) {
  if (cb->cleardiagonal(sq, move)) {
    return true;
  }
  return false;
}


