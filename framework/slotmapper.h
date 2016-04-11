#ifndef SLOTMAPPER_H
#define SLOTMAPPER_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/

#include <QObject>
#include <QVector3D>
#include <QString>
#include <QColor>
#include <functional>

/**
 * @brief Die SlotMapper Klasse
 *
 * Wird verwendet, um zwischen GUI und eigenem Code zu vermitteln.
 * Interne Klasse, bitte nicht direkt einbinden und/oder verändern!
 */
class SlotMapper : public QObject
{
    Q_OBJECT
public:
    SlotMapper(bool& mappedValue);
    SlotMapper(int& mappedValue);
    SlotMapper(QVector3D& mappedValue);
    SlotMapper(QString& mappedValue);

public slots:
    void mapBool(bool value);
    void mapInteger(int value);
    void mapColor(QColor value);
    void mapString(QString value);

    void mapToWidget();

signals:
    void boolChanged(bool);
    void intChanged(int);
    void colorChanged(QVector3D);
    void stringChanged(QString);
    void styleSheetChanged(QString);


private:
    bool*       boolValue;
    int*        intValue;
    QVector3D*  colorValue;
    QString*    stringValue;

    bool lastBool;
    int lastInt;
    QVector3D lastColor;
    QString lastString;    
};

class EventMapper : public QObject
{
    Q_OBJECT
public slots:
    virtual void mapEvent() {}
    virtual void mapEvent(int index) {Q_UNUSED(index);}
};

class ActionEventMapper : public EventMapper
{
    Q_OBJECT
public:
    ActionEventMapper(std::function<void()>& event);

public slots:
    virtual void mapEvent();

private:
    std::function<void()> eventCall;
};

class SelectionEventMapper : public EventMapper
{
    Q_OBJECT
public:
    SelectionEventMapper(std::function<void(int)>& event);

public slots:
    virtual void mapEvent(int index);

private:
    std::function<void(int)> eventCall;
};

#endif // SLOTMAPPER_H
