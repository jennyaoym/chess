#ifndef ROOK_H
#define ROOK_H
#include "piece.h"
#include <string>

class Square;
class Checkerboard;

class Rook: public Piece {
  int moved;
 public:
  Rook(std::string colour, Square *sq, Checkerboard *cb);
  ~Rook() {};
  int getValue() const override;
  char getname() const override;
  bool can_move(Square *move) override;
  bool hasmoved() const override;
  void castling_move(Square *move) override;
  void undo() override;
  void move_to(Square *move) override;
};

#endif


