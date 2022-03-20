#ifndef SETTINGBAR_H
#define SETTINGBAR_H

#include "CustomQtWidgets/labeleddoubleslider.h"

#include <QWidget>
#include <QComboBox>

#include <QPushButton>

class SettingBar : public QWidget
{
  Q_OBJECT
public:
  explicit SettingBar(QWidget *parent = nullptr);

  void add_solver_value(const std::string &value);
  void remove_solver_value(const std::string &value);

  void set_solver_value(const std::string &value);

protected:

  int _combo_text2id(const std::string &value);

  QComboBox *m_solver_combo;
  QPushButton *m_pause_button;
  LabeledDoubleSlider *m_speed_slider;

private slots:
  void _pause_button_pressed();

signals:
  void pause_pushed();
  void solver_combo_text_change(const std::string &text);
  void speed_value_changed(double new_value);
};

#endif // SETTINGBAR_H
