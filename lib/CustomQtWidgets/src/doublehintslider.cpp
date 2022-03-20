#include "CustomQtWidgets/doublehintslider.h"


#include <QSlider>
#include <QStyleOptionSlider>
#include <QString>
#include <QToolTip>

DoubleHintSlider::DoubleHintSlider(QWidget *parent)
  : DoubleHintSlider{0,1,0.1, Qt::Horizontal, false, parent}
{
}

DoubleHintSlider::DoubleHintSlider(double min, double max, double tick_size, Qt::Orientation orientation, bool show_hint, QWidget *parent)
  : QSlider{orientation, parent},
    m_min{min},
    m_max{max},
    m_tick_size{tick_size},
    m_show_hint{show_hint}
{
  QObject::connect(this, &DoubleHintSlider::valueChanged, this, [this](int value)
                                                                {
                                                                  emit this->double_value_changed(this->get_double_value());
                                                                });
  int tick_count = (max - min)/tick_size;
  this->setMinimum(0);
  this->setMaximum(tick_count);
  this->setTickInterval(1);
}

double DoubleHintSlider::get_double_value()
{
  return (m_tick_size * this->value()) + m_min;
}



void DoubleHintSlider::set_value(double new_value)
{
  new_value = std::min(std::max(new_value, m_min), m_max);
  setValue((new_value - m_min)/m_tick_size);
}


double DoubleHintSlider::get_min()
{
  return m_min;
}

double DoubleHintSlider::get_max()
{
  return m_max;
}

void DoubleHintSlider::set_show_hint(bool should_show)
{
  m_show_hint = should_show;
}

void DoubleHintSlider::sliderChange(SliderChange change)
{
  QSlider::sliderChange(change);
  if(change == QAbstractSlider::SliderValueChange && m_show_hint)
  {
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    QPoint bottomRightCorner = sr.bottomLeft();

    QToolTip::showText(mapToGlobal(QPoint( bottomRightCorner.x(), bottomRightCorner.y())) , QString::number(get_double_value()), this);
  }
}


