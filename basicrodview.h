#pragma once
#ifndef BASICRODVIEW_H
#include <QRect>
#define BASICRODVIEW_H

class BasicRodView {
private:
  QRect rect;
  double forceValue;
  //возможно, объект линии
  double q;
  double trueLength;
  double trueWidth;

public:
  BasicRodView(QRect &rect, double forceValue, double q);

  // Геттеры
  QRect getRect() const;
  double getForceValue() const;
  double getQ() const;
  double getTrueLength() const;
  double getTrueWidth() const;

  // Сеттеры
  void setRect(const QRect &rect);
  void setForceValue(double forceValue);
  void setQ(double q);
  void setTrueLength(double trueLength);
  void setTrueWidth(double trueWidth);
};

#endif // BASICRODVIEW_H
