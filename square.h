#ifndef SQUARE_H
#define SQUARE_H
#include <string>
#include "subject.h"

class Piece;

class Square: public Subject {
  char c;
  char r;
  Piece *piece;
  bool occupied;
  std::string colour;
 public:
  Square(char c, char r);
  ~Square();
  int getx() const;
  int gety() const;
  char getc() const override;
  char getr() const override;
  std::string getcolour() const override;
  void setPiece(Piece *p);
  void move_to(Square *sq);
  bool isoccupied() const override;
  Piece* occupiedby() const override;
};

#endif

