#include "drawingwidget.h"
#include "basicrodview.h"
#include <QMessageBox>
#include <QPushButton>

DrawingWidget::DrawingWidget(QWidget *parent)
    : QWidget(parent), isSelected(false), isResizable(false),
      selectedRectIndex(-1) {
  initDefault();
}

DrawingWidget::~DrawingWidget() {}

void DrawingWidget::initDefault() {
  QRect defaultRod = QRect(0, 35, DEFAULT_WIDTH, DEFAULT_HEIGHT);
  rods.append(BasicRodView(defaultRod, 0, 0));
  rods[rods.length() - 1].setTrueLength(DEFAULT_HEIGHT * PX_TO_METERS_COEFF);
  rods[rods.length() - 1].setTrueWidth(DEFAULT_WIDTH * PX_TO_MMS_COEFF);
  update();
}

void DrawingWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  // draw upper line
  int centerX = width() / 2;
  int lineLength = DEFAULT_WIDTH + 35;
  int startX = centerX - lineLength / 2;
  int endX = centerX + lineLength / 2;

  painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
  painter.drawLine(QPoint(startX, 35), QPoint(endX, 35));

  int dashStartX = startX;
  while (dashStartX < endX) {
    painter.drawLine(QPoint(dashStartX, 35), QPoint(dashStartX + 10, 20));
    dashStartX += 10;
  }

  // draw rects and forces
  for (int i = 0; i < rods.size(); ++i) {
    BasicRodView &rodObject = rods[i];
    painter.setPen(QPen(DEFAULT_COLOR, 3, Qt::SolidLine, Qt::FlatCap));

    QRect currRect = rodObject.getRect();
    if (currRect.width() < 10) {
      currRect.setWidth(10);
    }
    if (currRect.height() < 10) {
      currRect.setHeight(10);
    }
    double centeredX = (width() - currRect.width()) / 2;
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

    // Draw force arrow
    double forceValue = rodObject.getForceValue();

    if (forceValue != 0) {
      painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::FlatCap));
      double arrowX = centeredX + (currRect.width() / 2);
      double arrowY = currRect.top();
      int arrowLength = 30; // Length of the arrow

      if (forceValue > 0) {
        // Arrow pointing upwards
        painter.drawLine(QPoint(arrowX, arrowY),
                         QPoint(arrowX, arrowY - arrowLength));
        painter.drawLine(QPoint(arrowX, arrowY - arrowLength),
                         QPoint(arrowX - 5, arrowY - arrowLength + 5));
        painter.drawLine(QPoint(arrowX, arrowY - arrowLength),
                         QPoint(arrowX + 5, arrowY - arrowLength + 5));
      } else {
        // Arrow pointing downwards
        painter.drawLine(QPoint(arrowX, arrowY),
                         QPoint(arrowX, arrowY + arrowLength));
        painter.drawLine(QPoint(arrowX, arrowY + arrowLength),
                         QPoint(arrowX - 5, arrowY + arrowLength - 5));
        painter.drawLine(QPoint(arrowX, arrowY + arrowLength),
                         QPoint(arrowX + 5, arrowY + arrowLength - 5));
      }

      // Draw force label
      QString forceLabel = QString("F=%1 кН").arg(forceValue);
      painter.drawText(QPoint(arrowX + 10, arrowY - arrowLength / 2),
                       forceLabel);
    }
  }
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
      emit updateRectParams(rods[selectedRectIndex].getTrueLength(),
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
    QRect rect = rods[selectedRectIndex].getRect();
    if (rect.contains(event->pos())) {
      int dx = event->pos().x() - lastMousePosition.x();
      int dy = event->pos().y() - lastMousePosition.y();
      if (rect.width() + dx > 10 && rect.height() + dy > 10) {

        double length = rect.height() * PX_TO_METERS_COEFF;
        double width = rect.width() * PX_TO_MMS_COEFF;
        int newX = rect.x() - dx / 2;

        rods[selectedRectIndex].setRect(
            QRect(newX, rect.y(), rect.width() + dx, rect.height() + dy));
        emit updateRectParams(length, width);
        lastMousePosition = event->pos();
        updateOtherRods();
        update();
      }
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
  rods[rods.length() - 1].setTrueLength(DEFAULT_HEIGHT * PX_TO_METERS_COEFF);
  rods[rods.length() - 1].setTrueWidth(DEFAULT_WIDTH * PX_TO_MMS_COEFF);
  update();
}

void DrawingWidget::on_lengthEdit_textEdited(const QString &arg1) //вычисления?
{
  bool isNum = arg1.toDouble();

  if (isNum && selectedRectIndex != -1 && arg1.toDouble() > 0) {
    QRect currRect = rods[selectedRectIndex].getRect();
    rods[selectedRectIndex].setRect(
        QRect(currRect.x(), currRect.y(), currRect.width(),
              arg1.toDouble() * METERS_TO_PX_COEFF));
    rods[selectedRectIndex].setTrueLength(arg1.toDouble());
    update();
  }
}

void DrawingWidget::on_radEdit_textEdited(const QString &arg1) {
  bool isNum = arg1.toDouble();
  if (isNum && selectedRectIndex != -1 && arg1.toDouble() > 0) {
    QRect currRect = rods[selectedRectIndex].getRect();
    rods[selectedRectIndex].setRect(QRect(currRect.x(), currRect.y(),
                                          arg1.toDouble() * MMS_TO_PX_COEFF,
                                          currRect.height()));
    rods[selectedRectIndex].setTrueWidth(arg1.toDouble());
    update();
  }
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


//TODO: максимальная длина/ширина, работа с силой на конце
