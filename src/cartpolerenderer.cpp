#include "cartpolerenderer.h"
#include "Environments/CartPole/CartPole.h"
#include "auxilaries.h"

#include <QObject>
#include <QPainter>
#include <QTimer>

#include <QPaintEvent>
#include <QResizeEvent>
#include <QKeyEvent>

#include <cmath>
#include <iostream>

CartPoleRenderer::CartPoleRenderer(QWidget *parent)
  : QWidget{parent}
{
  this->grabKeyboard();
}

void CartPoleRenderer::set_constraints(CartRenderConstraints constraints)
{
  m_constraints = constraints;
}

void CartPoleRenderer::render(const CartPoleState *state2render)
{
  int width = this->size().width();
  double render_height_pole = width * 0.5 * (m_constraints.pole_lenght/m_constraints.x_limit);
  int height = 1.5 * render_height_pole;

  m_image = QImage(width, height, QImage::Format_ARGB32);
  QPainter painter(&m_image);
  painter.fillRect(m_image.rect(), {255, 255, 255});

  QPen pen({0, 0, 0});
  pen.setWidth(3);
  painter.setPen(pen);
  painter.drawLine(0,height,width,height);

  double cart_mid_position = state2render->x + m_constraints.x_limit;
  cart_mid_position *= ((0.5*width)/m_constraints.x_limit);

  draw_pole(render_height_pole, cart_mid_position, radians2degree(state2render->theta), painter);
  draw_cart(render_height_pole, cart_mid_position, painter);

  update();
}

void CartPoleRenderer::draw_cart(double render_height_pole, double cart_mid_position, QPainter &painter)
{
  int cart_height = 0.2 * render_height_pole;
  int cart_width = 0.4 * render_height_pole;
  int cart_y = 1.3 * render_height_pole;
  int cart_x = cart_mid_position - cart_width/2;

  painter.fillRect(cart_x, cart_y, cart_width, cart_height, {0, 0, 0});
}
void CartPoleRenderer::draw_pole(double render_height_pole, double cart_mid_position, double degree_angle, QPainter &painter)
{
  int pole_width = render_height_pole/10;
  painter.translate(cart_mid_position+pole_width/2, 1.4 * render_height_pole);
  painter.rotate(180+degree_angle);

  painter.fillRect(0, 0, pole_width, render_height_pole* 1.1, {255,0,0});
  painter.resetTransform();
}


void CartPoleRenderer::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  QRect tmp_rect = event->rect();
  painter.drawImage(tmp_rect, m_image, tmp_rect);
}

void CartPoleRenderer::resizeEvent(QResizeEvent *event)
{

}
void CartPoleRenderer::keyPressEvent(QKeyEvent *event)
{
  emit key_event(event);
}

void CartPoleRenderer::keyReleaseEvent(QKeyEvent *event)
{
  emit key_event(event);
}
