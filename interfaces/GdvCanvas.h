#ifndef GDVCANVAS_H
#define GDVCANVAS_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/

#include <QVector3D>

/**
 * @brief Die GdvCanvas Klasse
 *
 * Eine Instanz dieser Klasse wird in der render-Methode übergeben und stellt
 * die einzige Schnittstelle dar, um auf die Zeichenfläche zugreifen zu können.
 * Im 3D-rendering-Modus sind die Funktionen dieser Klasse ohne Funktion, da
 * in diesem Fall direkt mit OpenGL gearbeitet wird.
 *
 * Die in der render-Methode übergebene Instanz dieser Klasse darf nur innerhalb
 * der Methode verwendet werden. Das Verhalten ausserhalb des Funktionskontexts
 * ist nicht definiert und kann zu Abstürzen führen.
 */
class GdvCanvas
{
public:
    /**
     * @brief setPixel Ändert die Farbe eines Pixels im Back-Buffer
     * @param x Die x-Koordinate des zu ändernden Pixels
     * @param y Die y-Koordinate des zu ändernden Pixels
     * @param color Die Farbe des Pixels in RGB-Darstellung, jeweils im Bereich 0.0 bis 1.0
     *
     * Es ist unbedingt zu vermeiden, ausserhalb des aktuellen Viewports zu zeichnen.
     * Zwar werden derartige Fehler abgefangen, die Ausführungsgeschwindigkeit wird
     * jedoch drastisch (!) verlangsamt. Wird bei der Farbwahl der Bereich 0.0 bis 1.0
     * verlassen, so ist die resultierende Farbe undefiniert.
     */
    virtual void setPixel(unsigned int x, unsigned int y, const QVector3D& color) = 0;

    /**
     * @brief clearBuffer Löscht den aktuellen Backpuffer und setzt die Hintergrundfarbe auf den angegebenen Wert
     * @param clearColor Die zu setzende Farbe in RGB-Darstellung, jeweils im Bereich 0.0 bis 1.0
     */
    virtual void clearBuffer(const QVector3D& clearColor) = 0;

    /**
     * @brief flipBuffer Übernimmt die Änderungen seit dem letzten Aufruf
     *
     * Das Framework implementiert das sog. Double-Buffering, d.h. Aufrufe von
     * setPixel werden zunächst in einen nicht sichtbaren Puffer umgeleitet.
     * Erst beim Aufruf von flipBuffer wird dieser Puffer gezeichnet, wodurch
     * eine flackerfreie Darstellung garantiert wird.
     */
    virtual void flipBuffer() = 0;

    /**
     * @brief flipBuffer Übernimmt den Inhalt eines vorgegebenen Buffers
     * @param buffer Das zu Übernehmende QImage
     *
     * FÜR FORTGESCHRITTENE:
     * Anstelle mit setPixel und clearBuffer zu arbeiten kann auch ein eigenes
     * QImage übernommen werden. Hierbei ist zu beachten, dass die Größe des
     * Bildes der des aktuellen Viewports entspricht.
     *
     * Durch das direkte verwenden eines QImages kann evtl. ein wenig Rechenzeit
     * eingespart werden, da direkt auf den Speicher des Bildes zugegriffen
     * werden kann.
     */
    virtual void flipBuffer(const QImage& buffer) = 0;
};

#endif // GDVCANVAS_H
