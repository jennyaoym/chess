#ifndef GRAPHICSDISPALY_H
#define GRAPHICSDISPALY_H
#include "observer.h"
#include "square.h"
#include "window.h"

class GraphicsDisplay: public Observer {
  Xwindow xw;
  const int size = 8;
  public:
  GraphicsDisplay();
  void init();
  void notify(Subject &whoNotified) override;
};
#endif


