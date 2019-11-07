#ifndef BISHOP_H
#define BISHOP_H
#include "piece.h"

class Square;
class Checkerboard;

class Bishop: public Piece {
 public:
  Bishop(std::string colour, Square *sq, Checkerboard *cb);
  ~Bishop() {};
  char getname() const override;
  int getValue() const override;
  bool can_move(Square* move) override;
};

#endif


