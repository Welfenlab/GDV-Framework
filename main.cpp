/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/

#include "framework/mainwindow.h"
#include <QApplication>

#include "examples/frameworkexample.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("GDV-Framework");
    a.setApplicationVersion("r2");
    a.setOrganizationName("Leibniz Universität Hannover - Welfenlab");
    MainWindow w;

    w.addLecture("Beispielprojekt", new FrameworkExample());
    // ...
    // Hier können eigene Abgaben eingetragen werden.
   

    w.setDefaultIndices(0,0,0);
    w.show();    
    return a.exec();
}
