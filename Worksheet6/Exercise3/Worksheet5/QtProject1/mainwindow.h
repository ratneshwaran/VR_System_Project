
/**     @file mainwindow.h
  *
  *     EEEE2046 - Software Engineering & VR Project
  *
  *     The Mainwindow header contains all the function declarations and the class prototypes from the mainwindow classes
  *
  *     Group 10
  */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "VRRenderThread.h"

#include "ModelPartList.h"
#include "ModelPart.h"
#include <QFileDialog>
#include <QMainWindow>
#include <QLineEdit>
#include <QColorDialog>
#include <QColor>
#include <vtkPlaneSource.h>

/* VTK headers */

#include <QVTKOpenGLNativeWidget.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkPlaneSource.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setIsVisible(bool _isVisible);
    bool getIsVisible();
    

public slots:
    void handleButton ();
    void handleButton2 ();
    void handleTreeClicked(const QModelIndex& Index);
    void on_actionOpen_File_triggered ();
    void on_actionactionItem_Options_triggered ();
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);
    void on_actionOpen_Folder_triggered();
    void loadFile(QString filePath, QModelIndex parentIndex);
    void on_checkBox_stateChanged(int arg1);
    void on_actionStopVR_triggered();

signals:
    void statusUpdateMessage (const QString & message, int timeout ) ;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    //void on_pushButton_4_clicked();

    void on_actionactionstartVR_triggered();
    void startVRFromTree(const QModelIndex& index, VRRenderThread* thread);
    void on_treeView_doubleClicked(const QModelIndex& index);
    void on_actionRename_File_triggered();

private:
    Ui::MainWindow *ui;

    ModelPartList *partList;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    VRRenderThread* thread = nullptr;
    vtkSmartPointer<vtkActor> floor;
    vtkSmartPointer<vtkActor> VRfloor;
    

};

#endif // MAINWINDOW_H
