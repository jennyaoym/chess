#include "pawn.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include <cmath>
using namespace std;
#include <vector>
#include "checkerboard.h"
#include "square.h"

Pawn::Pawn(string colour, Square *sq, Checkerboard *cb): 
  Piece(colour, sq, cb), moved{0}, p{nullptr} {
  value = 10;
  if (colour == "B") name = 'p';
  else if (colour == "W") name = 'P';
}

Pawn::~Pawn() {
  delete p;
}

void Pawn::undo() { --moved;}

int Pawn::getValue() const {
  if (p) return p->getValue();
  else return value;
}

char Pawn::getname() const {
  if (p) return p->getname();
  else return name;
}

bool Pawn::hasmoved() const {
  return moved;
}

bool Pawn::isenpassant(Square *move) {
  if (moved) {
    string prev1 = cb->getCommand()->c[0];
    string prev2 = cb->getCommand()->c[1];
    Square *sq1 = cb->squareat(7 - (prev2[1] - '1'), prev2[0] - 'a');
    if (prev1[0] == prev2[0] && (abs(prev1[1] - prev2[1]) == 2)) {
      if (colour == "B" && sq1->occupiedby()->getname() == 'P') {
        if (prev2[1] == '4' && sq->getr() == '4' && move->getc() == prev2[0] && 
          move->getr() == '3' && prev1[1] == '2') return true;
      }
      if (colour == "W" && sq1->occupiedby()->getname() == 'p') {

        if (prev2[1] == '5' && sq->getr() == '5' && move->getc() == prev2[0] && 
          move->getr() == '6' && prev1[1] == '7') return true;
      }
    }
  }
  return false;
}


bool Pawn::can_move(Square *move) {
  int distancec = abs(this->sq->getc() - move->getc());
  int distancer = this->sq->getr() - move->getr();
  if (p) {
    return p->can_move(move);
  }
  else {
    if (colour == "W") {
      distancer = distancer * (-1);
    }
    if (distancer == 1 && distancec == 0 && (!move->isoccupied())) {
      return true;
    }
    else if (distancer == 2 && distancec == 0 && 
             cb->clearvertical(sq, move) && (!move->isoccupied())) {
      if (this->sq->getr() == '2' && this->colour == "W") return true;
      if (this->sq->getr() == '7' && this->colour == "B") return true;
    }
    else if (distancer == 1 && distancec == 1 && move->isoccupied()) {
      return true;
    }
  }
  return isenpassant(move);
}

void Pawn::move_to(Square *move) {
  if (p) {
    p->move_to(move);
  }
  else {
    // normal pawn move
    if (can_move(move)) {
      vector<string> s;
      string c1 = "";
      string c2 = "";
      c1.push_back(sq->getc());
      c1.push_back(sq->getr());
      c2.push_back(move->getc());
      c2.push_back(move->getr());
      s.emplace_back(c1);
      s.emplace_back(c2);
      if (isenpassant(move)) {
        string prev2 = cb->getCommand()->c[1];
        Square *sq1 = cb->squareat(7 - (prev2[1] - '1'), prev2[0] - 'a');
        struct Command c{s, sq1->occupiedby(), false, true, false};
        sq1->setPiece(nullptr);
        sq1->notifyObservers();
        cb->attachCommand(c);
      }  
      else {
        if (move->isoccupied()) {
          if (move->occupiedby()->getcolour() == this->colour) {
            cout << "Invalid Move." << endl;
            return;
          }
        }
        if (cb->endrow(move)) {cout << move->getr() << "no"  << endl;
          if (cb->getplayer(cb->getturn())->getIs_com()) p = new Queen(colour,move,cb);
          else {
            string pro;
            while (cin >> pro) {
              if (pro == "Q" || pro == "q") { 
                p = new Queen(colour, move, cb);
                break;
              }
              else if (pro == "R" || pro == "r") {
                p = new Rook(colour, move, cb);
                break;
              }
              else if (pro == "B" || pro == "b") {
                p = new Bishop(colour, move, cb);
                break;
              }
              else if (pro == "N" || pro == "n") {
                p = new Knight(colour, move, cb);
                break;
              }
              else cout << "Invalid promotion. Please choose another piece." << endl;  
            }
          }
          struct Command c{s, move->occupiedby(), true, false, false};
          cb->attachCommand(c);
        } else {
          struct Command c{s, move->occupiedby(), false, false, false};
          cb->attachCommand(c);
        }
      }
      move->setPiece(this);
      sq->setPiece(nullptr);
      sq->notifyObservers();
      move->notifyObservers();
      sq = move;
      moved += 1;
      cb->setTurn();
    }
    else {
      cout << "Invalid Move." << endl;
      return ;
    }
 } 
}

void Pawn::restore() {
  delete p;
  p = nullptr;
  sq->notifyObservers();
}

