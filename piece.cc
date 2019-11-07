#include "piece.h"
#include "checkerboard.h"
using namespace std;
#include <vector>
#include "square.h"

Piece::Piece(string colour, Square *sq, Checkerboard *cb): colour{colour}, value{0}, 
  sq{sq}, name{' '}, cb{cb} {}

Piece::~Piece() {}

string Piece::getcolour() {
  return colour;
}

void Piece::setsquare(Square *sq) {
  this->sq = sq;
}

Square* Piece::getsquare() {
  return sq;
}

bool Piece::hasmoved() const {
  return true;
}

void Piece::castling_move(Square *move) {
  return;
}

void Piece::move_to(Square *move) { 
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
    cb->setTurn();
  }
  else {
    cout << "Invalid Move." << endl;
    return;
  }  
}

