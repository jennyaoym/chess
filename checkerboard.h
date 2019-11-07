#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H
#include <vector>
#include <string>
#include <iostream>
#include "player.h"

class Player;
class Piece;
class Square;
class TextDisplay;
//class GraphicsDisplay;

struct Command {
  std::vector<std::string> c;
  Piece *p;
  bool promotion;
  bool enpassant;
  bool castling;
};

class Checkerboard {
  std::vector< std::vector<Square *> > checkerboard;
  std::vector<Command> commands;
  std::vector<Piece *> pieces;
  TextDisplay *td;
 // GraphicsDisplay *gd;
  Square *Wking;
  Square *Bking;
  Player *p1;
  Player *p2;
  std::string turn;
  
  public:
  Checkerboard(bool is_com1,bool is_com2,int lvl1,int lvl2);
  ~Checkerboard();

  void init();
  void attachCommand(Command &c);
  Command *getCommand();
  void clear();
  void undo();
  void move(std::string sq1 = "", std::string sq2 = "");
  bool check(std::string king);
  bool checkmate(std::string king);
  void concede();
  void setup(std::string piece, std::string sq);
  void unset(std::string sq);
  void setturn(std::string t);
  bool verify();
  bool underattack(Piece *p);
  int value(std::string colour);
  Player *getplayer(std::string p);
  std::string getturn();
  void setTurn();
  void setking(std::string colour, Square *sq);
  bool stalemate(std::string colour);
  void standardopening(std::string colour);
  void printmoves();

  void move1(std::string colour);
  void move2(std::string colour);
  void move3(std::string colour);
  void move4(std::string colour);

  Square* squareat(int x, int y) const;
  bool clearhorizontal(Square *from, Square *to) const;
  bool clearvertical(Square *from, Square *to) const;
  bool cleardiagonal(Square *from, Square *to) const;
  bool endrow(Square *sq) const;

  friend std::ostream &operator<<(std::ostream &out, Checkerboard &cb);
};


#endif



