#include "graphicsdisplay.h"
#include <string>
#include "piece.h"
using namespace std;

GraphicsDisplay::GraphicsDisplay(): xw{} {
  int len = 500 / size;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if ((i + j) % 2 == 0) xw.fillRectangle(j*len, i*len, len, len, Xwindow::White);
      else xw.fillRectangle(j*len, i*len, len, len, Xwindow::Black);
    }
  }
}
  	
void GraphicsDisplay::init() {
  int len = 500 / size;
  int plen = 250 / size;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if ((i + j) % 2 == 0) xw.fillRectangle(j*len, i*len, len, len, Xwindow::White);
      else xw.fillRectangle(j*len, i*len, len, len, Xwindow::Black);
      if (i == 0) {
        if (j == 0 || j == 7) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "r", Xwindow::Blue);
        if (j == 1 || j == 6) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "n", Xwindow::Blue);
        if (j == 2 || j == 5) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "b", Xwindow::Blue);
        if (j == 3) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "q", Xwindow::Blue);
        if (j == 4) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "k", Xwindow::Blue);
      }
      else if (i == 1) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "p", Xwindow::Blue);
      else if (i == 6) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "P", Xwindow::Red);
      if (i == 7) {
        if (j == 0 || j == 7) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "R", Xwindow::Red);
        if (j == 1 || j == 6) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "N", Xwindow::Red);
        if (j == 2 || j == 5) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "B", Xwindow::Red);
        if (j == 3) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "Q", Xwindow::Red);
        if (j == 4) xw.drawBigString((2*j+1)*plen, (2*i+1)*plen, "K", Xwindow::Red);
      }
    }
  }
}


void GraphicsDisplay::notify(Subject &whoNotified) {
  int c = whoNotified.getc() - 'a';
  int r = 7 - (whoNotified.getr() - '1');
  int len = 500 / size;
  int plen = 250 / size;
  string color = whoNotified.getcolour();
  if (color == "B") xw.fillRectangle(c*len, r*len, len, len, Xwindow::Black);
  else xw.fillRectangle(c*len, r*len, len, len, Xwindow::White);
  if (whoNotified.isoccupied()) {
    string p_colour = (whoNotified.occupiedby())->getcolour();
    string name = "a";
    name[0] = (whoNotified.occupiedby())->getname();
    if (p_colour == "B") xw.drawBigString((2*c+1)*plen, (2*r+1)*plen, name, Xwindow::Blue);
    else if (p_colour == "W") xw.drawBigString((2*c+1)*plen, (2*r+1)*plen, name, Xwindow::Red);
  }
}


