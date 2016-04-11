/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#include "gdvcanvas2d.h"

#include <QColor>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

GdvCanvas2D::GdvCanvas2D(QWidget *parent) :
    QWidget(parent)
{
}

void GdvCanvas2D::setPixel(unsigned int x, unsigned int y, const QVector3D& color)
{
    if(x >= static_cast<unsigned int>(width()) || y >= static_cast<unsigned int>(height()))
    {
        qDebug() << "Warning: Drawing outside of viewport! (" << x << "," << y << ")";
        return;
    }

    if(y != lastY)
    {
        lastLine = reinterpret_cast<QRgb*>(buffer2D.scanLine(y));
        lastY = y;
    }

    *(lastLine+x) = qRgb(color.x()*255, color.y()*255, color.z()*255);
}

void GdvCanvas2D::clearBuffer(const QVector3D &clearColor)
{
    buffer2D.fill(QColor(clearColor.x()*255,
                         clearColor.y()*255,
                         clearColor.z()*255));
}

void GdvCanvas2D::flipBuffer()
{
    currentBuf = buffer2D;
}

void GdvCanvas2D::flipBuffer(const QImage& buffer)
{
    currentBuf = buffer.copy(); // Evtl. ist das copy nicht notwendig...
}

void GdvCanvas2D::paintEvent(QPaintEvent* pe)
{
    Q_UNUSED(pe);
    QPainter p(this);
    p.drawImage(0,0, currentBuf);
}

void GdvCanvas2D::resizeEvent(QResizeEvent* pe)
{
    Q_UNUSED(pe);
    buffer2D = QImage(this->size(), QImage::Format_RGB32);
    emit sizeChanged(this->width(), this->height());
}

void GdvCanvas2D::mousePressEvent(QMouseEvent* e)
{
    emit mousePressed(e->pos().x(), e->pos().y());
}

void GdvCanvas2D::mouseReleaseEvent(QMouseEvent* e)
{
    emit mouseReleased(e->pos().x(), e->pos().y());
}

void GdvCanvas2D::mouseMoveEvent(QMouseEvent* e)
{
    emit mouseMoved(e->pos().x(), e->pos().y());
}

void GdvCanvas2D::wheelEvent(QWheelEvent* e)
{
    emit wheelMoved(e->delta());
}

void GdvCanvas2D::keyPressEvent(QKeyEvent* e)
{
    qDebug() << "Key pressed:" << e->text();
    emit keyPressed(e->text());
}

void GdvCanvas2D::keyReleaseEvent(QKeyEvent* e)
{
    emit keyReleased(e->text());
}

void GdvCanvas2D::enterEvent(QEvent*)
{
    this->grabKeyboard();
}

void GdvCanvas2D::leaveEvent(QEvent*)
{
    this->releaseKeyboard();
}
