/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/

#include "slotmapper.h"
#include <QColor>
#include <QDebug>

SlotMapper::SlotMapper(bool& mappedValue):
    QObject(0),
    boolValue(&mappedValue), intValue(0), colorValue(0), stringValue(0),
    lastBool(mappedValue)
{

}

SlotMapper::SlotMapper(int& mappedValue):
    QObject(0),
    boolValue(0), intValue(&mappedValue), colorValue(0), stringValue(0),
    lastInt(mappedValue)
{

}

SlotMapper::SlotMapper(QVector3D &mappedValue):
    QObject(0),
    boolValue(0), intValue(0), colorValue(&mappedValue), stringValue(0),
    lastColor(mappedValue)
{
}

SlotMapper::SlotMapper(QString& mappedValue):
    QObject(0),
    boolValue(0), intValue(0), colorValue(0), stringValue(&mappedValue),
    lastString(mappedValue)
{

}

void SlotMapper::mapBool(bool value)
{
    lastBool = value;
    if(boolValue)
        *boolValue = value;
}

void SlotMapper::mapInteger(int value)
{
    lastInt = value;
    if(intValue)
        *intValue = value;
}

void SlotMapper::mapColor(QColor value)
{
    lastColor = QVector3D(value.redF(), value.greenF(), value.blueF());
    if(colorValue)
    {
        *colorValue = lastColor;
        QString styleSheet("background-color:rgb(");
        styleSheet += QString::number(int(lastColor.x()*255)) + ",";
        styleSheet += QString::number(int(lastColor.y()*255)) + ",";
        styleSheet += QString::number(int(lastColor.z()*255)) + ");";
        emit styleSheetChanged(styleSheet);
    }
}

void SlotMapper::mapString(QString value)
{
    lastString = value;
    if(stringValue)
        *stringValue = value;
}

void SlotMapper::mapToWidget()
{
    if(boolValue && lastBool != *boolValue)
    {
        lastBool = *boolValue;
        emit boolChanged(lastBool);
    }

    if(intValue && lastInt != *intValue)
    {
        lastInt = *intValue;
        emit intChanged(lastInt);
    }

    if(colorValue && lastColor != *colorValue)
    {
        lastColor = *colorValue;
        QString styleSheet("background-color:rgb(");
        styleSheet += QString::number(int(lastColor.x()*255)) + ",";
        styleSheet += QString::number(int(lastColor.y()*255)) + ",";
        styleSheet += QString::number(int(lastColor.z()*255)) + ");";
        emit colorChanged(lastColor);
        emit styleSheetChanged(styleSheet);
    }

    if(stringValue && lastString != *stringValue)
    {
        lastString = *stringValue;
        emit stringChanged(lastString);
    }
}



ActionEventMapper::ActionEventMapper(std::function<void()>& event) :
    eventCall(event)
{

}


void ActionEventMapper::mapEvent()
{
  eventCall();
}

SelectionEventMapper::SelectionEventMapper(std::function<void(int)>& event) :
    eventCall(event)
{

}


void SelectionEventMapper::mapEvent(int index)
{
  eventCall(index);
}
