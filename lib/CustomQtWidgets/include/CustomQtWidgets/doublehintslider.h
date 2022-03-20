#ifndef DOUBLEHINTSLIDER_H
#define DOUBLEHINTSLIDER_H

#include <QObject>
#include <QSlider>

class DoubleHintSlider : public QSlider
{
  Q_OBJECT
public:
  explicit DoubleHintSlider(QWidget *parent = nullptr);
  explicit DoubleHintSlider(double min, double max, double tick_size, Qt::Orientation orientation = Qt::Horizontal, bool show_hint = false, QWidget *parent = nullptr);

  double get_min();
  double get_max();
  double get_double_value();

  void set_value(double new_value);
  void set_show_hint(bool should_show);

protected:
  double m_min{10};
  double m_max{1000};
  double m_tick_size{0.1};

  bool m_show_hint{false};

  virtual void sliderChange(SliderChange change) override;

signals:
  void double_value_changed(double value);

};

#endif // DOUBLEHINTSLIDER_H
