#include "drawingwidget.h"
#include "basicrodview.h"
#include "diagramParamsCalculation.h"
#include <QMessageBox>
#include <QPushButton>

DrawingWidget::DrawingWidget(QWidget *parent)
    : QWidget(parent), isSelected(false), isResizable(false),
      selectedRectIndex(-1) {

  diagramParamsCalculation = new DiagramParamsCalculation(this);
  initDefault();
  drawDiagrams = false;
}

DrawingWidget::~DrawingWidget() {}

void DrawingWidget::initDefault() {
  int yPos = 35;
  QRect topRod = QRect(0, yPos, 73, 120);
  rods.append(BasicRodView(topRod, 0, 0));
  rods.last().setTrueHeight(120 * PX_TO_METERS_COEFF);
  rods.last().setTrueWidth(73 * PX_TO_MMS_COEFF);

  yPos += 120;
  QRect middleRod = QRect(0, yPos, 91, 200);
  rods.append(BasicRodView(middleRod, 50, 0));
  rods.last().setTrueHeight(200 * PX_TO_METERS_COEFF);
  rods.last().setTrueWidth(91 * PX_TO_MMS_COEFF);

  yPos += 200;
  QRect bottomRod = QRect(0, yPos, 56, 80);
  rods.append(BasicRodView(bottomRod, 20, 0));
  rods.last().setTrueHeight(80 * PX_TO_METERS_COEFF);
  rods.last().setTrueWidth(56 * PX_TO_MMS_COEFF);

  endForce = -30;

  update();
}

void DrawingWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  // draw upper line
  int width = rods[0].getRect().width();
  int lineLength = width + 20;
  int startX = DEFAULT_LEFTCORNER_X + width / 2 - (lineLength / 2);
  int endX = DEFAULT_LEFTCORNER_X + width / 2 + (lineLength / 2);

  painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
  painter.drawLine(QPoint(startX, 35), QPoint(endX, 35));

  int dashStartX = startX;
  while (dashStartX < endX) {
    painter.drawLine(QPoint(dashStartX, 35), QPoint(dashStartX + 10, 20));
    dashStartX += 10;
  }

  // draw rects and forces
  for (int i = 0; i < rods.size(); ++i) {

    painter.setPen(QPen(DEFAULT_COLOR, 3, Qt::SolidLine, Qt::FlatCap));

    BasicRodView &rodObject = rods[i];
    QRect currRect = rodObject.getRect();
    if (currRect.width() < 10) {
      currRect.setWidth(10);
    }
    if (currRect.height() < 10) {
      currRect.setHeight(10);
    }
    double centeredX = DEFAULT_LEFTCORNER_X;
    rodObject.setRect(
        QRect(centeredX, currRect.y(), currRect.width(), currRect.height()));
    painter.drawRect(rodObject.getRect());
    painter.setBrush(QBrush(DEFAULT_FILLING_COLOR));
    painter.drawRect(rodObject.getRect());

    // Draw selection if applicable
    if (isSelected && selectedRectIndex == i) {
      painter.setBrush(QBrush(DEFAULT_SELECTED_COLOR));
      painter.drawRect(rodObject.getRect());
    }

    double forceValue = rodObject.getForceValue();
    drawForceArrow(painter, currRect, forceValue, false);
  }

  // Draw end force if it exists
  if (endForce != 0) {
    // Use the last rod's position for end force
    QRect lastRodRect = rods.last().getRect();
    drawForceArrow(painter, lastRodRect, this->endForce, true);
  }
}

void DrawingWidget::drawForceArrow(QPainter &painter, const QRect &rect,
                                   double forceValue, bool isEndForce) {
  if (forceValue == 0)
    return;

  painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::FlatCap));

  // For end force, we position it at the bottom of the last rod
  double arrowX = DEFAULT_LEFTCORNER_X + (rect.width() / 2);
  double arrowY;
  if (isEndForce) {
    arrowY = rect.bottom();
  } else {
    arrowY = rect.top();
  }
  int arrowLength = DEFAULT_FORCE_HEIGHT;

  if (forceValue > 0) {
    // Arrow pointing upwards (for end force it will be from bottom up)
    painter.drawLine(QPoint(arrowX, arrowY),
                     QPoint(arrowX, arrowY - arrowLength));
    painter.drawLine(QPoint(arrowX, arrowY - arrowLength),
                     QPoint(arrowX - 5, arrowY - arrowLength + 5));
    painter.drawLine(QPoint(arrowX, arrowY - arrowLength),
                     QPoint(arrowX + 5, arrowY - arrowLength + 5));
  } else {
    // Arrow pointing downwards (for end force it will be from top down)
    painter.drawLine(QPoint(arrowX, arrowY),
                     QPoint(arrowX, arrowY + arrowLength));
    painter.drawLine(QPoint(arrowX, arrowY + arrowLength),
                     QPoint(arrowX - 5, arrowY + arrowLength - 5));
    painter.drawLine(QPoint(arrowX, arrowY + arrowLength),
                     QPoint(arrowX + 5, arrowY + arrowLength - 5));
  }

  // Draw force label
  QString forceLabel = QString("F=%1 кН").arg(forceValue);
  QPoint labelPos;
  if (isEndForce) {
    labelPos =
        QPoint(arrowX + 10, arrowY + arrowLength / 2); // Для концевой силы
  } else {
    labelPos = QPoint(arrowX + 10, arrowY - arrowLength / 2); // Для обычных сил
  }

  painter.drawText(labelPos, forceLabel);
}

void DrawingWidget::mousePressEvent(QMouseEvent *event) {
  if (isSelected && !isResizable) {
    isResizable = true;
  }
  bool found = false;
  for (int i = 0; i < rods.length(); ++i) {
    QRect currRect = rods[i].getRect();
    if (currRect.contains(event->pos())) {
      selectedRectIndex = i;
      isSelected = true;
      found = true;
      lastMousePosition = event->pos();
      emit updateRectParams(rods[selectedRectIndex].getTrueHeight(),
                            rods[selectedRectIndex].getTrueWidth());
      emit updateRodSettings(rods[selectedRectIndex].getForceValue(),
                             rods[selectedRectIndex].getQ());
      emit rodSelected();
      break;
    }
  }
  if (!found) {
    if (selectedRectIndex != -1) {
      if (!rods[selectedRectIndex].getRect().contains(event->pos())) {
        isSelected = false;
        isResizable = false;
        emit rodUnselected();
      }
    }
  }
  update();
}

void DrawingWidget::mouseMoveEvent(QMouseEvent *event) {
  if (isResizable) {
    QRect currentRect = rods[selectedRectIndex].getRect();
    if (currentRect.contains(event->pos())) {

      int dx = event->pos().x() - lastMousePosition.x();
      int dy = event->pos().y() - lastMousePosition.y();

      int newPixelWidth = currentRect.width() + dx;
      int newPixelHeight = currentRect.height() + dy;

      double trueNewHeight =
          static_cast<double>(newPixelHeight) * PX_TO_METERS_COEFF;
      double trueNewWidth =
          static_cast<double>(newPixelWidth) * PX_TO_MMS_COEFF;

      int newX = currentRect.x() - dx / 2;

      rods[selectedRectIndex].setRect(
          QRect(newX, currentRect.y(), newPixelWidth, newPixelHeight));

      rods[selectedRectIndex].setTrueHeight(trueNewHeight);
      rods[selectedRectIndex].setTrueWidth(trueNewWidth);

      emit updateRectParams(trueNewHeight, trueNewWidth);

      lastMousePosition = event->pos();
      updateOtherRods();
      update();
    }
  }
}
void DrawingWidget::updateOtherRods() {
  if (selectedRectIndex < rods.size() - 1) {
    int currBottom = rods[selectedRectIndex].getRect().bottom();
    for (int i = selectedRectIndex + 1; i < rods.length(); ++i) {
      QRect lowerRect = rods[i].getRect();
      lowerRect.moveTop(currBottom);
      rods[i].setRect(lowerRect);
      currBottom = lowerRect.bottom();
    }
  }
}

void DrawingWidget::on_addRodButton_clicked() {
  QRect newRod;
  int length = rods.length();
  if (rods.length() > 0) {
    QRect prevRod = rods[rods.length() - 1].getRect();
    newRod = QRect(width() - DEFAULT_WIDTH / 2, prevRod.bottom(), DEFAULT_WIDTH,
                   DEFAULT_HEIGHT);
  } else {
    newRod =
        QRect(width() - DEFAULT_WIDTH / 2, 35, DEFAULT_WIDTH, DEFAULT_HEIGHT);
  }
  rods.append(BasicRodView(newRod, 15, 0));
  rods[rods.length() - 1].setTrueHeight(DEFAULT_HEIGHT * PX_TO_METERS_COEFF);
  rods[rods.length() - 1].setTrueWidth(DEFAULT_WIDTH * PX_TO_MMS_COEFF);
  update();
}

void DrawingWidget::on_lengthEdit_textEdited(const double &arg1) //вычисления?
{

  QRect currRect = rods[selectedRectIndex].getRect();
  rods[selectedRectIndex].setRect(QRect(
      currRect.x(), currRect.y(), currRect.width(), arg1 * METERS_TO_PX_COEFF));
  rods[selectedRectIndex].setTrueHeight(arg1);
  updateOtherRods();
  update();
}

void DrawingWidget::on_radEdit_textEdited(const double &arg1) {
  QRect currRect = rods[selectedRectIndex].getRect();
  rods[selectedRectIndex].setRect(QRect(
      currRect.x(), currRect.y(), arg1 * MMS_TO_PX_COEFF, currRect.height()));
  rods[selectedRectIndex].setTrueWidth(arg1);
  update();
}

void DrawingWidget::on_removeRodButton_clicked() {
  QMessageBox messageBox;
  messageBox.setWindowTitle("Подтверждение удаления");
  messageBox.setText("Вы действительно хотите удалить выбранный стержень?");

  QPushButton *yesButton = messageBox.addButton(tr("Да"), QMessageBox::YesRole);
  QPushButton *noButton = messageBox.addButton(tr("Нет"), QMessageBox::NoRole);

  messageBox.exec();

  if (messageBox.clickedButton() == yesButton) {
    QRect rect = rods[selectedRectIndex].getRect();
    rods[selectedRectIndex].setRect(QRect(rect.x(), rect.y(), rect.width(), 1));
    updateOtherRods();
    isSelected = false;
    isResizable = false;
    emit rodUnselected();
    rods.remove(selectedRectIndex);
    selectedRectIndex = -1;
    update();
  }
}

void DrawingWidget::on_force_updated(double newForce) {
  if (isSelected && selectedRectIndex != -1) {
    rods[selectedRectIndex].setForceValue(newForce);
    update();
  }
}

void DrawingWidget::on_q_updated(double newQ) {
    if (isSelected && selectedRectIndex != -1) {
        rods[selectedRectIndex].setQ(newQ);
    }
}
void DrawingWidget::on_calculateButton_clicked() {
  nValues = diagramParamsCalculation->calculateLongitudinalForces();
  stressValues = diagramParamsCalculation->calculateStresses();
  extensionsValues = diagramParamsCalculation->calculateExtensions();


  emit on_calculations_complete(nValues, stressValues, extensionsValues);
  update();
}
void DrawingWidget::on_endForce_updated(double value) {
  this->endForce = value;
  update();
}

void DrawingWidget::on_resilince_updated(double value) { this->E = value; }

// TODO: максимальная длина/ширина, работа с силой на конце
