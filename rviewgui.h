#ifndef RVIEWGUI_H
#define RVIEWGUI_H

#include <QApplication>
//#include <QGuiApplication>
#include <QMainWindow>
#include <QBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>
#include <QStatusBar>
#include <QFile>
#include <QScrollArea>
#include <QLabel>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileDialog>
#include <QMouseEvent>
#include "hist16painter.h"

#include <QDebug>

class rviewGui : public QMainWindow
{
  Q_OBJECT

public:
  bool loadImage(const QString &); // not implemented yet
  rviewGui(QWidget *parent = nullptr);
  ~rviewGui();

private slots:
  void open();
  void save();
  void fitTo();
  void actualSize();
  void zoomIn();
  void zoomOut();
  void negative();
  void exit();
//  void copy();
//  void paste();
//  void zoomIn();
//  void zoomOut();
//  void normalSize();
//  void fitToWindow();
  void help();
private:
  void createActions();
  void createLayouts();
  void updateActions();
  bool saveFile(const QString &fileName);
  void setImage(const QImage &newImage);
  void scaleImage(double factor);  
//  void adjustScrollBar(QScrollBar *scrollBar, double factor); // XXX ?
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);

  hist16painter *histpaint;

  QAction *openAct;
  QAction *saveAct;
  QAction *exitAct;
  QAction *fitAct;
  QAction *actualAct;
  QAction *zoominAct;
  QAction *zoomoutAct;
  QAction *negativeAct;
//  QAction *copyAct;

  QImage mainImage;
  QImage prevImage;
  QLabel *mainImageLabel;
  QLabel *prevImageLabel;
  QFrame *histFrame;
  QScrollArea *scrollArea;
  double scaleFactor = 1.0;
  bool grayImage = false;
  QTextStream outLog; // init from outLog.setDevice(&logFile)
  QFile logFile;
  QPoint currentPosition;
  const int rightPanelWidth = 300;
//  const QSize prevSize = QSize(300,200);

};
#endif // RVIEWGUI_H
