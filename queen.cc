#include "queen.h"
using namespace std;
#include "checkerboard.h"

Queen::Queen(string colour, Square *sq, Checkerboard *cb): 
  Piece(colour, sq, cb) {
  value = 90;
  if (colour == "B") name = 'q';
  else if (colour == "W") name = 'Q';
 }

Queen::~Queen() {}

char Queen::getname() const {
  return name;
}

int Queen::getValue() const {
  return value;
}

bool Queen::can_move(Square *move) {
  // valid move if moving on a clear vertical/horizontal/diagonal
  if (cb->clearvertical(sq, move)) return true;
  else if (cb->clearhorizontal(sq, move)) return true;
  else if (cb->cleardiagonal(sq, move)) return true;
  return false;
}
