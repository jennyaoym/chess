#include "king.h"
using namespace std;
#include <cmath>
#include "checkerboard.h"
#include "square.h"

King::King(string colour, Square *sq, Checkerboard *cb): 
  Piece(colour, sq, cb), moved{0} {
  if (!(sq->getc() == 'e' && (sq->getr() == '1' || sq->getr() == '8'))) moved = 1;
  value = 900;
  if (colour == "B") name = 'k';
  else if (colour == "W") name = 'K';
}

char King::getname() const {
  return name;
}

int King::getValue() const {
  return value;
}

void King::undo() { --moved;}

bool King::iscastling(Square *move) {  
  if (!(move->getr() == '1' || move->getr() == '8')) return false;
  if (moved) return false;
  if (!(move->getc() == 'g' || move->getc() == 'c')) return false;
  if (move->getr() == this->sq->getr()) {
    Piece *temp = nullptr;
    Square *temp2 = nullptr;
    if (move->getc() == 'c' && this->sq->getc() == 'e') {
      temp2 = cb->squareat(move->gety(), 0);
      if (temp2->isoccupied()) temp = temp2->occupiedby();
      else return false;
    }
    if (move->getc() == 'g' && this->sq->getc() == 'e') {
      temp2 = cb->squareat(move->gety(), 7);
      if (temp2->isoccupied()) temp = temp2->occupiedby();
      else return false;
    }
    if (temp->hasmoved()) return false;
    if ((temp->getname() == 'R' || temp->getname() == 'r' ) && cb->clearhorizontal(temp2, this->sq)
      && this->colour == temp->getcolour()) {
        if (cb->check(colour)) return false;
        else {
          if (move->getc() == 'g') {
            this->move_to(cb->squareat(move->gety(), move->getx() - 1));  
            if (cb->check(colour)) {
              cb->undo();
              return false;
            }
            else {
              this->move_to(cb->squareat(move->gety(), move->getx()));
              if (cb->check(colour)) {
                cb->undo();
                cb->undo();
                return false;
              }
              else {
                cb->undo();
                cb->undo();
                castling = true;
                return true;
              }
            }
         }
         if (move->getc() == 'c') {
           this->move_to(cb->squareat(move->gety(), move->getx() + 1));
           if (cb->check(colour)) {
             cb->undo();
             return false;
           }
           else {
             this->move_to(cb->squareat(move->gety(), move->getx()));
             if (cb->check(colour)) {
               cb->undo();
               cb->undo();
               return false;
             }
             else {
               cb->undo();
               cb->undo();
               castling = true;           
               return true;
             }
           }
         }
       }
    }
  }
  return false;
}

bool King::can_move(Square *move) {
  const int distancer = abs(move->getr() - this->sq->getr());
  const int distancec = abs(move->getc() - this->sq->getc());
  // move 1 square forward/backwards/sideways, valid
  if ((distancer == 1 && distancec == 0) || (distancec == 1 && distancer == 0)) {
    return true;
  }
  // move 1 square diagonally, valid
  else if (distancec == 1 && distancer == 1) {
    return true;
  }
  else if (distancec == 2) return iscastling(move);
  else return false;
}

void King::move_to(Square *move) {
  if (can_move(move)) {
    if (castling) {
      moved++;
      vector<string> s;
      string c1 = "";
      string c2 = "";
      c1.push_back(sq->getc());
      c1.push_back(sq->getr());
      c2.push_back(move->getc());
      c2.push_back(move->getr());
      s.emplace_back(c1);
      s.emplace_back(c2);
      move->setPiece(this);
      sq->setPiece(nullptr);
      sq->notifyObservers();
      move->notifyObservers();
      sq = move;
      cb->setking(colour, move);
      Square *from = nullptr;
      Square *to = nullptr;
      if (move->getc() == 'c') {
        string c3 = "a";
        string c4 = "d";
        c3.push_back(move->getr());
        c4.push_back(move->getr()); 
        s.emplace_back(c3);
        s.emplace_back(c4);
        struct Command C{s, nullptr, false, false, true};
        cb->attachCommand(C);
        from = cb->squareat(move->gety(), 0);
        to = cb->squareat(move->gety(), 3);
      }
      if (move->getc() == 'g') {
        string c3 = "h";
        c3.push_back(move->getr());
        string c4 = "f";
        c4.push_back(move->getr());
        s.emplace_back(c3);
        s.emplace_back(c4);
        struct Command C{s, nullptr, false, false, true};
        cb->attachCommand(C);
        from = cb->squareat(move->gety(), 7);
        to = cb->squareat(move->gety(), 5);
      }
      from->occupiedby()->castling_move(to);
      cb->setTurn();
      castling = false;
    } else {
      if (move->isoccupied()) {  // capture 
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
      cb->setking(colour, move);
      move->setPiece(this);
      sq->setPiece(nullptr);
      sq->notifyObservers();
      moved++;
      move->notifyObservers();
      sq = move;
      cb->setTurn();
    }
  }
  else {
    cout << "1Invalid Move." << endl;
    return;
  }
}
  

bool King::hasmoved() const {
  return moved;
}

