#include "CustomQtWidgets/labeleddoubleslider.h"
#include "CustomQtWidgets/doublehintslider.h"


#include <string>
#include <cmath>

#include <iostream>
#include <sstream>
#include <iomanip>

#include <QString>
#include <QWidget>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>

LabeledDoubleSlider::LabeledDoubleSlider(QString label, QWidget *parent)
  : LabeledDoubleSlider{0.0, 10.0, 0.1, Qt::Horizontal, label, parent}
{

}

LabeledDoubleSlider::LabeledDoubleSlider(double min, double max, double tick_size, Qt::Orientation orientation, QString label, QWidget *parent)
  : QWidget{parent}, m_precision(std::ceil(-std::log10(tick_size))+1)
{
  QWidget *widget_horizontal = new QWidget(this);

  QVBoxLayout *layout_vertical = new QVBoxLayout(this);
  QHBoxLayout *layout_horizontal = new QHBoxLayout(widget_horizontal);
  std::stringstream ss;
  ss << std::fixed << std::setprecision(m_precision) << max;
  std::string max_string = ss.str();
  m_number_label = new QLabel(QString::fromStdString(max_string));
  int w = m_number_label->fontMetrics().size(Qt::TextSingleLine, m_number_label->text()).width();
  m_number_label->setMinimumWidth(w+10);
  QLabel *upper_label = new QLabel(label);

  m_slider = new DoubleHintSlider(min, max, tick_size, orientation);

  m_slider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  m_number_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  upper_label->setAlignment(Qt::AlignCenter);
  layout_horizontal->addWidget(m_slider);
  layout_horizontal->addWidget(m_number_label);
  widget_horizontal->setLayout(layout_horizontal);

  layout_vertical->addWidget(upper_label);
  layout_vertical->addWidget(widget_horizontal);

  layout_vertical->setAlignment(Qt::AlignTop);


  QWidget::connect(m_slider, &DoubleHintSlider::double_value_changed, this, [this](double value)
                                                                                  {
                                                                                    emit double_value_changed(value);
                                                                                  });
  QWidget::connect(m_slider, &DoubleHintSlider::double_value_changed, this, &LabeledDoubleSlider::change_number_label);

  this->setLayout(layout_vertical);

}

void LabeledDoubleSlider::set_value(double new_value)
{
  m_slider->set_value(new_value);
}


void LabeledDoubleSlider::change_number_label(double new_val)
{

  std::stringstream ss;
  ss << std::fixed << std::setprecision(m_precision) << new_val;
  std::string str_val = ss.str();

  m_number_label->setText(QString::fromStdString(str_val));
}
