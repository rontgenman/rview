#include "rviewgui.h"

rviewGui::rviewGui(QWidget *parent) : QMainWindow(parent) {
  createLayouts();
  createActions();

  statusBar()->showMessage("Ready");
  this->resize(1200,800);
//  this->resize(QGuiApplication::primaryScreen()->availableSize());
//  QScreen *scr = this->screen();
//  this->resize(scr->availableSize() * 3 / 5);
  this->setWindowTitle("rView 0.1");
  // TO-DO shorten log piece of code
  // TO-DO include timer into log
  logFile.setFileName("rview.log");
  if (logFile.open(QIODevice::WriteOnly)) {
      outLog.setDevice(&logFile);
      outLog << "Constructor done.\n";
//      QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
//                             tr("Log-file is started."));
    } else {
      QMessageBox::warning(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot write into log-file"));
    }
  // =========
}

rviewGui::~rviewGui(){
//  logFile.close();
}
// ---------- Initialization --------------
void rviewGui::createLayouts() {
  // Making holder widget for frames
  QWidget *holderWidget = new QWidget;
  auto *vbox = new QVBoxLayout();
  auto *hbox = new QHBoxLayout(holderWidget);

  mainImageLabel = new QLabel(this);
  mainImageLabel->setScaledContents(true); // to zoom correctly
//  mainImageLabel->setToolTip("Image area");

  prevImageLabel = new QLabel(this);
  prevImageLabel->setFrameStyle(QFrame::Box);
//  prevImageLabel->setLineWidth(1);
  prevImageLabel->setMinimumWidth(rightPanelWidth);
  prevImageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//  prevImageLabel->setToolTip("Navigation");

//  auto *histFrame = new QFrame(this);
//  histFrame->setFrameStyle(QFrame::Box);
//  histFrame->setLineWidth(1);
//  histFrame->setMinimumSize(100,50);
//  histFrame->setToolTip("Histogram");
  histpaint = new hist16painter;

  vbox->setSpacing(5);
//  vbox->addWidget(histFrame);
  vbox->addWidget(histpaint);
  vbox->addWidget(prevImageLabel);

  scrollArea = new QScrollArea;
  scrollArea->setBackgroundRole(QPalette::Dark);
//  scrollArea->setPalette(Qt::darkGreen); //What color to choose?
  scrollArea->setWidget(mainImageLabel);
  scrollArea->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  hbox->addWidget(scrollArea);
  hbox->addSpacing(5);
  hbox->addLayout(vbox);
  holderWidget->setLayout(hbox);
  setCentralWidget(holderWidget);
}

void rviewGui::createActions() {
  // I'm not sure if separating actions and menu items is always good practice
  // like from the Qt book:
//  newAction = new QAction(tr("&New"), this);
//  newAction->setIcon(QIcon(":/images/new.png"));
//  newAction->setShortcut(QKeySequence::New);
//  newAction->setStatusTip(tr("Create a new spreadsheet file"));
//  connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
  // Also, toolbar item should be toogleable this way (p. 68) - NOT, need qtoolbutton

  // Main menu
  openAct = new QAction(tr("&Open"), this);
  openAct->setShortcut(tr("Ctrl+O"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAct = new QAction(tr("&Save as"), this);
  saveAct->setEnabled(false);
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+X"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));

  fitAct = new QAction(tr("&Fit to window"), this);
  fitAct->setShortcut(tr("Ctrl+Space"));
  connect(fitAct, SIGNAL(triggered()), this, SLOT(fitTo()));

  actualAct = new QAction(tr("&Actual size"), this);
  actualAct->setShortcut(tr("Ctrl+0"));
  connect(actualAct, SIGNAL(triggered()), this, SLOT(actualSize()));

  zoominAct = new QAction(tr("Zoom &In"), this);
  zoominAct->setShortcut(tr("Ctrl++"));
  connect(zoominAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

  zoomoutAct = new QAction(tr("Zoom &Out"), this);
  zoomoutAct->setShortcut(tr("Ctrl+-"));
  connect(zoomoutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

  negativeAct = new QAction(tr("&Negative"), this);
  negativeAct->setShortcut(tr("Ctrl+N"));
  negativeAct->setCheckable(true);
  negativeAct->setEnabled(grayImage);
  connect(negativeAct ,SIGNAL(triggered()), this, SLOT(negative()));
  // ---- MENU items -------------
  QMenu *file = menuBar()->addMenu(tr("&File"));
  file->addAction(openAct);
  file->addAction(saveAct);
  file->addSeparator();
  file->addAction(exitAct);
  // -----
  QMenu *view = menuBar()->addMenu(tr("&View"));
  view->addAction(fitAct);
  view->addAction(actualAct);
  view->addAction(zoominAct);
  view->addAction(zoomoutAct);
  view->addSeparator();
  view->addAction(negativeAct);


  // -----
  QMenu *help = menuBar()->addMenu("&Help");
  help->addAction("Abou&t", this, &rviewGui::help);

  // Toolbar
  QPixmap savepix(":/img/save_as.png");
  QPixmap openpix(":/img/open.png");
  QPixmap fitpix(":/img/fit.png");
  QPixmap actualpix(":/img/actual.png");
  QPixmap zoominpix(":/img/zoomin.png");
  QPixmap zoomoutpix(":/img/zoomout.png");
  QPixmap negativpix(":/img/negativebutton.png");
  QPixmap exitpix(":/img/exit.png");

  QToolBar *toolbar = addToolBar("main toolbar");
  toolbar->addAction(QIcon(openpix), "Open", this, &rviewGui::open);
  toolbar->addAction(QIcon(savepix), "Save", this, &rviewGui::save);
  toolbar->addSeparator();
  toolbar->addAction(QIcon(fitpix), "Fit to window", this, &rviewGui::fitTo);
  toolbar->addAction(QIcon(actualpix), "Actual size", this, &rviewGui::actualSize);
  toolbar->addAction(QIcon(zoominpix), "Zoom In", this, &rviewGui::zoomIn);
  toolbar->addAction(QIcon(zoomoutpix), "Zoom Out", this, &rviewGui::zoomOut);

//  QToolButton *toolbtn = new QToolButton; // I don't see how to use it properly
//  toolbtn->addAction(negativeAct);
//  toolbtn->setIcon(QIcon(negativpix));
//  toolbtn->setCheckable(true);
//  toolbtn->setChecked(false);
//  toolbar->addWidget(toolbtn);
  toolbar->addAction(QIcon(negativpix), "Negative view", this, &rviewGui::negative);
  toolbar->addSeparator();
  toolbar->addAction(QIcon(exitpix), "Exit", this, &rviewGui::exit);

}
// ============ SLOTS ===================
void rviewGui::open() {
  QString defdir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
  QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                         defdir, tr("Image Files (*.bmp *.png *.jpg *.tif)"));
  outLog << "Loading image...\n";
  if (!fileName.isNull()) {
//      outLog << "image.isNull = " << mainImage.isNull() << "\n";
      mainImage.load(fileName);
//      outLog << "image.isNull = " << mainImage.isNull() << "\n";
      // via loadImage(const QString &); ??
      mainImageLabel->setPixmap(QPixmap::fromImage(mainImage));
      scaleFactor = 1.0;
      statusBar()->showMessage("Image loaded.");
      scrollArea->setWidgetResizable(false); // WHY false?
      mainImageLabel->adjustSize();
//      mainImageLabel->setAlignment(Qt::AlignRight);
      prevImage = mainImage.scaled(300,300, Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation); // smooth preview
      prevImageLabel->setPixmap(QPixmap::fromImage(prevImage));

      if (mainImage.isGrayscale()) {
          grayImage = true;
          negativeAct->setEnabled(grayImage);
          QSize imgSize = mainImage.size();
//          histpaint->histogram.ptr16 = (quint16*) mainImage.bits();
//          hist->histogram.bins = hist->Nbins;
//          hist.size = imgSize.width()*imgSize.height();
//          histrange = findrange(hist);
//          makehist(hist,histrange,xData,yData);

          // +++++++++++++++++++++++++
          // TODO Here function to draw a histogram
          // First, get data from an image, then in paintEvent draw data directly,
          // without additional functions like axes etc.
//          hist.axes(hist.qp);
          outLog << "Grayscale image, ";
      } else {
          grayImage = false;
          negativeAct->setEnabled(grayImage);
          negativeAct->setChecked(grayImage);
      }

      outLog << "format (" << mainImage.format() << ")\n";
      //     statusBar()->showMessage(QString("%1 loaded").arg(fileName));
      //     this->setWindowTitle(fileName);
  } else {outLog << "Image NOT loaded.\n"; return; }
}
void rviewGui::save() {
  outLog << "Save file.\n";
}

void rviewGui::fitTo() {
  // it works as switcher: if is already fitted then it returt actual size of image
  // Should I check it?
  QSize szScroll = scrollArea->size();
  QSize szLabel = mainImageLabel->size();
  double rw, rh, factor = 1.0;
  rw = 0.995*szScroll.width()/szLabel.width();
  rh = 0.995*szScroll.height()/szLabel.height();
  if (rw > rh) { factor = rh; }
  if (rw < rh) { factor = rw; }
  scaleImage(factor);
//  QString str = QString("rw: %1,rh: %2, zoom: %3").arg(rw).arg(rh).arg(scaleFactor);
//  qInfo() << str;
}
void rviewGui::actualSize() {
  scaleImage(1.0);
}
void rviewGui::zoomIn() {
  scaleImage(1.2);
}
void rviewGui::zoomOut() {
  scaleImage(0.8);
}
void rviewGui::negative() {
  if (grayImage) {
      mainImage.invertPixels(QImage::InvertRgb);
      mainImageLabel->setPixmap(QPixmap::fromImage(mainImage));
//      qInfo() << "isChecked: " << negativeAct->isChecked();
//      if (negativeAct->isChecked()) { negativeAct->setChecked(false); }
//      else { negativeAct->setChecked(true); }
  }
//  qInfo() << "isChecked: " << negativeAct->isChecked();
}
void rviewGui::exit() {
  outLog << "Exit from rview.\n";
  QApplication::quit();
}
void rviewGui::help() {

}

void rviewGui::scaleImage(double factor) {
  if (factor == 1.0) {
      mainImageLabel->adjustSize();
      scaleFactor = 1.0;
    } else {
      scaleFactor *= factor;
      if (scaleFactor > 10.0) {scaleFactor = 10.0; }
      if (scaleFactor < 0.1)  {scaleFactor = 0.1; }
      mainImageLabel->resize(scaleFactor * mainImageLabel->pixmap(Qt::ReturnByValue).size());
//      adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
//      adjustScrollBar(scrollArea->verticalScrollBar(), factor);
    }
  QString str = QString("Zoom: %1%").arg(scaleFactor*100);
  statusBar()->showMessage(str);
  outLog << "1:1 pressed. Zoom = " << scaleFactor << ".\n";
    // this for menu enteries
//    zoomInAct->setEnabled(scaleFactor < 3.0);
//    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

//void rviewGui::adjustScrollBar(QScrollBar *scrollBar, double factor)
////! [25] //! [26]
//{
//    scrollBar->setValue(int(factor * scrollBar->value()
//                            + ((factor - 1) * scrollBar->pageStep()/2)));
//}

void rviewGui::mousePressEvent(QMouseEvent *e) {
  if (e->button() == Qt::RightButton) {
      currentPosition = e->pos();
      QString str = QString("e-pos: %1, %2 global pos: %3, %4")
          .arg(e->x()).arg(e->y()).arg(e->globalX()).arg(e->globalY());
//      outLog << "x: " << curpos.x() << "\n";
      scrollArea ->setCursor(Qt::SizeAllCursor);
      statusBar()->showMessage(str);
    }
}

void rviewGui::mouseMoveEvent(QMouseEvent *e) {
  if (e->button() == Qt::RightButton) {
      int newX = e->x()-currentPosition.x();
      int newY = e->y()-currentPosition.y();
//      mainImageLabel->scroll(newX, newY);
    QString str = QString("Move to: %1, %2").arg(newX).arg(newY);
      //newPosition.x()-currentPosition.x(),newPosition.y()-currentPosition.y());
//      currentPosition.x() = newX;
//     outLog << "x: " << curpos.x() << "\n";
     scrollArea ->setCursor(Qt::SizeAllCursor);
     statusBar()->showMessage("Mouse pressed and moving.");
  }
}
void rviewGui::mouseReleaseEvent(QMouseEvent *e) {
  if (e->button() == Qt::RightButton) {
      scrollArea->setCursor(Qt::ArrowCursor);
      statusBar()->clearMessage();
    }
}
