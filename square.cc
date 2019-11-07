#include "square.h"
#include "piece.h"
using namespace std;
#include <iostream>
Square::Square(char c, char r): c{c}, r{r}, piece{nullptr}, occupied{false}, colour{} {
    if ((7 - (r - '1')) % 2 == 0) {
      if ((c - 'a') % 2 == 0) colour = "W";
      else if ((c - 'a') % 2 == 1) colour = "B";
    }
    else if ((7 - (r - '1')) % 2 == 1) {
      if ((c - 'a') % 2 == 0) colour = "B";
      else if ((c - 'a') % 2 == 1) colour = "W";
    }
  }

Square::~Square() {}

char Square::getc() const {
  return c;
}

char Square::getr() const {
  return r;
}

int Square::getx() const {
  return c - 'a';
}

int Square::gety() const {
  return 7 - (r - '1');
}

string Square::getcolour() const {
  return colour;
}

void Square::move_to(Square *sq) {
  piece->move_to(sq);
}

void Square::setPiece(Piece *p) {
  if (p) {
    piece = p;
    occupied = true;
  } else {
    piece = p;
    occupied = false;
  }
}

bool Square::isoccupied() const {
  return occupied;
}

Piece* Square::occupiedby() const {
  return piece;
}



