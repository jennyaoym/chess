#include "textdisplay.h"
using namespace std;
#include "piece.h"
TextDisplay::TextDisplay(): theDisplay{}, size{8} {
  for (int i = 0; i < size; ++i) {
    vector<char> v;
    for (int j = 0; j < size; ++j) {
      if ((i + j) % 2 == 0) v.push_back(' ');
      else v.emplace_back('-');
    }
    theDisplay.emplace_back(v);
  }
}

void TextDisplay::init() {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if (i == 0) {
        if (j == 0 || j == 7) theDisplay[i][j] = 'r';
        if (j == 1 || j == 6) theDisplay[i][j] = 'n';
        if (j == 2 || j == 5) theDisplay[i][j] = 'b';
        if (j == 3) theDisplay[i][j] = 'q';
        if (j == 4) theDisplay[i][j] = 'k';
      }
      else if (i == 1) theDisplay[i][j] = 'p';
      else if (i == 6) theDisplay[i][j] = 'P';
      else if (i == 7) {
        if (j == 0 || j == 7) theDisplay[i][j] = 'R';
        if (j == 1 || j == 6) theDisplay[i][j] = 'N';
        if (j == 2 || j == 5) theDisplay[i][j] = 'B';
        if (j == 3) theDisplay[i][j] = 'Q';
        if (j == 4) theDisplay[i][j] = 'K';
      }
      else if ((i + j) % 2 == 0) theDisplay[i][j] = ' ';
      else theDisplay[i][j] = '-';
    }
  }
}

void TextDisplay::notify(Subject &whoNotified) {
  int c = whoNotified.getc() - 'a';
  int r = whoNotified.getr() - '1';
  if (whoNotified.isoccupied()) theDisplay[7 - r][c] = (whoNotified.occupiedby())->getname();
  else if (whoNotified.getcolour() == "B") theDisplay[7 - r][c] = '-';
  else theDisplay[7-r][c] = ' '; 
}

ostream &operator<<(ostream &out, const TextDisplay &td) {
  for (int i = 0; i < td.size; ++i) {
    for (int j = 0; j < td.size; ++ j) {
      if (j == 0) {
        int r = 8 - i;
        cout << r << " ";
      }
      cout << td.theDisplay[i][j];
      if (j == td.size - 1) cout << endl;
      if (i == td.size - 1 && j == td.size - 1) {
    	cout << endl;
    	cout << "  ";
    	char c = 'a';
    	for (int k = 0; k < td.size; ++k) {
    	  char ch = c + k;
    	  cout << ch;
    	  if (k == td.size - 1) cout << endl;
    	}
      }
    }
  }
  return out;
}


