#include "rviewgui.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  rviewGui w;
  w.show();
  return a.exec();
}
