#ifndef KNIGHT_H
#define KNIGHT_H
#include "piece.h"
#include <string>

class Square;
class Checkerboard;

class Knight: public Piece {
 public:
  Knight(std::string colour, Square *sq, Checkerboard *cb);
  ~Knight();
  char getname() const override;
  int getValue() const override;
  bool can_move(Square *move) override;
};

#endif

