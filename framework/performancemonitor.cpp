/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#include "performancemonitor.h"

PerformanceMonitor::PerformanceMonitor()
{
    reset();
    timer.start();
}

void PerformanceMonitor::startFrame()
{
    timer.restart();
}

void PerformanceMonitor::stopFrame()
{
    double secs = timer.nsecsElapsed() * 1.0e-9;

    _currentFPS = 1.0 / secs;

    _averageFPS = 0.1 * _currentFPS + (1.0 - 0.1) * _averageFPS;
}

void PerformanceMonitor::reset()
{
    _averageFPS = 0.0;
    _currentFPS = 0.0;
    frameCounter = 0;
}

float PerformanceMonitor::currentFPS()
{
    return _currentFPS;
}

float PerformanceMonitor::averageFPS()
{
    return _averageFPS;
}
