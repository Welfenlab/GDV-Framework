/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#include "gdvcanvas3d.h"
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>

GdvCanvas3D::GdvCanvas3D(QGLFormat format) :
    QGLWidget(format)
{
}

void GdvCanvas3D::setPixel(unsigned int x, unsigned int y, const QVector3D &color)
{
    Q_UNUSED(x); Q_UNUSED(y); Q_UNUSED(color);
    qWarning() << "'setPixel' is not supported in OpenGL mode. Please use the appropriate OpenGL functions.";
}

void GdvCanvas3D::clearBuffer(const QVector3D &clearColor)
{
    Q_UNUSED(clearColor);
    qWarning() << "'clearBuffer' is not supported in OpenGL mode. Please use the appropriate OpenGL functions.";
}

void GdvCanvas3D::flipBuffer()
{
    qWarning() << "'flipBuffer' is not supported in OpenGL mode. Please use the appropriate OpenGL functions.";
}

void GdvCanvas3D::flipBuffer(const QImage& buffer)
{
    Q_UNUSED(buffer);
    qWarning() << "'flipBuffer' is not supported in OpenGL mode. Please use the appropriate OpenGL functions.";
}

void GdvCanvas3D::initializeGL()
{

}

void GdvCanvas3D::paintGL()
{
    emit drawGL();
}

void GdvCanvas3D::resizeGL(int width, int height)
{
    emit sizeChanged(width, height);
}

void GdvCanvas3D::mousePressEvent(QMouseEvent* e)
{
    emit mousePressed(e->pos().x(), e->pos().y());
}

void GdvCanvas3D::mouseReleaseEvent(QMouseEvent* e)
{
    emit mouseReleased(e->pos().x(), e->pos().y());
}

void GdvCanvas3D::mouseMoveEvent(QMouseEvent* e)
{
    emit mouseMoved(e->pos().x(), e->pos().y());
}

void GdvCanvas3D::wheelEvent(QWheelEvent* e)
{
    emit wheelMoved(e->delta());
}


void GdvCanvas3D::keyPressEvent(QKeyEvent* e)
{
    qDebug() << "Key pressed:" << e->text();
    emit keyPressed(e->text());
}

void GdvCanvas3D::keyReleaseEvent(QKeyEvent* e)
{
    emit keyReleased(e->text());
}

void GdvCanvas3D::enterEvent(QEvent*)
{
    this->grabKeyboard();
}

void GdvCanvas3D::leaveEvent(QEvent*)
{
    this->releaseKeyboard();
}

