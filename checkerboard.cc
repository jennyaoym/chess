#include "player.h"
#include "checkerboard.h"
#include "queen.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "pawn.h"
#include "king.h"
#include "textdisplay.h"
//#include "graphicsdisplay.h"
#include "computer.h"
#include "human.h"
#include "square.h"
using namespace std;

Checkerboard::Checkerboard(bool is_com1,bool is_com2,int lvl1,int lvl2): 
  checkerboard{},commands{},pieces{},td{},Wking{nullptr},Bking{nullptr},p1{nullptr},p2{nullptr},turn{"p1"} {
  td = new TextDisplay();
//  gd = new GraphicsDisplay();
  for (int i = 0; i <= 7; ++i) {
    std::vector<Square *> v;
      for (int j = 0; j <= 7; ++j) {
      char c = 'a' + j;
      char r = 7 - i + '1';
      Square *sq = new Square(c,r);
      sq->attach(td);
//      sq->attach(gd);
      v.emplace_back(sq);
    }
    checkerboard.emplace_back(v);
  }
  if (is_com1) {
    p1 = new Computer("W",true,0,lvl1);
  }
  else {
    p1 = new Human("W",false,0);
  }
  if (is_com2) p2 = new Computer("B",true,0,lvl2);
  else p2 = new Human("B",false,0);
}

Checkerboard::~Checkerboard() {
  delete td;
  //delete gd;
  delete p1;
  delete p2;
  for (auto &p : pieces) {
    delete p;
  }
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard.at(i).at(j);
      delete sq;
    }
  }
}

void Checkerboard::init() {
  td->init();
 // gd->init();
  turn = "p1";
  for (int i = 0; i <= 7; ++i) {
    for (int j = 0; j <= 7; ++j) {
      Square *sq = checkerboard[i][j];
      if (i == 0) {
        if (j == 0 || j == 7) {
          Rook *r = new Rook("B",sq,this);
          pieces.emplace_back(r);
          sq->setPiece(r);
        }
        else if (j == 1|| j == 6) {
          Knight *n = new Knight("B",sq,this);
          pieces.emplace_back(n);
          sq->setPiece(n);
        }
        else if (j == 2 || j == 5) {
          Bishop *b = new Bishop("B",sq,this);
          pieces.emplace_back(b);
          sq->setPiece(b);
        }
        else if (j == 3) {
          Queen *q = new Queen("B",sq,this);
          pieces.emplace_back(q);
          sq->setPiece(q);
        }
        else if (j == 4) {
          King *k = new King("B",sq,this);
          pieces.emplace_back(k);
          Bking = sq;
          sq->setPiece(k);
        }
      }
      else if (i == 1) {
        Pawn *p = new Pawn("B",sq,this);
        pieces.emplace_back(p);
        sq->setPiece(p);
      }
      else if (i == 6) {
        Pawn *p = new Pawn("W",sq,this);
        pieces.emplace_back(p);
        sq->setPiece(p);
      }
      else if (i == 7) {
        if (j == 0 || j == 7) {
          Rook *r = new Rook("W",sq,this);
          pieces.emplace_back(r);
          sq->setPiece(r);
        }
        else if (j == 1|| j == 6) {
          Knight *n = new Knight("W",sq,this);
          pieces.emplace_back(n);
          sq->setPiece(n);
        }
        else if (j == 2 || j == 5) {
          Bishop *b = new Bishop("W",sq,this);
          pieces.emplace_back(b);
          sq->setPiece(b);
        }
        else if (j == 3) {
          Queen *q = new Queen("W",sq,this);
          pieces.emplace_back(q);
          sq->setPiece(q);
        }
        else if (j == 4) {
          King *k = new King("W",sq,this);
          pieces.emplace_back(k);
          Wking = sq;
          sq->setPiece(k);
        }
      }
      else {
        sq->setPiece(nullptr);
      }
    }
  }
}

void Checkerboard::clear() {
  for (auto &p : pieces) {
    delete p;
    p = nullptr;
  }
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard[i][j];
      sq->setPiece(nullptr);
      sq->notifyObservers();
    }
  }
}

void Checkerboard::setTurn() {
  if (turn == "p1") turn = "p2";
  else turn = "p1";
}

void Checkerboard::attachCommand(Command &c) {
  commands.emplace_back(c);
}

Command *Checkerboard::getCommand() {
  return &commands.back();
}

void Checkerboard::undo() {
  if (commands.size() == 0) {
    cout << "You haven't moved!" << endl;
    return;
  }

  Command *cd = this->getCommand();

  this->setTurn();

  int i1 = 7 - (cd->c[1][1] - '1');
  int j1 = cd->c[1][0] - 'a';
  int i2 = 7 - (cd->c[0][1] - '1');
  int j2 = cd->c[0][0] - 'a';
  Square *sq1 = checkerboard[i1][j1];
  Square *sq2 = checkerboard[i2][j2];
  Piece *piece = sq1->occupiedby();
  if (sq1 == Wking) Wking = sq2;
  else if(sq1 == Bking) Bking = sq2;
  piece->undo();


  if (cd->promotion) {
    sq1->occupiedby()->restore();
    sq2->setPiece(piece);
    sq1->setPiece(nullptr);
    piece->setsquare(sq2);
  }
  else if (cd->enpassant) {
    sq2->setPiece(piece);
    sq1->setPiece(nullptr);
    piece->setsquare(sq2);
    cd->p->getsquare()->setPiece(cd->p);
    cd->p->getsquare()->notifyObservers();
  }
  else if (cd->castling) {
    if (piece->getcolour() == "W") Wking = sq2;
    else if (piece->getcolour() == "B") Bking = sq2;
    sq2->setPiece(piece);
    sq1->setPiece(nullptr);
    piece->setsquare(sq2);
    int i3 = 7 - (cd->c[3][1] - '1');
    int i4 = 7 - (cd->c[2][1] - '1');
    int j3 = cd->c[3][0] - 'a';
    int j4 = cd->c[2][0] - 'a';
    Square *sq3 = checkerboard[i3][j3];
    Square *sq4 = checkerboard[i4][j4];
    Piece *piece2 = sq3->occupiedby();
    
    sq4->setPiece(piece2);
    sq3->setPiece(nullptr);
    piece2->setsquare(sq4);
    sq3->notifyObservers();
    sq4->notifyObservers();
  } 
  else {
    sq2->setPiece(piece);
    sq1->setPiece(nullptr);
    piece->setsquare(sq2);
  }
  if (cd->p) cd->p->getsquare()->setPiece(cd->p);
  commands.pop_back();
  
  sq1->notifyObservers();
  sq2->notifyObservers();
}

 
void Checkerboard::move(string sq1, string sq2) {
  Player *pl = nullptr;
  if (turn == "p1") pl = p1;
  else pl = p2;
  if (pl->getIs_com()) {
    string c = pl->getColour();
    if (pl->getLevel() == 1) {
      this->move1(c);
    } 
    else if (pl->getLevel() == 2) {
      this->move2(c);
    }
    else if (pl->getLevel() == 3) {
      this->move3(c);
    }
    else if (pl->getLevel() == 4) {
      this->move4(c);
    }
  }
  else {
    while(cin >> sq1) {
      if ((sq1.length() == 2) && (sq1[0] <= 'h') && (sq1[0] >= 'a') && (sq1[1] >= '1') && (sq1[1] <= '8')) break;
    }
    while(cin >> sq2) {
      if ((sq2.length() == 2) && (sq2[0] <= 'h') && (sq2[0] >= 'a') && (sq2[1] >= '1') && (sq2[1] <= '8')) break;
    }
    int i1 = 7 - (sq1[1] - '1');
    int j1 = sq1[0] - 'a';
    int i2 = 7 - (sq2[1] - '1');
    int j2 = sq2[0] - 'a';
    Square *s1 = checkerboard.at(i1).at(j1);
    Square *s2 = checkerboard.at(i2).at(j2);
    if (!s1->isoccupied()) { 
      cout << "Invalid Move! There is no piece here" << endl;
      return;
    }
    
    if (s1->occupiedby()->getcolour() != pl->getColour()) {
      cout <<  "Invalid Move! It's not your turn! " << endl;
      return;
    }

    if (s1 == Wking) Wking->move_to(s2);
    else if (s1 == Bking) Bking->move_to(s2);
    else s1->move_to(s2);

    if (s1 == Wking) Wking = s2;
    if (s1 == Bking) Bking = s2;
    if (turn == "p2") {
      if (check("W")) {
        cout << "Invalid Move! White is in check!" << endl;
        this->undo();
      }
      else if (check("B")) cout << "Black is in check!" << endl;
    } 
    else if (turn == "p1") {
      if (check("B")) {
        cout << "Invalid Move! Black is in check!" << endl;
        this->undo();
      }
      else if (check("W")) cout << "White is in check!" << endl;
    }

  }
}

bool Checkerboard::check(string king) {
  if (king == "B") {
    for (int i = 0;i <= 7; ++i) {
      for (int j = 0; j <= 7; ++j) {
        Square *sq = checkerboard[i][j];
        if (sq->isoccupied()) {
          Piece *p = sq->occupiedby();
          if (p->getcolour() != king) {
            if (p->can_move(Bking)) return true;
  	  }
        }
      }
    }
  }
  else {
    for (int x = 0;x <= 7; ++x) {
      for (int y = 0; y <= 7; ++y) {
        Square *sq = checkerboard.at(x).at(y);
        if (sq->isoccupied()) {
          Piece *p = sq->occupiedby();
          if (p->getcolour() != king) {
            if (p->can_move(Wking)) return true;
  	  }
        }
      }
    }
  }
  return false;
}

bool Checkerboard::checkmate(string king) {
  Square *to = nullptr;
  for (int i = 0; i <= 7; ++i) {
    for (int j =0; j <= 7; ++j) {
      Square *sq = checkerboard[i][j];
      if (king == "W") {
        if (sq->isoccupied() && sq->occupiedby()->getcolour() == king) continue;
    	if (Wking->occupiedby()->can_move(sq)) {
          to = sq;
          Wking->move_to(sq);
          if (!check("W")) {
            this->undo();
            return false;
          }
          this->undo();
        } 
      }
      else if (king == "B") {
        if (sq->isoccupied() && sq->occupiedby()->getcolour() == king) continue;
        if (Bking->occupiedby()->can_move(sq)) {
          to = sq;
          Bking->move_to(sq);
          if (!check("B")) {
            this->undo();
            return false;
          }
          this->undo();
        }
      }
    }
  }
  for (int a = 0; a <= 7; ++a) {
    for (int b = 0; b <= 7; ++b) {
      Square *s = checkerboard[a][b];
      if (s->isoccupied() && s->occupiedby()->getcolour() == king) {
        for (int x =0; x <= 7; ++x) {
          for (int y = 0; y <= 7; ++y) {
            Square *s2 = checkerboard[x][y];
            if (s2->isoccupied() && s2->occupiedby()->getcolour() == king) continue;
            if (s->occupiedby()->can_move(s2)) {
              s->move_to(s2);
              if (!check(king)) {
                this->undo();
                return false;
              }
              this->undo();
            }
          }
        }
      }
    }
  }
  if (!to) return check(king);
  else return true;
}

void Checkerboard::concede() {
  if (turn == "p1") cout << "Black wins!";
  else cout << "White wins!";
}

void Checkerboard::setup(string piece, string sq) {
  char c = sq[0];
  char r = sq[1];
  int j = c - 'a';
  int i = 7 - (r - '1');
  Square *toset = checkerboard[i][j];
  if (piece == "r") {
    Rook *r = new Rook("B",toset,this);
    toset->setPiece(r);
    pieces.emplace_back(r);
  }
  else if (piece == "n") {
    Knight *n = new Knight("B",toset,this);
    pieces.emplace_back(n);
    toset->setPiece(n);
  }
  else if (piece == "b") {
    Bishop *b = new Bishop("B",toset,this);
    pieces.emplace_back(b);
    toset->setPiece(b);
  }
  else if (piece == "q") {
    Queen *q = new Queen("B",toset,this);
    pieces.emplace_back(q);
    toset->setPiece(q);
  }
  else if (piece == "k") {
    King *k = new King("B",toset,this);
    pieces.emplace_back(k);
    Bking = toset;
    toset->setPiece(k);
  }
  else if (piece == "p") {
    Pawn *p = new Pawn("B",toset,this);
    pieces.emplace_back(p);
    toset->setPiece(p);
  }
  else if (piece == "P") {
    Pawn *p = new Pawn("W",toset,this);
    pieces.emplace_back(p);
    toset->setPiece(p);
  }
  else if (piece == "R") {
    Rook *r = new Rook("W",toset,this);
    pieces.emplace_back(r);
    toset->setPiece(r);
  }
  else if (piece == "N") {
    Knight *n = new Knight("W",toset,this);
    pieces.emplace_back(n);
    toset->setPiece(n);
  }
  else if (piece == "B") {
    Bishop *b = new Bishop("W",toset,this);
    pieces.emplace_back(b);
    toset->setPiece(b);
  }
  else if (piece == "Q") {
    Queen *q = new Queen("W",toset,this);
    pieces.emplace_back(q);
    toset->setPiece(q);
  }
  else if (piece == "K") {
    King *k = new King("W",toset,this);
    pieces.emplace_back(k);
    toset->setPiece(k);
    Wking = toset;
  }
  toset->notifyObservers();
}


void Checkerboard::unset(string sq) {
  char c = sq[0];
  char r = sq[1];
  int j = c - 'a';
  int i = 7 - (r - '1');

  Square *tounset = checkerboard[i][j];
  tounset->setPiece(nullptr);
  tounset->notifyObservers();
}

void Checkerboard::setturn(string t) {
  turn = t;
}

ostream &operator<<(ostream &out, Checkerboard &cb) {
  out << *cb.td;
  return out;
}

bool Checkerboard::verify() {
  int bk = 0,wk = 0;
  bool pawn = true;

  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard[i][j];
      if (sq->isoccupied()) {
        if (sq->occupiedby()->getname() == 'K') ++wk;
        if (sq->occupiedby()->getname() == 'k') ++bk;
        if (sq->occupiedby()->getname() == 'p') {
          if (sq->getr() == '1') pawn = false;
        }
        if (sq->occupiedby()->getname() == 'P') {
          if (sq->getr() == '8') pawn = false;
        }
      }
    }
  }

  return (bk == 1) && (wk == 1) && pawn;
}

bool Checkerboard::underattack(Piece *p) {
  Square *pos = p->getsquare();
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard[i][j];
      if (sq->isoccupied() && sq->occupiedby()->getcolour() != p->getcolour()) {
        if (sq->occupiedby()->can_move(pos)) return true;
      } 
    }
  }
  return false;
}


int Checkerboard::value(string colour) {
  int v = 0;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard[i][j];
      if (sq->isoccupied() && sq->occupiedby()->getcolour() == colour) v += sq->occupiedby()->getValue();
      else if (sq->isoccupied() && sq->occupiedby()->getcolour() != colour) v = v - sq->occupiedby()->getValue();
    }
  }
  return v;
}

Player *Checkerboard::getplayer(string p) {
  if (p == "p1") return p1;
  else return p2;
}

string Checkerboard::getturn() {
  return turn;
}

void Checkerboard::setking(string colour, Square *sq) {
  if (colour == "B") Bking = sq;
  else if (colour == "W") Wking = sq;
}

void Checkerboard::move1(string colour) {
  Square *from = nullptr;
  Square *to = nullptr;
  vector<vector<Square *>> v;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard[i][j];
      if (sq->isoccupied()) {
    	if (sq->occupiedby()->getcolour() == colour) {
    	  from = sq;
    	  for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
              Square *sq2 = checkerboard[x][y];
              if (sq2->isoccupied() && sq2->occupiedby()->getcolour() == colour) continue;
              if (from->occupiedby()->can_move(sq2)) {
                to = sq2;
                from->move_to(sq2);
                if (check(colour)) { 
                  this->undo();
                  continue;
                }
                vector<Square *> sv;
                sv.emplace_back(from);
                sv.emplace_back(to);
                v.emplace_back(sv);
                this->undo();
              }
  	    }
          }
        }
      }
    }
  }
  int count = rand()%(v.size());
  v[count][0]->move_to(v[count][1]);
}

void Checkerboard::move2(string colour) {
  Square *from = nullptr;
  Square *to = nullptr;
  for(int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard[i][j];
      if (sq->isoccupied() && (sq->occupiedby()->getcolour() == colour)) {
        from = sq;
        for (int x = 0; x < 8; ++x) {
          for (int y = 0; y < 8; ++y) {
            Square *sq2 = checkerboard[x][y];
            if (sq2->isoccupied() && sq2->occupiedby()->getcolour() == colour) continue;
            if (from->occupiedby()->can_move(sq2)) {
              to = sq2;
              from->move_to(sq2);
              if (colour == "W") {
                if (check("B")) {
                  return;
                }
              }
              else if (colour == "B") {
                if (check("W")) {
                  return;
                }
              }
              this->undo();
            }
            if (sq2->isoccupied()) {
              if (sq2->occupiedby()->getcolour() != colour && from->occupiedby()->can_move(sq2)) {
                to = sq2;
                from->move_to(sq2);
                return;
              }
            }
          }
        }
      }
    }
  }
  this->move1(colour);
}

void Checkerboard::move3(string colour) {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard[i][j];
      if (sq->isoccupied() && sq->occupiedby()->getcolour() == colour && underattack(sq->occupiedby())) {
        for (int x = 0; x < 8; ++x) {
          for (int y = 0; y < 8; ++y) {
            Square *sq2 = checkerboard[x][y];
            if (sq2->isoccupied() && sq2->occupiedby()->getcolour() == colour) continue;
  	    if (sq->occupiedby()->can_move(sq2)) {
  	      sq->move_to(sq2);
              if (this->check(colour) || this->underattack(sq2->occupiedby())) {
                this->undo();
                break;
              }
              if (colour == "W") {
                if(check("B")) {
                }
              }
              else if (colour == "B") {
                if(check("W")) {
                }
              }
  	      return;
  	    }
          }
        }
      }
    }
  }
  move2(colour);
}

void Checkerboard::move4(string colour) {
  int pre = this->value(colour);
  int max = -999; 
  Square *from = nullptr;
  Square *to = nullptr;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard[i][j];
      if (sq->isoccupied() && sq->occupiedby()->getcolour() == colour) {
      	for (int x = 0; x < 8; ++x) {
      	  for (int y = 0; y < 8; ++y) {
      	    Square *sq2 = checkerboard[x][y];
            if (sq2->isoccupied() && sq2->occupiedby()->getcolour() == colour) continue;
      	    if (sq->isoccupied() && sq->occupiedby()->can_move(sq2)) {
      	      sq->move_to(sq2);
              if (this->value(colour) > max) {
                max = this->value(colour);
                from = sq;
                to = sq2;
              }
              this->undo();
      	    }
          }   
        }
      }
    }
  }
  if (from && to) {
    if (max > pre) {
      from->move_to(to);
      if (colour == "W") {
        if(check("B")) {
          cout << "Black is in check!" << endl;
        }   
      }   
      else if (colour == "B") {
        if (check("W")) {
          cout << "White is in check!" << endl;
        }   
      }
    }   
    else move3(colour);
  }
}

Square* Checkerboard::squareat(int r, int c) const {
  Square* s = checkerboard[r][c];
  return s;
}

bool Checkerboard::clearvertical(Square *from, Square *to) const {
  int start = 0;
  int end = 0;
  if (from->gety() <= to->gety()) {
    start = from->gety();
    end = to->gety();
  }
  if (from->gety() > to->gety()) {
    start = to->gety();
    end = from->gety();
  }
  if (from->getx() != to->getx()) return false;
  for (int i = start + 1; i < end; ++i) {
    if (squareat(i, from->getx())->isoccupied()) return false;
  } 
  return true;
}

bool Checkerboard::clearhorizontal(Square *from, Square *to) const {
  int start = 0;
  int end = 0;
  if (from->getx() <= to->getx()) {
    start = from->getx();
    end = to->getx();
  }
  if (from->getx() > to->getx()) {
    start = to->getx();
    end = from->getx();
  }
  if (from->gety() != to->gety()) return false;
  for (int i = start + 1; i < end; ++i) {
    if (squareat(from->gety(), i)->isoccupied()) return false;
  }
  return true;
}

bool Checkerboard::cleardiagonal(Square *from, Square *to) const {
  const int directiony = to->gety() - from->gety();
  const int directionx = to->getx() - from->getx();
  int d1 = 1;
  int d2 = 1;
  if (directiony < 0) d1 = -1;
  if (directionx < 0) d2 = -1;
  if (abs(directiony) != abs(directionx)) return false;
  for (int i = 1; i < abs(directionx); ++i) {
    if (squareat(from->gety() + (i * d1), from->getx() + (i * d2))->isoccupied()) {
      return false;
    }
  }
  return true;
}

bool Checkerboard::endrow(Square *sq) const {
  return (sq->gety() == 0 || sq->gety() == 7);
}

bool Checkerboard::stalemate(string colour) {
  Square *form = nullptr;
  Square *to = nullptr;
  if (check(colour)) return false;
  if (colour == "W") {
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        Square *sq = checkerboard[i][j];
        if (sq->isoccupied() && sq->occupiedby()->getcolour() == colour) {
          for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
              Square *sq2 = checkerboard[x][y];
              if (sq2->isoccupied() && sq2->occupiedby()->getcolour() == colour) continue;
              if (sq->occupiedby()->can_move(sq2)) {
                sq->move_to(sq2);
                if (!check(colour)) {
                  this->undo();
                  return false;
                }
                undo();
              }
            }
          }
        }
      }
    }
  }
  return true;
}

void Checkerboard::standardopening(string colour) {
  string from = "  ";
  string to = "  ";
  vector<vector<string>> v;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Square *sq = checkerboard[i][j];
      char c = 'a' + j;
      char r = 7 - i + '1';
      if (sq->isoccupied()) {
        if (sq->occupiedby()->getcolour() == colour) {
          from[0] = c;
          from[1] = r;
          for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
              Square *sq2 = checkerboard[x][y];
              char c2 = 'a' + y;
              char r2 = 7 - x + '1';
              if (sq2->isoccupied() && sq2->occupiedby()->getcolour() == colour) continue;
              if (sq->occupiedby()->can_move(sq2)) {
                to[0] = c2;
                to[1] = r2;
                sq->move_to(sq2);
                if (check(colour)) {
                  this->undo();
                  continue;
                }
                vector<string> sv;
                sv.emplace_back(from);
                sv.emplace_back(to);
                v.emplace_back(sv);
                this->undo();
              }
            }
          }
        }
      }
    }
  }
  for (size_t k = 0; k < v.size(); ++k) cout << "move " << v[k][0] << " " << v[k][1] << endl;
}

void Checkerboard::printmoves() {
  const int size = commands.size();
  if (size == 0) {
    cout << "No move has been made!" << endl;
    return;
  }
  else {
    for (int i = 0; i < size; ++i) {
      cout << "move " << commands[i].c[0] << " " << commands[i].c[1] << endl;
    }
  }
}

