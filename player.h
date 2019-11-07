#ifndef PLAYER_H
#define PLAYER_H
#include <string>

class Player {
  protected:
  std::string colour;
  bool is_com;
  double score;
  public:
  virtual ~Player() {};
  Player(std::string colour,bool is_com,double score);
  bool getIs_com() { return is_com; }
  double getScore() { return score; }
  void winscore();
  void draw();
  virtual int getLevel() = 0;
  std::string getColour() { return colour;}
};
#endif


