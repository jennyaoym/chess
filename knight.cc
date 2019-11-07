#include "knight.h"
using namespace std;
#include "square.h"
#include "checkerboard.h"

Knight::Knight(string colour, Square *sq, Checkerboard *cb): Piece(colour, sq, cb) {
  value = 30;
  if (colour == "B") name = 'n';
  else if (colour == "W") name = 'N';
}

Knight::~Knight() {}

char Knight::getname() const {
  return name;
}

int Knight::getValue() const {
  return value;
}

bool Knight::can_move(Square *move) {
  const int distancec = abs(move->getc() - this->sq->getc());
  const int distancer = abs(move->getr() - this->sq->getr());
  // valid if move 1 square forward/backwards and 2 squares sideways or
  //  1 square sideways and 2 squares forward/backwards
  if ((distancec == 1 && distancer == 2) || (distancec == 2 && distancer == 1)) {
    return true;
  }
  return false;
}

