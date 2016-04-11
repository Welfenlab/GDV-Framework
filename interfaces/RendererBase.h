#ifndef RENDERERBASE_H
#define RENDERERBASE_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>

#include <QImage>
#include "GdvGui.h"
#include "GdvCanvas.h"

#include "framework/meshloader.h"

/**
 * @brief Die RendererBase Klasse
 *
 * Sämtliche Aufgaben des Labors werden durch Implementierung dieses Interfaces
 * realisiert. In der Datei main.cpp können die fertigen Implementierungen
 * mittels addLecture eingetragen werden.
 *
 * Sämtliche Methoden, welche auf "= 0" enden MÜSSEN implementiert werden.
 * Die restlichen Methoden sind optional und können je nach Aufgabenstellung
 * implementiert oder auch ignoriert werden.
 */
class RendererBase
{
public:
    /**
     * @brief setupGUI Ermöglicht das Anpassen des User-Interfaces
     * @param userInterface Eine Instanz der GdvGui Implementierung
     *
     * In der Beschreibung des GdvGui-Interfaces sind die einzelnen
     * hinzufügbaren Elemente genau beschrieben.
     *
     * Wichtig: userInterface ist nur im Kontext dieser Methode gültig!
     */
    virtual void setupGUI(GdvGui& userInterface) = 0;

    /**
     * @brief initialize Initialisierungsmethode, die beim aktivieren der Abgabe einmalig ausgeführt wird
     *
     * Diese Methode wird jedes mal aufgerufen, wenn die Klasse/Abgabe in der
     * GUI angewählt wird. An dieser Stelle ist es am sinnvollsten OpenGL-
     * Ressourcen zu initialisieren.
     */
    virtual void initialize() = 0;

    /**
     * @brief render Zeichenmethode, die vom Framework periodisch aufgerufen wird.
     * @param canvas Die momentan aktive Zeichenfläche
     *
     * Diese Methode wird - jeweils nachdem ein Frame gezeichnet und alle
     * GUI-Operationen durchgeführt wurden - periodisch aufgerufen. Im OpenGL-
     * Modus kann es sein, dass (je nach VSync Einstellungen) diese Methode max.
     * 60 mal in der Sekunde aufgerufen wird.
     *
     * Wichtig: canvas ist nur im Kontext dieser Methode gültig!
     */
    virtual void render(GdvCanvas& canvas) = 0;

    /**
     * @brief deinitialize Deinitialisierungsmethode, die vor dem Wechsel zu einer anderen Abgabe einmalig ausgeführt wird
     *
     * Diese Methode wird jedes mal aufgerufen, wenn eine andere Klasse/Abgabe
     * in der GUI angewählt wird, oder die GUI beendet wird. Wird OpenGL genutzt
     * sollten Ressourcen (TextureIDs, Buffer-Objekte) hier freigegeben werden.
     */
    virtual void deinitialize() = 0;

    /**
     * @brief sizeChanged Wird aufgerufen, wenn sich die Größe des Fensters bzw. der Zeichenfläche ändert
     * @param width Die neue Breite der Zeichenfläche
     * @param height Die neue Höhe der Zeichenfläche
     *
     * Eigene Buffer (Tiefenbuffer, Framebuffer, etc.) sollten - wann immer
     * diese Methode aufgerufen wird - angepasst bzw. reinitialisiert werden.
     *
     * Hinweis: Auf manchen Systemen und QT-Versionen kann beim Start der GUI
     * kurzzeitig eine Höhe oder Breite von 0 auftauchen. Dieses Verhalten kann
     * je nach eigener Implementierung zu Problemen führen und ist daher ggf.
     * abzufangen.
     */
    virtual void sizeChanged(unsigned int width, unsigned int height) = 0;

    /**
     * @brief usesOpenGL Gibt an, ob diese Klasse/Abgabe OpenGL verwendet
     * @return true, falls OpenGL in der Abgabe verwendet wird. False, wenn nicht
     *
     * Je nach Auswahl wird ein valider OpenGL-Kontext in der render-Methode
     * bereitgestellt oder nicht. Direkte Zeichenoperationen werden nur im
     * nicht-OpenGL-Modus unterstützt.
     */
    virtual bool usesOpenGL() = 0;

    /**
     * @brief meshChanged Wird aufgerufen, wenn in der GUI der aktive Mesh geändert wurde
     * @param faces Eine Liste/Vektor mit den einzelnen Faces des Meshes
     *
     * -- Die Implementierung dieser Methode ist optional.
     *
     * Beim Anwählen eines neuen Meshes, oder auch beim aktivieren der Abgabe,
     * wird diese Methode ausgeführt. Innerhalb dieser Methode sollte das
     * übergebene Mesh in eine eigene Datenstruktur überführt/konvertiert
     * werden.
     */
    virtual void meshChanged(const QVector<MeshLoader::Face>& faces) { Q_UNUSED(faces); }

    /**
     * @brief textureChanged Wird aufgerufen. wenn in der GUI die aktive Textur geändert wurde
     * @param texture Ein QImage, welches die neue Textur repräsentiert
     *
     * -- Die Implementierung dieser Methode ist optional.
     *
     * Beim Anwählen einer neuen Textur, oder auch beim aktivieren der Abgabe,
     * wird diese Methode ausgeführt. Die Höhe und Breite der Textur kann über
     * die width() bzw. height() Methode des QImages ermittelt werden. Mit der
     * pixel(x, y)-Methode kann der Farbwert eines Pixels ermittelt werden.
     *
     * Hinweis: Im OpenGL-Modus ist das übergebene QImage bereits in ein
     * kompatibles Format kovertiert worden. Mit der bits()-Methode kann auf den
     * für OpenGL benötigten Pointer zugegriffen werden.
     */
    virtual void textureChanged(QImage texture) { Q_UNUSED(texture); }

    /**
     * @brief mousePressed Wird aufgerufen, wenn innerhalb der Zeichenfläche eine Maustaste gedrückt wird
     * @param x Die horizontale Position des Mauszeigers
     * @param y Die vertikale Position des Mauszeigers
     *
     * -- Die Implementierung dieser Methode ist optional.
     *
     */
    virtual void mousePressed(int x, int y)     { Q_UNUSED(x); Q_UNUSED(y); }

    /**
     * @brief mouseReleased Wird aufgerufen, wenn innerhalb der Zeichenfläche eine Maustaste losgelassen wird
     * @param x Die horizontale Position des Mauszeigers
     * @param y Die vertikale Position des Mauszeigers
     *
     * -- Die Implementierung dieser Methode ist optional.
     *
     */
    virtual void mouseReleased(int x, int y)    { Q_UNUSED(x); Q_UNUSED(y); }

    /**
     * @brief mouseMoved Wird aufgerufen, wenn innerhalb der Zeichenfläche die Maus bewegt wird
     * @param x Die horizontale Position des Mauszeigers
     * @param y Die vertikale Position des Mauszeigers
     *
     * -- Die Implementierung dieser Methode ist optional.
     *
     */
    virtual void mouseMoved(int x, int y)       { Q_UNUSED(x); Q_UNUSED(y); }

    /**
     * @brief wheelMoved Wird aufgerufen, wenn innerhalb der Zeichenfläche das Mausrad bewegt wird
     * @param delta Die Richtung, in der das Mausrad bewegt wurde
     *
     * -- Die Implementierung dieser Methode ist optional.
     *
     * Die Richtung der Mausradbewegung kann durch das Vorzeichen des Wertes
     * ermittelt werden. Die Größe des Wertes ist abhängig von verschiedenen
     * Faktoren.
     */
    virtual void wheelMoved(int delta)          { Q_UNUSED(delta); }

    /**
     * @brief keyPressed Wird aufgerufen, wenn der Mauszeiger sich innerhalb der Zeichenfläche befindet und eine Taste auf dem Keyboard gedrückt wird
     * @param key Die gedrückte Taste als String
     *
     * -- Die Implementierung dieser Methode ist optional.
     *
     */
    virtual void keyPressed(QString key)        { Q_UNUSED(key); }

    /**
     * @brief keyReleased Wird aufgerufen, wenn der Mauszeiger sich innerhalb der Zeichenfläche befindet und eine Taste auf dem Keyboard losgelassen wird
     * @param key Die losgelassene Taste als String
     *
     * -- Die Implementierung dieser Methode ist optional.
     *
     */
    virtual void keyReleased(QString key)        { Q_UNUSED(key); }

    virtual ~RendererBase() {}
};

#endif // RENDERERBASE_H
