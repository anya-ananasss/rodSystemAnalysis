#pragma once
#ifndef BASICRODVIEW_H
#include <QRect>
#define BASICRODVIEW_H

class BasicRodView {
private:
  QRect rect;
  double forceValue;
  double q;
  double trueHeight;
  double trueWidth;

public:
  BasicRodView(QRect &rect, double forceValue, double q);

  // Геттеры
  QRect getRect() const;
  double getForceValue() const;
  double getQ() const;
  double getTrueHeight() const;
  double getTrueWidth() const;

  // Сеттеры
  void setRect(const QRect &rect);
  void setForceValue(double forceValue);
  void setQ(double q);
  void setTrueHeight(double trueHeight);
  void setTrueWidth(double trueWidth);
};

#endif // BASICRODVIEW_H
