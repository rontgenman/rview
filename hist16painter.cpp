#include "hist16painter.h"

hist16painter::hist16painter(QWidget *parent) : QWidget(parent)
{
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  qInfo("hist16 created");
//  xData = new int [N];
//  yData = new int [N];
}

QSize hist16painter::minimumSizeHint() const {
    return QSize(100, 100);
}

QSize hist16painter::sizeHint() const {
    return QSize(200, 150);
}

void hist16painter::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e);
  QPainter qp(this);
  QPen pen;
  pen.setWidth(1);
  pen.setBrush(Qt::blue);
  qp.setPen(pen);
  qp.drawLine(0,150,200,150);
  qp.drawLine(20,0,20,300);
  qp.drawText(10,165,"0");

//  axes(qp);
//  draw(&qp, N, xData, yData);
}
//void hist16painter::draw(QPainter *qp, int N, int *x, int *y) {
//  Q_UNUSED(x);
//  QPen pen(Qt::black, 2, Qt::SolidLine);
//  qp->setPen(pen);
//  int n;
//  // find max and min Y values // only max for histogram
//  int yMax = 0.0; // yMin = 0.0;
//  for (n = 0; n < N; n++) {
//      if (y[n] > yMax)
//        yMax = y[n];
////      if (y[n] < yMin)
////        yMin = y[n];
//  }
//  qreal scale = yMax/100.0;
//  qreal baseX = 150.0, baseY = 20.0;
// for (n = 0; n < N; n++) {
//     qp->drawLine(n+ baseY, baseX, n + baseY, baseX- (y[n]/scale));
// }
//// for (n = 0; n < N; n+=25)
////   qInfo("(%i) x: %i, y: %g",n,x[n],y[n]/scale);

//       //    } else {
////      for (n = 0; n < N; n++) {
//////      qp->drawLine(x[n]+ baseY, baseX, x[n] + baseY, scale*y[n]);
////        qp->drawLine(x[n]+ baseY, baseX, x[n] + baseY, baseX- scale*y[n]);
////      }
////    }
////  qInfo() << "Min, max, scale: " << 0 << ", " << yMax << ", " << scale;
////             qp->drawPoint(x[n],y[n]);
//}
//void hist16painter::axes(QPainter *qp) {
//  QPen pen;
//  pen.setWidth(1);
//  pen.setBrush(Qt::blue);
//  if (qp==nullptr) qInfo("Bad QPainter");
////  qp->setPen(pen);
////  qp->drawLine(0,150,400,150);
////  qp->drawLine(20,0,20,300);
////  qp->drawText(10,165,"0");
//}

range16 hist16painter::findrange(hist16bit h) {
  // seems to work but does not always as Matlab results
  int n;
  quint16 *y = h.ptr16;
  range16 r;
  r.max = 0;
  for (n = 0; n < h.size; n++) {
      if (y[n] > r.max)
         r.max = y[n];
  }
  r.min = r.max; // does work
  for (n = 0; n < h.size; n++) {
      if (y[n] < r.min)
         r.min = y[n];
  }
  return r;
}

void hist16painter::makehist(hist16bit h, range16 r, int *x, int *y) {
//  histcount hc;
  int n;
  int N = h.bins;
  double step = (r.max-r.min)/N; // Be careful!!
  for (n = 0; n < N; n++) {
      x[n] = r.min + (step/2)*(n+1);
      y[n] = 0;
  }
  int M = h.size;
  quint16 *data = h.ptr16;
  for (n = 0; n < M; n++) {
      ++y[(int) (data[n]/step)];
  }
}
