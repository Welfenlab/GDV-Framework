#ifndef GDVCANVAS2D_H
#define GDVCANVAS2D_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#include "interfaces/GdvCanvas.h"

#include <QWidget>
#include <QImage>

/**
 * @brief Die GdvCanvas2D Klasse
 *
 * Ermöglicht direktes Zeichnen in einen Puffer. Implementiert das GdvCanvas-
 * Interface.
 *
 * Interne Klasse, bitte nicht direkt einbinden und/oder verändern!
 */
class GdvCanvas2D : public QWidget, public GdvCanvas
{
    Q_OBJECT
public:
    explicit GdvCanvas2D(QWidget *parent = 0);

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

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent*);

    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void wheelEvent(QWheelEvent*);

    virtual void keyPressEvent(QKeyEvent*);
    virtual void keyReleaseEvent(QKeyEvent*);

    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent*);


private:
    QImage buffer2D;
    QImage currentBuf;

    unsigned int lastY = INT_MAX;
    QRgb* lastLine = 0;
};

#endif // GDVCANVAS2D_H
