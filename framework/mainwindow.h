#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **
 ** Changes:
 ** 2015/03 (r2) - Added Fullscreen and Screenshot support
 ** 2015/03 (r2) - Added call based actions like Button and DropdownList
 **
 **/


#include <QMainWindow>
#include <QVBoxLayout>
#include <QTimer>
#include <QVector3D>
#include <functional>
#include "interfaces/GdvGui.h"
#include "meshloader.h"
#include "performancemonitor.h"

namespace Ui {
    class MainWindow;
}

class SlotMapper;
class EventMapper;
class GdvCanvas2D;
class GdvCanvas3D;

/**
 * @brief Die MainWindow Klasse
 *
 * Die meiste Verwaltung des Frameworks geschieht hier.
 *
 * Interne Klasse, bitte nicht direkt einbinden und/oder verändern!
 */
class MainWindow : public QMainWindow, public GdvGui
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // GdvGui interface implementation
    virtual void addCheckBox(QString label, bool defaultValue, bool& mappedValue);
    virtual void addColorSelector(QString label, QVector3D defaultValue, QVector3D& mappedValue);
    virtual void addSlider(QString label, int minimalValue, int maximalValue, int defaultValue, int& mappedValue);
    virtual void addLabel(QString defaultValue, QString& mappedValue, QString styleSheet = "font-weight:bold;");
    virtual void addLabel(QString value, QString styleSheet = "font-weight:bold;");

    virtual void addButton(QString label, std::function<void()> fun);
    virtual void addDropdownList(QString entries, unsigned int defaultIndex, std::function<void(int)> fun);

    virtual void addSeparator();
    virtual void clearElements();

    virtual void addLecture(QString identifier, RendererBase* renderer);
    virtual void setDefaultIndices(unsigned int lecture, unsigned int mesh, unsigned int texture);

protected slots:
    void activateLecture(int index);
    void redraw();
    void resized(int width, int height);
    void mousePressed(int, int);
    void mouseReleased(int, int);
    void mouseMoved(int, int);
    void wheelMoved(int);
    void keyPressed(QString);
    void keyReleased(QString);

    void activateMesh(int index);
    void activateTexture(int index);
    void showFPS();
    void drawGLCall();

    void toggleFullscreen();

private:

    void populateMeshList();
    void populateTextureList();
    void updateFullscreenBar();

    RendererBase* currentLecture;
    QVector<RendererBase*> allLectures;
    QVector<MeshLoader> meshes;
    QVector<QImage> textures;

    Ui::MainWindow *ui;
    QWidget* scrollBase;
    QVector<QWidget*> activeUserWidgets;
    QVector<SlotMapper*> qObjectMappings;
    QVector<EventMapper*> qObjectEvents;
    QVBoxLayout userWidgets;

    QTimer guiUpdate;
    QTimer redrawUpdate;
    PerformanceMonitor perfCount;

    GdvCanvas2D* canvas2D;
    GdvCanvas3D* canvas3D;
    QWidget*     fullscreenControls;

    bool enableGL;

    bool fullscreen;
    QSize oldSize;
    QWidget* oldParent;
    Qt::WindowFlags oldFlags;
};

#endif // MAINWINDOW_H
