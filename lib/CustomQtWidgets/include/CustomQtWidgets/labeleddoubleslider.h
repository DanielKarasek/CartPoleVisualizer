#ifndef LABELEDDOUBLESLIDER_H
#define LABELEDDOUBLESLIDER_H

#include "CustomQtWidgets/doublehintslider.h"
#include <QWidget>
#include <QLabel>

class LabeledDoubleSlider : public QWidget
{
  Q_OBJECT
public:

  explicit LabeledDoubleSlider(QString label = "label", QWidget *parent = nullptr);
  explicit LabeledDoubleSlider(double min, double max, double tick_size, Qt::Orientation orientation, QString label = "label", QWidget *parent = nullptr);

  void set_value(double new_value);

public slots:
  void change_number_label(double new_value);

signals:
  void double_value_changed(double new_value);

protected:


  DoubleHintSlider *m_slider;
  QLabel *m_number_label;

  int m_precision;
};

#endif // LABELEDDOUBLESLIDER_H
