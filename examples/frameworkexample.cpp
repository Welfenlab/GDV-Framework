/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#include "frameworkexample.h"
#include <QMessageBox>

FrameworkExample::FrameworkExample()
{
}

FrameworkExample::~FrameworkExample()
{

}

void FrameworkExample::setupGUI(GdvGui& userInterface)
{
    userInterface.addCheckBox("Animation", true, animate);
    userInterface.addSlider("Geschwindigkeit", 0, 1000, 50, timeStep);
    userInterface.addSlider("Amplitude", 0, 100, 50, amplitude);
    userInterface.addSeparator();
    userInterface.addColorSelector("Erste Farbe", QVector3D(1.0, 0.0, 0.0), firstColor);
    userInterface.addColorSelector("Zweite Farbe", QVector3D(0.0, 1.0, 1.0), secondColor);
    userInterface.addSlider("Groesse", 10, 300, 80, size);

    userInterface.addButton("Info", callAction(alert));
    userInterface.addDropdownList("TestA;TestB;TestC", 2, callSelection(select));
}

void FrameworkExample::alert()
{
    QMessageBox::aboutQt(0, "Test!");
}

void FrameworkExample::select(int index)
{
    QMessageBox::information(0, "Test!", QString::number(index));
}

void FrameworkExample::initialize()
{
    time = 0.0f;
}

float FrameworkExample::prettyFunction(int x, int y)
{
    float xf = x * 0.1f;
    float yf = y * 0.1f;

    yf -= 20.0f * size;

    float a = sqrtf(xf*xf + yf*yf) - time;

    xf += 20.0f * size;
    yf += 40.0f * size;

    float b = sqrtf(xf*xf + yf*yf) - time * 0.25f;

    xf -= 60.0f * size;

    float c = sqrtf(xf*xf + yf*yf) - time * 0.5f;

    return sinf(a) + sinf(b) + sinf(c);
}

QVector3D FrameworkExample::mixColors(QVector3D first, QVector3D second, float amount)
{
    amount = qMin(1.0f, amount);
    amount = qMax(-1.0f, amount);

    amount = (amount + 1.0f) * 0.5f;

    QVector3D result(
                first.x() * amount + second.x() * (1.0f - amount),
                first.y() * amount + second.y() * (1.0f - amount),
                first.z() * amount + second.z() * (1.0f - amount));

    return result;
}

void FrameworkExample::render(GdvCanvas& canvas)
{
    if(animate)
        time = time + timeStep/100.0f * 0.1f;

    int midX = viewWidth / 2;
    int midY = viewHeight / 2;

    float amplitude = this->amplitude * 0.01;

    canvas.clearBuffer(QVector3D(1.0, 1.0, 1.0));

    for(int y = midY - size; y <= midY + size; y++)
    {
        for(int x = midX - size; x <= midX + size; x++)
        {
            float value = prettyFunction(midX - x, midY - y) * amplitude;
            QVector3D color = mixColors(firstColor, secondColor, value);
            canvas.setPixel(x, y, color);
        }
    }

    canvas.flipBuffer();
}

void FrameworkExample::deinitialize()
{

}

void FrameworkExample::sizeChanged(unsigned int width, unsigned int height)
{
    viewWidth = width;
    viewHeight = height;
}

bool FrameworkExample::usesOpenGL()
{
    return false;
}

void FrameworkExample::wheelMoved(int delta)
{
    amplitude += delta * 0.05f;
}

void FrameworkExample::keyPressed(QString key)
{
    QMessageBox::information(0, "KeyPressed", "Key pressed: " + key);
}

void FrameworkExample::keyReleased(QString key)
{
    QMessageBox::information(0, "KeyReleased", "Key released: " + key);
}
