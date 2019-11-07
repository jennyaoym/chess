#ifndef QUEEN_H
#define QUEEN_H
#include "piece.h"
#include <string>

class Square;
class Checkerboard;

class Queen: public Piece {
 public:
  Queen(std::string colour, Square *sq, Checkerboard *cb);
  ~Queen();
  char getname() const override;
  int getValue() const override;
  bool can_move(Square *move) override;
};

#endif

