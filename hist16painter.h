#ifndef HIST16PAINTER_H
#define HIST16PAINTER_H

#include <QWidget>
#include <QPainter>

struct hist16bit {
  quint16 *ptr16;
  int bins;
  int size;
};
struct range16 {
  int min;
  int max;
};

class hist16painter : public QWidget
{
  Q_OBJECT
public:
  explicit hist16painter(QWidget *parent = nullptr);
  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;
//  void draw(QPainter *p, int ,int *, int *);
//  void axes(QPainter *p);
  const int Nbins = 256;
  int *xData;
  int *yData;
  hist16bit histogram;
  range16 histrange;

  QPainter *qp;
protected:
  void paintEvent(QPaintEvent *e) override;
private:
  range16 findrange(hist16bit);
  void makehist(hist16bit, range16, int*, int*);


//  QImage img;
//  void dataGen();

//signals:

};
#endif // HIST16PAINTER_H
