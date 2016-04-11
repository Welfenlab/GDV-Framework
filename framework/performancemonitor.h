#ifndef PERFORMANCEMONITOR_H
#define PERFORMANCEMONITOR_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 ** Changes:
 ** 2015/03 (r2) - Use QElapsedTimer for higher precision
 **
 **/

#include <QElapsedTimer>

/**
 * @brief Die PerformanceMonitor Klasse
 *
 * Wird verwendet, um die Framerate des aktiven Renderers zu ermitteln
 * Interne Klasse, bitte nicht direkt einbinden und/oder verändern!
 */
class PerformanceMonitor
{
public:
    PerformanceMonitor();

    void startFrame();
    void stopFrame();

    void reset();

    float currentFPS();
    float averageFPS();

private:
    long frameCounter;
    QElapsedTimer timer;
    float _averageFPS;
    float _currentFPS;
};

#endif // PERFORMANCEMONITOR_H
