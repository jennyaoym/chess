#ifndef PIECE_H
#define PIECE_H
#include <string>

class Square;
class Checkerboard;

class Piece {
 protected:
  std::string colour;
  int value;
  Square *sq;
  char name;
  Checkerboard *cb;
 public:
  Piece(std::string colour, Square *sq, Checkerboard *cb);
  virtual ~Piece();
  virtual char getname() const = 0;
  virtual bool can_move(Square *move) = 0;
  virtual int getValue() const = 0;
  virtual void move_to(Square *move);
  virtual void undo() {}
  std::string getcolour();
  void setsquare(Square* sq);
  Square* getsquare();
  virtual bool hasmoved() const;
  virtual void castling_move(Square *move);
  virtual void restore() {return; }
};

#endif


