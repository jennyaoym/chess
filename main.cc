#include <iostream>
#include <string>
#include "checkerboard.h"
#include <stdexcept>
using namespace std;
#include "player.h"

int main(int argc, char *argv[]) {
  string cmd;
  string p1 = "";
  string p2 = "";
  int lvl1 = 0,lvl2 = 0;
  bool is_com1,is_com2;

  while (p1 == "" || p2 == "") {
    if (p1 == "") {
      cin >> p1;
      if (p1 == "human") is_com1 = false;
      else if (p1 == "computer[1]") {
        is_com1 = true;
        lvl1 = 1;
      }
      else if (p1 == "computer[2]") {
        is_com1 = true;
        lvl1 = 2;
      }
      else if (p1 == "computer[3]") {
        is_com1 = true;
        lvl1 = 3;
      }
      else if (p1 == "computer[4]") {
        is_com1 = true;
        lvl1 = 4;
      }
      else {
        p1 = "";
        continue;
      }
    }
    cin >> p2;
    if (p2 == "human") is_com2 = false;
    else if (p2 == "computer[1]") {
      is_com2 = true;
      lvl2 = 1;
    }
    else if (p2 == "computer[2]") {
      is_com2 = true;
      lvl2 = 2;
    }
    else if (p2 == "computer[3]") {
      is_com2 = true;
      lvl2 = 3;
    }
    else if (p2 == "computer[4]") {
      is_com2 = true;
      lvl2 = 4;
    }
    else { 
      p2 = "";
      continue;
    }
  }

  Checkerboard cb{is_com1,is_com2,lvl1,lvl2};
  cb.init();
  cout << cb;

  int count = 0;
  while (true) {
    cin >> cmd;
    if (cin.eof()) {
      cout << "Final Score:" << endl;
      cout << "White: " << cb.getplayer("p1")->getScore() << endl;
      cout << "Black: " << cb.getplayer("p2")->getScore() << endl;
      break;
    }
    ++count;
    if (count > 1 && cmd == "setup") {
      cout << "You can't enter 'setup' mode during the game" << endl;
      --count;
      continue;
    }
    if (cmd == "setup") {
      cb.clear();
      cout << cb;
      while (true) {
        string s;
  	cin >> s;
  	if (s == "+") {
          string piece;
  	  cin >> piece;
  	  string pos;
  	  cin >> pos;
          cb.setup(piece,pos);
          cout << cb;
  	}
  	else if (s == "-") {
          string pos;
  	  cin >> pos;
    	  cb.unset(pos);
    	  cout << cb;
    	}
    	else if (s == "=") {
    	  string t;
    	  cin >> t;
          if (t == "W") cb.setturn("p1");
    	  else if (t == "B") cb.setturn("p2");
    	}
    	else if (s == "done") {
          break;
    	  if (cb.verify()) { 
            if (cb.getturn() == "p1") {
              if (cb.stalemate("W")) {
                cout << "Stalemate!" << endl;
                cb.clear();
                cb.init();
                cout << cb;
                cb.getplayer("p2")->draw();
                cb.getplayer("p1")->draw();
              }
              if (cb.checkmate("W")){
                cout << "Checkmate! Black wins!" << endl;
                cb.getplayer("p2")->winscore();
                cb.clear();
                cb.init();
                cout << cb;
              }
              else if (cb.check("W")) {
                cout << "White is in check!" << endl;
              }
            }
            else if (cb.getturn() == "p2") {
              if (cb.stalemate("B")) {
                cout << "Stalemate!" << endl;
                cb.clear();
                cb.init();
                cout << cb;
                cb.getplayer("p2")->draw();
                cb.getplayer("p1")->draw();
              }
              if (cb.checkmate("B")) {
                cout << "Checkemate! White wins!" << endl;
                cb.getplayer("p1")->winscore();
                cb.clear();
                cb.init();
                cout << cb;
              }
              else if (cb.check("B")) {
                cout << "Black is in check!" << endl;
              }
            }
            break;
    	  }
          else cout << "You can't leave setup mode!" << endl;
        }
      }
    }
    else if (cmd == "move") {
      cb.move();
      cout << cb;
      
      if (cb.getplayer(cb.getturn())->getColour() == "W" && cb.checkmate("W")) {
        cout << "Checkmate! Black wins!" << endl;
        cb.clear();
        cb.init();
        cb.getplayer("p2")->winscore();
        cout << cb;
      }
      else if (cb.getplayer(cb.getturn())->getColour() == "B" && cb.checkmate("B")){
        cout << "Checkmate! White wins!" << endl;
        cb.clear();
        cb.init();
        cb.getplayer("p1")->winscore();
        cout << cb;
      }
      else if (cb.getplayer(cb.getturn())->getColour() == "W" && cb.stalemate("W")) {
        cout << "Stalemate!" << endl;
        cb.clear();
        cb.init();
        cout << cb;
        cb.getplayer("p2")->draw();
        cb.getplayer("p1")->draw();
      }
      else if (cb.getplayer(cb.getturn())->getColour() == "W" && cb.stalemate("W")) {
        cout << "Stalemate!" << endl;
        cb.clear();
        cb.init();
        cout << cb;
        cb.getplayer("p2")->draw();
        cb.getplayer("p1")->draw();          
      }

    }
    else if (cmd == "resign") {
      if (cb.getturn() == "p1") {
        cb.getplayer("p2")->winscore();
        cout << "Black wins!" << endl;
      }
      else {
        cb.getplayer("p1")->winscore();
        cout << "White wins!" << endl;
      }
      cb.clear();
      cb.init();
      cout << cb;
    }
    else if (cmd == "undo") {
      if (argc == 1) cout << "Do not support undo!" << endl;
      else {
        cb.undo();
        cout << cb;
      }
    }
    else if (cmd == "standardopening") {
      if (argc == 1) cout << "Do not support standardopening!" << endl;
      else {
        if (cb.getturn() == "p1") cb.standardopening("W");
        else if (cb.getturn() == "p2") cb.standardopening("B");
      }
    }
    else if (cmd == "history") {
      if (argc == 1) cout << "Do not support history!" << endl;
      else cb.printmoves();
    }
    else --count;   
  }
}

