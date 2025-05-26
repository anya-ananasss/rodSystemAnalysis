#include "basicrodview.h"
QRect BasicRodView::getRect() const { return rect; }
double BasicRodView::getForceValue() const { return forceValue; }
double BasicRodView::getQ() const { return q; }
double BasicRodView::getTrueHeight() const { return trueHeight; }
double BasicRodView::getTrueWidth() const { return trueWidth; }

void BasicRodView::setRect(const QRect &rect) { this->rect = rect; }
void BasicRodView::setForceValue(double forceValue) {
  this->forceValue = forceValue;
}
void BasicRodView::setQ(double q) { this->q = q; }

void BasicRodView::setTrueHeight(double trueHeight) { this->trueHeight = trueHeight; }

void BasicRodView::setTrueWidth(double trueWidth) { this->trueWidth = trueWidth; }

BasicRodView::BasicRodView(QRect &rect, double forceValue, double q) {
  this->rect = rect;
  this->forceValue = forceValue;
  this->q = q;
}
