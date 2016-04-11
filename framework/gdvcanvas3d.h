#ifndef GDVCANVAS3D_H
#define GDVCANVAS3D_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#include <QGLWidget>
#include "interfaces/GdvCanvas.h"

/**
 * @brief Die GdvCanvas3D Klasse
 *
 * Stellt einen OpenGL Kontext zur Verfügung. Das GdvCanvas-Interface
 * ist leer implementiert, da diese Aufrufe nicht benötigt werden.
 *
 * Interne Klasse, bitte nicht direkt einbinden und/oder verändern!
 */
class GdvCanvas3D : public QGLWidget, public GdvCanvas
{
    Q_OBJECT
public:
    explicit GdvCanvas3D(QGLFormat format);

    virtual void setPixel(unsigned int x, unsigned int y, const QVector3D& color);
    virtual void clearBuffer(const QVector3D& clearColor);
    virtual void flipBuffer();
    virtual void flipBuffer(const QImage& buffer);

signals:
    void sizeChanged(int, int);
    void mousePressed(int, int);
    void mouseReleased(int, int);
    void mouseMoved(int, int);
    void wheelMoved(int);
    void keyPressed(QString);
    void keyReleased(QString);

    void drawGL();

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int width, int height);

    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void wheelEvent(QWheelEvent*);

    virtual void keyPressEvent(QKeyEvent*);
    virtual void keyReleaseEvent(QKeyEvent*);

    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent*);

};

#endif // GDVCANVAS3D_H
