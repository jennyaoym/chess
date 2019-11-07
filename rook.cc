#include "rook.h"
using namespace std;
#include "square.h"
#include "checkerboard.h"
#include <vector>

Rook::Rook(string colour, Square *sq, Checkerboard *cb): 
  Piece(colour, sq, cb), moved{0} {
  value = 50;
  if (colour == "B") name = 'r';
  else if (colour == "W") name = 'R';
}

int Rook::getValue() const {
  return value;
}

char Rook::getname() const {
  return name;
}

void Rook::undo() { --moved; }

bool Rook::hasmoved() const {
  return moved;
}

bool Rook::can_move(Square *move) {
  if (cb->clearhorizontal(sq, move)) {
    return true;
  }
  if (cb->clearvertical(sq, move)) {
    return true;
  }
  return false;
}

void Rook::castling_move(Square *move) {
  move->setPiece(this);
  sq->setPiece(nullptr);
  sq->notifyObservers();
  ++moved;
  move->notifyObservers();
  sq = move;
}

void Rook::move_to(Square *move) {
  if (can_move(move)) {
    if (move->isoccupied()) {
      if (move->occupiedby()->getcolour() == this->colour) {
        cout << "Invalid Move." << endl;
        return;
      }
    }
    vector<string> s;
    string c1 = "";
    string c2 = "";
    c1.push_back(sq->getc());
    c1.push_back(sq->getr());
    c2.push_back(move->getc());
    c2.push_back(move->getr());
    s.emplace_back(c1);
    s.emplace_back(c2);
    struct Command c{s, move->occupiedby(), false, false, false};
    cb->attachCommand(c);
    move->setPiece(this);
    sq->setPiece(nullptr);
    sq->notifyObservers();
    move->notifyObservers();
    sq = move;
    ++move;
    cb->setTurn();
  }
  else {
    cout << "Invalid Move." << endl;
    return;
  }
}

