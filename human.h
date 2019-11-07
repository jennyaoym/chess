#ifndef HUMAN_H
#define HUMAN_H
#include "player.h"

class Human: public Player {
  public:
  ~Human() override {};
  Human(std::string colour,bool is_com,double score): Player{colour,is_com,score} {}
  int getLevel() override { return 0;}
};
#endif


