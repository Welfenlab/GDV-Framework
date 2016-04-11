#ifndef FRAMEWORKEXAMPLE_H
#define FRAMEWORKEXAMPLE_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/

#include "interfaces/RendererBase.h"

// Sämtliche Abgaben sind von RendererBase abgeleitet
// Wenn Funktionalität einer vorhergegangenen Abgabe benötigt wird, so kann
// auch von dieser abgeleitet werden (anstelle von RendererBase)
// In diesem Fall ist es jedoch wichtig, die zu übernehmenden Methoden
// mit dem Schlüsselwort "virtual" zu versehen.
class FrameworkExample : public RendererBase
{
public:
    // Konstruktor und Destruktor müssen auf jeden Fall deklariert werden,
    // auch wenn die Implementierung ggf. leer ist!
    FrameworkExample();
    virtual ~FrameworkExample();

    // Zwingend zu implementierende Methoden aus dem RendererBase-Interface
    virtual void setupGUI(GdvGui& userInterface);
    virtual void initialize();
    virtual void render(GdvCanvas& canvas);
    virtual void deinitialize();
    virtual void sizeChanged(unsigned int width, unsigned int height);
    virtual bool usesOpenGL();

    void alert();
    void select(int index);

    // Optionale Methoden aus RendererBase
    virtual void wheelMoved(int delta);
    virtual void keyPressed(QString key);
    virtual void keyReleased(QString key);

protected:
    // Eigene lokale Methoden
    float prettyFunction(int x, int y);
    QVector3D mixColors(QVector3D first, QVector3D second, float amount);

    // Variablen
    unsigned int viewWidth, viewHeight;
    float time;

    // Mit GUI-Elementen verknüpfte Variablen
    bool animate;
    int timeStep;
    int amplitude;
    int size;
    QVector3D firstColor;
    QVector3D secondColor;
};

#endif // FRAMEWORKEXAMPLE_H
