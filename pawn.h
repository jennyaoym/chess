#ifndef PAWN_H
#define PAWN_H
#include <string>
#include "piece.h"

class Square;
class Checkerboard;

class Pawn: public Piece {
  int moved;
  Piece *p;
  bool isenpassant(Square *move);
 public:
  Pawn(std::string colour, Square *sq, Checkerboard *cb);
  ~Pawn();
  char getname() const override;
  int getValue() const override;
  bool can_move(Square *move) override;
  void move_to(Square *move) override;
  bool hasmoved() const override;
  void restore() override;
  void undo() override;
};

#endif


