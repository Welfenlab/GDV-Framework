/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "slotmapper.h"

#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QFrame>

#include <QDebug>
#include "interfaces/RendererBase.h"
#include "framework/gdvcanvas2d.h"
#include "framework/gdvcanvas3d.h"

#include <QDir>
#include <QGLWidget>
#include <QColorDialog>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    oldParent(0)
{
    ui->setupUi(this);
    this->setWindowTitle(qApp->applicationName() + " - " + qApp->applicationVersion());

    guiUpdate.setInterval(50);
    guiUpdate.start();

    connect(ui->comboClass, SIGNAL(activated(int)), this, SLOT(activateLecture(int)));
    connect(ui->comboMesh, SIGNAL(activated(int)), this, SLOT(activateMesh(int)));
    connect(ui->comboTexture, SIGNAL(activated(int)), this, SLOT(activateTexture(int)));

    redrawUpdate.setInterval(0);
    redrawUpdate.start();
    connect(&redrawUpdate, SIGNAL(timeout()), this, SLOT(redraw()));

    currentLecture = 0;
    enableGL = false;

    scrollBase = new QWidget();
    scrollBase->setLayout(&userWidgets);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setWidget(scrollBase);

    canvas2D = new GdvCanvas2D();
    ui->splitter->addWidget(canvas2D);
    canvas2D->hide();

    QGLFormat glFormat;
    glFormat.setVersion( 3, 2 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0

    canvas3D = new GdvCanvas3D(glFormat);
    ui->splitter->addWidget(canvas3D);
    canvas3D->hide();

    fullscreenControls = ui->frameInscreen;
    ui->frame->layout()->removeWidget(fullscreenControls);
    ui->buttonFullscreen->setIcon(QIcon::fromTheme("view-fullscreen"));
    fullscreen = true;
    updateFullscreenBar();

    ui->scrollArea->setMaximumWidth(400);
    ui->scrollArea->setMinimumWidth(200);



    connect(canvas2D, SIGNAL(sizeChanged(int,int)), this, SLOT(resized(int,int)));
    connect(canvas3D, SIGNAL(sizeChanged(int,int)), this, SLOT(resized(int,int)));
    connect(&guiUpdate, SIGNAL(timeout()), this, SLOT(showFPS()));

    connect(canvas2D, SIGNAL(mouseMoved(int,int)), this, SLOT(mouseMoved(int,int)));
    connect(canvas2D, SIGNAL(mousePressed(int,int)), this, SLOT(mousePressed(int,int)));
    connect(canvas2D, SIGNAL(mouseReleased(int,int)), this, SLOT(mouseReleased(int,int)));
    connect(canvas2D, SIGNAL(wheelMoved(int)), this, SLOT(wheelMoved(int)));
    connect(canvas2D, SIGNAL(keyPressed(QString)), this, SLOT(keyPressed(QString)));
    connect(canvas2D, SIGNAL(keyReleased(QString)), this, SLOT(keyReleased(QString)));

    connect(canvas3D, SIGNAL(mouseMoved(int,int)), this, SLOT(mouseMoved(int,int)));
    connect(canvas3D, SIGNAL(mousePressed(int,int)), this, SLOT(mousePressed(int,int)));
    connect(canvas3D, SIGNAL(mouseReleased(int,int)), this, SLOT(mouseReleased(int,int)));
    connect(canvas3D, SIGNAL(wheelMoved(int)), this, SLOT(wheelMoved(int)));
    connect(canvas3D, SIGNAL(keyPressed(QString)), this, SLOT(keyPressed(QString)));
    connect(canvas3D, SIGNAL(keyReleased(QString)), this, SLOT(keyReleased(QString)));

    connect(ui->buttonFullscreen, SIGNAL(toggled(bool)), this, SLOT(toggleFullscreen()));

    populateMeshList();
    populateTextureList();

    if(meshes.count() == 0 || textures.count() == 0)
    {
        QMessageBox::warning(0, "No textures/meshes found", "No textures/meshes were found in the build directory.\nDid you forget to configure QtCreator to run 'make install' after building?");
    }
}

MainWindow::~MainWindow()
{
    clearElements();

    if(currentLecture)
        currentLecture->deinitialize();

    currentLecture = 0;

    foreach(RendererBase* r, allLectures)
    {
        delete r;
    }
    delete ui;
}

void MainWindow::addCheckBox(QString label, bool defaultValue, bool& mappedValue)
{
    mappedValue = defaultValue;
    QCheckBox* element = new QCheckBox(label);
    element->setChecked(defaultValue);
    SlotMapper* map = new SlotMapper(mappedValue);
    connect(element, SIGNAL(toggled(bool)), map, SLOT(mapBool(bool)));
    connect(map, SIGNAL(boolChanged(bool)), element, SLOT(setChecked(bool)));
    connect(&guiUpdate, SIGNAL(timeout()), map, SLOT(mapToWidget()));

    activeUserWidgets.append(element);
    qObjectMappings.append(map);

    userWidgets.addWidget(element);
}

void MainWindow::addColorSelector(QString label, QVector3D defaultValue, QVector3D& mappedValue)
{
    mappedValue = defaultValue;
    QPushButton* element = new QPushButton(label);
    SlotMapper* map = new SlotMapper(mappedValue);
    QColorDialog* dialog = new QColorDialog();
    dialog->setWindowTitle(QString("Change color: ") + label);

    connect(element, SIGNAL(clicked()), dialog, SLOT(show()));
    connect(dialog, SIGNAL(currentColorChanged(QColor)), map, SLOT(mapColor(QColor)));
    connect(map, SIGNAL(styleSheetChanged(QString)), element, SLOT(setStyleSheet(QString)));
    connect(&guiUpdate, SIGNAL(timeout()), map, SLOT(mapToWidget()));

    activeUserWidgets.append(element);
    activeUserWidgets.append(dialog);
    qObjectMappings.append(map);

    userWidgets.addWidget(element);

    map->mapColor(QColor::fromRgb(mappedValue.x()*255, mappedValue.y()*255, mappedValue.z()*255));
}

void MainWindow::addSlider(QString label, int minimalValue, int maximalValue, int defaultValue, int& mappedValue)
{
    mappedValue = defaultValue;
    addLabel(label, "");
    QSlider* element = new QSlider(Qt::Horizontal);
    element->setMinimum(minimalValue);
    element->setMaximum(maximalValue);
    element->setValue(defaultValue);
    SlotMapper* map = new SlotMapper(mappedValue);
    connect(element, SIGNAL(valueChanged(int)), map, SLOT(mapInteger(int)));
    connect(map, SIGNAL(intChanged(int)), element, SLOT(setValue(int)));
    connect(&guiUpdate, SIGNAL(timeout()), map, SLOT(mapToWidget()));

    activeUserWidgets.append(element);
    qObjectMappings.append(map);

    userWidgets.addWidget(element);
}

void MainWindow::addLabel(QString defaultValue, QString& mappedValue, QString styleSheet)
{
    mappedValue = defaultValue;
    QLabel* element = new QLabel(defaultValue);
    SlotMapper* map = new SlotMapper(mappedValue);
    connect(map, SIGNAL(stringChanged(QString)), element, SLOT(setText(QString)));
    connect(&guiUpdate, SIGNAL(timeout()), map, SLOT(mapToWidget()));

    activeUserWidgets.append(element);
    qObjectMappings.append(map);

    userWidgets.addWidget(element);
    element->setStyleSheet(styleSheet);
}

void MainWindow::addLabel(QString value, QString styleSheet)
{
    QLabel* element = new QLabel(value);
    activeUserWidgets.append(element);
    userWidgets.addWidget(element);
    element->setStyleSheet(styleSheet);
}

void MainWindow::addButton(QString label, std::function<void()> fun)
{
    QPushButton* button = new QPushButton(label);
    EventMapper* map = new ActionEventMapper(fun);

    connect(button, SIGNAL(clicked()), map, SLOT(mapEvent()));

    activeUserWidgets.append(button);
    userWidgets.addWidget(button);
    qObjectEvents.append(map);
}

void MainWindow::addDropdownList(QString entries, unsigned int defaultIndex, std::function<void(int)> fun)
{
    QComboBox* combo = new QComboBox();
    EventMapper* map = new SelectionEventMapper(fun);

    QStringList entryList = entries.split(';');
    combo->addItems(entryList);
    combo->setCurrentIndex(defaultIndex);

    connect(combo, SIGNAL(activated(int)), map, SLOT(mapEvent(int)));

    activeUserWidgets.append(combo);
    userWidgets.addWidget(combo);
    qObjectEvents.append(map);
}

void MainWindow::addSeparator()
{
    QFrame* element = new QFrame();
    element->setFrameShape(QFrame::HLine);
    element->setFrameShadow(QFrame::Sunken);

    activeUserWidgets.append(element);
    userWidgets.addWidget(element);
}

void MainWindow::clearElements()
{
    foreach(SlotMapper* s, qObjectMappings)
    {
        s->deleteLater();
    }

    qObjectMappings.clear();

    foreach(EventMapper* s, qObjectEvents)
    {
        s->deleteLater();
    }

    qObjectEvents.clear();

    foreach(QWidget* w, activeUserWidgets)
    {
        w->deleteLater();
    }

    activeUserWidgets.clear();

    qApp->processEvents();

    QLayoutItem* item;
    while((item = userWidgets.takeAt(0)) != 0)
    {
        delete item;
    }
}

void MainWindow::addLecture(QString identifier, RendererBase *renderer)
{
    allLectures.append(renderer);
    ui->comboClass->addItem(identifier);
}

void MainWindow::setDefaultIndices(unsigned int lecture, unsigned int mesh, unsigned int texture)
{
    ui->comboClass->setCurrentIndex(lecture);
    ui->comboMesh->setCurrentIndex(mesh);
    ui->comboTexture->setCurrentIndex(texture);

    activateLecture(ui->comboClass->currentIndex());
}

void MainWindow::activateLecture(int index)
{
    if(index < 0)
    {
        qWarning() << "Invalid lecture-index specified. No class activated.";
        return;
    }

    qDebug() << "Activated lecture no." << index;

    if(currentLecture)
    {
        clearElements();
        currentLecture->deinitialize();
    }

    currentLecture = allLectures.at(index);

    if(currentLecture->usesOpenGL())
    {
        canvas2D->hide();
        canvas3D->show();
    }
    else
    {
        canvas2D->show();
        canvas3D->hide();
    }


    enableGL = currentLecture->usesOpenGL();

    currentLecture->setupGUI(*this);
    currentLecture->initialize();
    userWidgets.addStretch();

    if(currentLecture->usesOpenGL())
        currentLecture->sizeChanged(canvas3D->width(), canvas3D->height());
    else
        currentLecture->sizeChanged(canvas2D->width(), canvas2D->height());

    // Reload the current mesh
    activateMesh(ui->comboMesh->currentIndex());
    activateTexture(ui->comboTexture->currentIndex());
    perfCount.reset();
}

void MainWindow::redraw()
{
    if(currentLecture && !currentLecture->usesOpenGL())
    {
        perfCount.startFrame();
        currentLecture->render(*canvas2D);
        perfCount.stopFrame();

        canvas2D->repaint();
    }

    if(currentLecture && currentLecture->usesOpenGL())
    {
        perfCount.startFrame();
        currentLecture->render(*canvas3D);
        perfCount.stopFrame();

        canvas3D->repaint();
    }
}

void MainWindow::resized(int width, int height)
{
    if(width > 0 && height > 0)
    {
        if(currentLecture)
            currentLecture->sizeChanged(width, height);

        perfCount.reset();

        fullscreenControls->setGeometry(QRect(width-250,0, 250, 48));

    }
}

void MainWindow::mousePressed(int x, int y)
{
    if(currentLecture)
        currentLecture->mousePressed(x, y);
}

void MainWindow::mouseReleased(int x, int y)
{
    if(currentLecture)
        currentLecture->mouseReleased(x, y);
}

void MainWindow::mouseMoved(int x, int y)
{
    if(currentLecture)
        currentLecture->mouseMoved(x, y);
}

void MainWindow::wheelMoved(int delta)
{
    if(currentLecture)
        currentLecture->wheelMoved(delta);
}

void MainWindow::keyPressed(QString key)
{
    if(currentLecture)
        currentLecture->keyPressed(key);
}

void MainWindow::keyReleased(QString key)
{
    if(currentLecture)
        currentLecture->keyReleased(key);
}

void MainWindow::activateMesh(int index)
{
    if(index < 0)
    {
        qWarning() << "Invalid mesh-index specified. No mesh activated.";
        return;
    }

    if(meshes.count() == 0)
    {
        qCritical() << "Error: No meshes were found. Please make sure you copied the 'meshes' folder to the location of the binary.";
        return;
    }

    if(index > meshes.count())
    {
        qCritical() << "Internal Error: Mesh-Index out of range. Please report to <atarnows@welfenlab.de>";
        return;
    }

    if(!meshes.at(index).isValid())
        meshes[index].parseFile();

    if(currentLecture)
    {
        qDebug() << "Changing mesh to" << ui->comboMesh->itemText(index)
                 << "containing" << meshes[index].faces().size() << "faces.";
        currentLecture->meshChanged(meshes[index].faces());
    }

}

void MainWindow::activateTexture(int index)
{
    if(index < 0)
    {
        qWarning() << "Invalid texture-index specified. No texture activated.";
        return;
    }

    if(textures.count() == 0)
    {
        qCritical() << "Error: No textures were found. Please make sure you copied the 'textures' folder to the location of the binary.";
        return;
    }

    if(index > textures.count())
    {
        qCritical() << "Internal Error: Texture-Index out of range. Please report to <atarnows@welfenlab.de>";
        return;
    }

    if(currentLecture)
    {
        qDebug() << "Changing texture to" << ui->comboTexture->itemText(index) << ".";
        if(currentLecture->usesOpenGL())
            currentLecture->textureChanged(QGLWidget::convertToGLFormat(textures[index]));
        else
            currentLecture->textureChanged(textures[index]);
    }
}

void MainWindow::populateMeshList()
{
    // We will search in the actual
    qDebug() << "Populating mesh-list...";
    meshes.clear();
    ui->comboMesh->clear();

    QDir searchDir("meshes/");
    QStringList nameFilters;
    nameFilters << "*.ply" << "*.PLY";
    searchDir.setNameFilters(nameFilters);
    QStringList allMeshFiles = searchDir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name | QDir::IgnoreCase);

    foreach(QString file, allMeshFiles)
    {
        meshes.append(MeshLoader(QString("meshes/") + file));
        qDebug() << "Added" << file;
        file.truncate(file.length()-4);
        ui->comboMesh->addItem(file);
    }

}

void MainWindow::populateTextureList()
{
    qDebug() << "Populating texture-list...";
    textures.clear();
    ui->comboTexture->clear();

    QDir searchDir("textures/");
    QStringList nameFilters;
    nameFilters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";
    searchDir.setNameFilters(nameFilters);
    QStringList allTextureFiles = searchDir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name | QDir::IgnoreCase);

    foreach(QString file, allTextureFiles)
    {
        QImage t(QString("textures/") + file);
        t = t.convertToFormat(QImage::Format_RGB32);
        textures.append(t);
        qDebug() << "Added" << file;
        file.truncate(file.length()-4);
        ui->comboTexture->addItem(file);
    }
}

void MainWindow::updateFullscreenBar()
{
    QWidget* wd = enableGL? static_cast<QWidget*>(canvas3D) : static_cast<QWidget*>(canvas2D);
    fullscreenControls->setParent(wd);

    ui->frameInscreen->setStyleSheet("QFrame { background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0.5, y2:0, stop:0 rgba(255, 255, 255, 0), stop:1 rgba(255, 255, 255, 255)); }"
                                     "QFrame { border-bottom:1px solid qlineargradient(spread:pad, x1:0, y1:0, x2:0.75, y2:0, stop:0 rgba(255, 255, 255, 0), stop:1 rgba(255, 255, 255, 255)); }");
    ui->buttonFullscreen->setStyleSheet("QToolButton {background-color:transparent; border-radius:2px; padding:3px;}"
                                        "QToolButton:hover {background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}"
                                        "QToolButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}"
                                        "QToolButton:checked { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}");
    ui->labelFPS->setStyleSheet("border:0px solid transparent; font-weight:bold; background-color:transparent;");

    fullscreenControls->setGeometry(QRect(wd->width()-250,0, 250, 48));
}

void MainWindow::showFPS()
{
    int fps = round(perfCount.averageFPS());
    ui->labelFPS->setText(QString::number(fps) + QString(" Frames/s"));

    if(fps < 5)
        ui->labelFPS->setStyleSheet("color:#A00;");
    else if (fps > 25)
        ui->labelFPS->setStyleSheet("color:#0A0;");
    else
        ui->labelFPS->setStyleSheet("color:#000;");
}

void MainWindow::drawGLCall()
{
    if(currentLecture && currentLecture->usesOpenGL())
        currentLecture->render(*canvas3D);
}

void MainWindow::toggleFullscreen()
{
    fullscreen = ui->buttonFullscreen->isChecked();

    QWidget* win = enableGL? static_cast<QWidget*>(canvas3D) : static_cast<QWidget*>(canvas2D);

    if(fullscreen)
    {
        oldFlags = this->windowFlags();
        oldSize = win->size();
        oldParent = static_cast<QWidget*>(win->parent());
        win->setParent(0);
        win->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        win->showMaximized();
    }
    else
    {
        win->setParent(oldParent);
        win->resize(oldSize);
        win->overrideWindowFlags(oldFlags);
        win->show();
    }

    updateFullscreenBar();
}
