#include "player.h"

using namespace std;

Player::Player(string colour,bool is_com,double score):
  colour{colour}, is_com{is_com},score{score}  {}

void Player::winscore() {
  ++score;
}

void Player::draw() {
  score += 0.5;
}

