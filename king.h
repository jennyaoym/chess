#ifndef KING_H
#define KING_H
#include "piece.h"

class Square;
class Checkerboard;

class King: public Piece {
  int moved;
  bool castling;
  bool iscastling(Square *move);
 public:
  King(std::string colour, Square *sq, Checkerboard *cb);
  ~King() {};
  bool hasmoved() const override;
  char getname() const override;
  bool can_move(Square *move) override;
  void move_to(Square *move) override;
  int getValue() const override;
  void undo() override;
};

#endif


