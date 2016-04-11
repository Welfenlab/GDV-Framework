#ifndef GDVGUI_H
#define GDVGUI_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#include <QString>
#include <QVector3D>
#include <functional>

#define callAction(fun) [&](){fun();}
#define callSelection(fun) [&](int ny){fun(ny);}

class RendererBase;

/**
 * @brief Die GdvGui Klasse
 *
 * Eine Instanz dieser Klasse wird in der Methode setupGUI übergeben.
 * Sie erlaubt es, auch ohne QT-GUI Kenntnisse eigene GUI Elemente dem Framework
 * hinzuzufügen. Die Werte der entsprechenden Elemente können direkt mit lokalen
 * Variablen der eigenen Klasse verknüpft werden. Hier ist es jedoch wichtig,
 * auf den Scope der jeweiligen Variablen zu achten: Verknüpfungen mit lokalen
 * Stack-Variablen führen unweigerlich zum Absturz!
 */
class GdvGui
{
public:
    /**
     * @brief addCheckBox Fügt der GUI eine Checkbox hinzu
     * @param label Der angezeigte Text der Checkbox
     * @param defaultValue Der voreingestellte Zustand der Checkbox
     * @param mappedValue Die mit dem Element zu verknüpfende lokale Variable
     */
    virtual void addCheckBox(QString label, bool defaultValue, bool& mappedValue) = 0;

    /**
     * @brief addSlider Fügt der GUI einen (horizontalen) Slider hinzu
     * @param label Beschriftung des Textes, welcher oberhalb des Slides dargestellt wird
     * @param minimalValue Der kleinstmögliche auswählbare Wert des Sliders
     * @param maximalValue Der größtmögliche auswählbare Wert des Sliders
     * @param defaultValue Der voreingestellte Wert des Sliders
     * @param mappedValue Die mit dem Element zu verknüpfende lokale Variable
     *
     * In der aktuellen Version des Frameworks werden leider noch keine
     * Floatingpoint-Slider unterstützt. Diese können jedoch mit einer
     * entsprechenden Skalierung des Integer-Bereichs "von Hand" nachgebildet
     * werden.
     */
    virtual void addSlider(QString label, int minimalValue, int maximalValue, int defaultValue, int& mappedValue) = 0;

    /**
     * @brief addColorSelector Fügt der GUI einen Button hinzu, der einen Farbauswahldialog öffnet
     * @param label Die Beschriftung des Buttons
     * @param defaultValue Der voreingestellte Farbwert (im Bereich 3x 0.0..1.0)
     * @param mappedValue Die mit dem Element zu verknüpfende lokale Variable
     *
     * Es wird der systemeigene Farbwahldialog verwendet. Daher kann sich die
     * Darstellung auf Windows/MacOS/Linux-Systemen unterscheiden.
     */
    virtual void addColorSelector(QString label, QVector3D defaultValue, QVector3D& mappedValue) = 0;

    /**
     * @brief addLabel Fügt der GUI ein Beschriftungsfeld hinzu
     * @param defaultValue Der standardmäßig anzuzeigende Text
     * @param mappedValue Die mit dem Element zu verknüpfende lokale Variable
     * @param styleSheet Ein String mit einem validen StyleSheet, welches das Aussehen des Labels bestimmt
     *
     * Die genaue Syntax des styleSheet-Arguments kann unter
     * http://qt-project.org/doc/qt-4.8/stylesheet-syntax.html
     * eingesehen werden.
     */
    virtual void addLabel(QString defaultValue, QString& mappedValue, QString styleSheet = "font-weight:bold;") = 0;

    /**
     * @brief addLabel Fügt der GUI ein statisches Beschriftungsfeld hinzu
     * @param value Der anzuzeigende Text
     * @param styleSheet Ein String mit einem validen StyleSheet, welches das Aussehen des Labels bestimmt
     *
     * Diese Methode kann verwendet werden, um einen statischen, einmalig gesetzen Text darzustellen.
     * Die Übergabe einer lokalen Variable ist in diesem Fall nicht notwendig.
     */
    virtual void addLabel(QString value, QString styleSheet = "font-weight:bold;") = 0;

    /**
     * @brief addButton Fügt der GUI einen Knopf hinzu, der bei einem Klick eine zuvor bestimmte Methode aufruft
     * @param label Die Beschriftung des Knopfes
     * @param fun Die aufzurufende Funktion. Bitte den Befehl "callAction" verwenden, um eine Methode innerhalb der eigenen Klasse aufzurufen!
     *
     * Bei einem Klick wird die in 'fun' spezifizierte Funktion aufgerufen.
     * Beispiel - Die eigene Abgabeklasse enthält eine Methode "alert()", die bei einem Klick auf den Knopf aufgerufen werden soll:
     *  userInterface.addButton("Info", callAction(alert));
     */
    virtual void addButton(QString label, std::function<void()> fun) = 0;

    /**
     * @brief addDropdownList Fügt der GUI einen Auswahlliste hinzu, die bei einer Änderung der Auswahl eine zuvor bestimmte Methode aufruft
     * @param entries Ein String mit den anzuzeigenden Auswahloptionen, jeweils mit Semikolon getrennt
     * @param defaultIndex Der standardmäßig ausgewählte Eintrag
     * @param fun Die aufzurufende Funktion. Bitte den Befehl "callSelection" verwenden, um eine Methode innerhalb der eigenen Klasse aufzurufen!
     *
     * Bei der Änderung der aktuellen Auswahl wird die in 'fun' spezifizierte Funktion aufgerufen.
     * Als Argument wird der Index des aktuell ausgewählten Elements übergeben.
     * Beispiel - Die eigene Abgabeklasse enthält eine Methode "select(int value)":
     *  userInterface.addDropdownList("TestA;TestB;TestC", 2, callSelection(select));
     * Es werden die drei Einträge "TestA", "TestB" und "TestC" angezeigt. Bei einer Auswahl wird "select" mit dem Wert 0, 1 oder 2 aufgerufen.
     * Standardmäßig ist der Eintrag "TestC" (Index 2) ausgewählt.
     */
    virtual void addDropdownList(QString entries, unsigned int defaultIndex, std::function<void(int)> fun) = 0;

    /**
     * @brief addSeparator Fügt eine Trennlinie in die GUI ein
     */
    virtual void addSeparator() = 0;

    /**
     * @brief clearElements Entfernt sämtliche bislang hinzugefügte Elemente aus der GUI
     *
     * Diese Funktion muss für gewöhnlich nicht explizit aufgerufen werden,
     * da beim deaktivieren einer RendererBase Instanz dies automatisch geschieht.
     */
    virtual void clearElements() = 0;

    /**
     * @brief addLecture Fügt dem Framework eine Instanz einer eigenen Abgabe hinzu
     * @param identifier Die Bezeichnung, mit der die Abgabe in der GUI versehen wird
     * @param renderer Ein Pointer auf eine valide Instanz einer von RenderereBase abgeleiteten Klasse
     *
     * Wird eine Klasse "FrameworkExample" mit addLecture wie folgt hinzugefügt:
     *  addLecture("Beispielprojekt", new FrameworkExample());
     * So ist diese beim Start des Frameworks in der Auswahlbox "Active class" unter der Bezeichnung
     * "Beispielprojekt" aufrufbar.
     *
     * Wichtig: Beim Beenden der GUI wird die Instanz automatisch mittels "delete" gelöscht!
     */
    virtual void addLecture(QString identifier, RendererBase* renderer) = 0;

    /**
     * @brief setDefaultIndices Lädt beim Starten der GUI automatisch eine gewählte Konfiguration
     * @param lecture Der Index der Klasse, welche standardmäßig geladen werden soll (beginnend bei 0)
     * @param mesh Der Index des Meshes, welcher standardmäßig geladen werden soll (beginnend bei 0)
     * @param texture Der Index der Textur, welche standardmäßig geladen werden soll (beginnend bei 0)
     *
     * Diese Methode gibt an, welche Einstellungen beim Starten der GUI automatische geladen werden sollen.
     * Wurden in der main.cpp beispielsweise drei Abgaben mit "addLecture" hinzugefügt, kann mit
     * setDefaultIndices(1,0,0) die als zweites eingetragene Abgabe standardmäßig beim Start aktiviert werden.
     */
    virtual void setDefaultIndices(unsigned int lecture, unsigned int mesh, unsigned int texture) = 0;
};

#endif // GDVGUI_H
