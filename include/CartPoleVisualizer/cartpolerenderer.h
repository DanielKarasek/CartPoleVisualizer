#ifndef CARTPOLERENDERER_H
#define CARTPOLERENDERER_H

#include "../../lib/Environments/include/Environments/CartPole/CartPole.h"

#include <QWidget>
#include <QImage>

class CartPoleRenderer : public QWidget
{
  Q_OBJECT
public:
  explicit CartPoleRenderer(QWidget *parent = nullptr);

public slots:
  void render(const CartPoleState *state2render);
  void set_constraints(CartRenderConstraints constraints);

protected:

  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

  void draw_cart(double render_height_pole, double cart_mid_position, QPainter &painter);
  void draw_pole(double render_height_pole, double cart_mid_position, double degree_angle, QPainter &painter);


  QImage m_image;

  CartRenderConstraints m_constraints;

signals:
  void key_event(QKeyEvent *event);
};

#endif // CARTPOLERENDERER_H
