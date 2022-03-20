#include "settingbar.h"

#include <iostream>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
SettingBar::SettingBar(QWidget *parent)
  : QWidget{parent}
{
  //combo_box -> change_solver;
  //speed labeled_slider
  QVBoxLayout *layout = new QVBoxLayout(this);

  m_solver_combo = new QComboBox(this);
  m_speed_slider = new LabeledDoubleSlider(0.2, 3, 0.01, Qt::Horizontal, "Speed", this);
  m_pause_button = new QPushButton(style()->standardPixmap(QStyle::SP_MediaPause), "Pause", this);

  m_speed_slider->set_value(1);

  QObject::connect(m_pause_button, &QPushButton::pressed, this, &SettingBar::_pause_button_pressed);

  QObject::connect(m_pause_button, &QPushButton::pressed, this, [this](){emit pause_pushed();});
  QObject::connect(m_solver_combo, &QComboBox::currentTextChanged, this, [this](const QString &text){emit solver_combo_text_change(text.toStdString());});
  QObject::connect(m_speed_slider, &LabeledDoubleSlider::double_value_changed, this, [this](double value){emit speed_value_changed(value);});

  QLabel *title = new QLabel("Game settings");
  QLabel *solver_label = new QLabel ("Current solver");

  auto font = title->font();
  font.setBold(true);
  font.setPixelSize(20);
  title->setFont(font);

  layout->addWidget(title);
  layout->addWidget(solver_label);
  layout->addWidget(m_solver_combo);
  layout->addWidget(m_speed_slider);
  layout->addWidget(m_pause_button);

  layout->setAlignment(Qt::AlignTop);

  this->setLayout(layout);
}


void SettingBar::add_solver_value(const std::string &new_value)
{
  int pos = _combo_text2id(new_value);
  if (pos == -1)
    m_solver_combo->addItem(QString::fromStdString(new_value));
}
void SettingBar::remove_solver_value(const std::string &value)
{
  int pos = _combo_text2id(value);
  if (pos != -1)
    m_solver_combo->removeItem(pos);
}

void SettingBar::set_solver_value(const std::string &value)
{
  int pos = _combo_text2id(value);
  if (pos == -1)
    m_solver_combo->setCurrentIndex(0);
  else
    m_solver_combo->setCurrentIndex(pos);

}


void SettingBar::_pause_button_pressed()
{
  auto val {style()->standardPixmap(QStyle::SP_MediaPause)};
  struct icon_pair{QPixmap icon;
                  std::string text;};
  const std::array<icon_pair, 2> icon_text {style()->standardPixmap(QStyle::SP_MediaPause), "Pause",
                                            style()->standardPixmap(QStyle::SP_MediaPlay), "Resume"};

  static int index = 0;
  index = (index+1)%2;
  m_pause_button->setText(QString::fromStdString(icon_text[index].text));
  m_pause_button->setIcon(icon_text[index].icon);
}


int SettingBar::_combo_text2id(const std::string &value)
{
  for(int i{0}; i < m_solver_combo->count() ;i++)
  {
    if (!QString::compare(m_solver_combo->itemText(i), QString::fromStdString(value), Qt::CaseInsensitive))
    {
      return i;
    }
  }
  return -1;
}
