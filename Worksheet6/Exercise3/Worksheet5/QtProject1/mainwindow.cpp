
/**     @file mainwindow.cpp
  *     The Mainwindow class contains various functions and the constructors for the user-interface
  *     Group 10
  */

  /* VTK headers */
#include "VRRenderThread.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "ModelPart.h"
#include "ModelPartList.h"
#include "optiondialog.h"
#include <qfiledialog.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkJPEGReader.h>
#include <vtkTexture.h>
#include <vtkCubeSource.h>


/**
* @brief Constructor of the mainwindow
*/
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /**
    * @param parent The pointer of the parent widget
    */
    ui->setupUi(this);
    this->setStyleSheet("background-color:#2d2e2e;""color:#1bbffa;");
    //ui.setwindowTitle("Model ");
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
    /*connect ( ui->pushButton_3 , &QPushButton::released, this, &MainWindow::handleButton3 );*/
    connect(this, &MainWindow::statusUpdateMessage,
    ui->statusbar, &QStatusBar::showMessage);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    ui->treeView->addAction(ui->actionactionItem_Options);
    
    /* Link a render window with the Qt widget */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    /* Add a renderer */
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    /* Create an object and add to renderer (this will change later to display a CAD
    model) */
    /* Will just copy and paste cylinder example from before */
    // This creates a polygonal cylinder model with eight circumferential facets
    // (i.e, in practice an octagonal prism).
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    // The mapper is responsible for pushing the geometry in to the graphics
    // library. It may also do color mapping, if scalars or other attributes are
    // defined.
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // The actor is a grouping mechanism: besides the geometry (mapper), it
    // also has a property, transformation matrix, and/or texture map.
    // Here we set its color and rotate it around the X and Y axes.
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    vtkNew<vtkPlaneSource> planeSource;
    planeSource->SetOrigin(0.0, 0.0, 0.0);
    planeSource->SetPoint1(3.0, 0.0, 0.0);
    planeSource->SetPoint2(0.0, 3.0, 0.0);
    planeSource->SetCenter(0.0, 0.0, 0.0);
    planeSource->Update();
    vtkPolyData* plane = planeSource->GetOutput();
    vtkNew<vtkPolyDataMapper> bgmapper;
    bgmapper->SetInputData(plane);
    floor = vtkSmartPointer<vtkActor>::New();
    floor->SetMapper(bgmapper);
    floor->GetProperty()->SetColor(1., 1., 1.);
    renderer->AddActor(floor);

    //prepare another floor
    planeSource = vtkNew<vtkPlaneSource>::vtkNew();
    planeSource->SetOrigin(0.0, 0.0, 0.0);
    planeSource->SetPoint1(250.0, 0.0, 0.0);
    planeSource->SetPoint2(0.0, 250.0, 0.0);
    planeSource->SetCenter(90.0, 0.0, 0.0);
    planeSource->Update();
    plane = planeSource->GetOutput();
    bgmapper = vtkNew<vtkPolyDataMapper>::vtkNew();
    bgmapper->SetInputData(plane);
    floor = vtkSmartPointer<vtkActor>::New();
    floor->SetMapper(bgmapper);
    floor->GetProperty()->SetColor(1., 1., 1.);

    /* Reset Camera (probably needs to go in its own function that is called whenever
     model is changed) */
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    vtkSmartPointer<vtkJPEGReader> backgroundreader = vtkSmartPointer<vtkJPEGReader>::New();

    //when checking please change the path accordingly to your computer
    backgroundreader->SetFileName("C:/Users/Hisham/2022_GROUP_10/Worksheet6/Exercise3/Worksheet5/QtProject1/background.jpg");
    vtkSmartPointer<vtkTexture> backgroundtexture = vtkSmartPointer<vtkTexture>::New();
    backgroundtexture->SetInputConnection(backgroundreader->GetOutputPort());

    // This generates a cube to cover the entire window
    vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
    cube->SetCenter(0, 0, 0);
    cube->SetXLength(10000);
    cube->SetYLength(10000);
    cube->SetZLength(10000);
    cube->Update();

    // This maps the texure onto the cube
    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputData(cube->GetOutput());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(cubeMapper);
    actor->SetTexture(backgroundtexture);

    // This adds the actor to the renderer
    renderer->AddActor(actor);

    /* Create / allocate the ModelList */
    this->partList = new ModelPartList("PartsList");

    /* Link it to the treeview in the GUI */
    ui->treeView->setModel(this->partList);

    /* Manually create a model tree = there a much better and more flexible ways of doing
    this,
    e.g. with nested functions. This is just a quick example as astarting point. */
    ModelPart* rootItem = this->partList->getRootItem();

    /* Add 3 top level items */
    for (int i = 0; i < 3; i++) {
        /* Create strings for both data columns */
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        /* Create child item */
        ModelPart* childItem = new ModelPart({ name , visible });

        /* Append t o t r e e top=l e v e l */
        rootItem->appendChild(childItem);

        /* Add 5 sub=items */
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart* childChildItem = new ModelPart({ name , visible });

            /* Append to parent */
            childItem->appendChild(childChildItem);
        }
    }
}

/**
* Member Functions
*/

/**
* @brief This slot function displays the action as a status update.The slots handle the UI with the aid of the tree view and buttons
*/
void MainWindow::handleButton() {
    //QMessageBox msgBox ;
    //msgBox.setText("Add button was clicked");
    //msgBox.exec ( ) ;
    this->updateRender();
    emit statusUpdateMessage(QString("Add button was clicked"), 0);
}

/**
* @brief This slot function displays the action as a status update
*/
void MainWindow::handleButton2() {
    emit statusUpdateMessage(QString("Color button was clicked"), 0);
}

/** @brief This slot function displays the action as a status update
*   Displays for selected item
*/

void MainWindow::handleTreeClicked(const QModelIndex& Index) {
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast <ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);

}

bool MainWindow::getIsVisible() {
    return ui->checkBox->isChecked();
}

void MainWindow::setIsVisible(bool _isVisible) {
    ui->checkBox->setChecked(_isVisible);
}

/**
  *     @brief virtual destructor
  */
MainWindow::~MainWindow()
{
    delete ui;
}


/** @brief This slot function allows files to be selected (Open file dialog box, directory, option dialog box)(CAD STL files) and loads the stl file and updates the rendering
* Contains the camera range
*/
void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage(QString("Open File "), 0);

    /* Allow user to select a file */
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\", tr("STL Files (*.stl);; Text Files (*.txt)"));

    QModelIndex index = ui->treeView->currentIndex();

    // Add this line of code so you can see if the action is working
    //emit statusUpdateMessage( QString ( "Open File action triggered ") , 0 )

    /* Extract file name and and add to a tree view */
    QFileInfo info(fileName);
    QString File_Name = info.baseName();

    QString visible = "True";
    QModelIndex newIndex = partList->appendChild(index, { File_Name, visible });
    ModelPart* selectedPart = static_cast <ModelPart*>(newIndex.internalPointer());

    /* Load the STL file and update rendering */
    selectedPart->loadSTL(fileName);
    updateRender();

    vtkSmartPointer<vtkJPEGReader> backgroundreader = vtkSmartPointer<vtkJPEGReader>::New();
    backgroundreader->SetFileName("C:/Users/Hisham/2022_GROUP_10/Worksheet6/Exercise3/Worksheet5/QtProject1/background.jpg");
    vtkSmartPointer<vtkTexture> backgroundtexture = vtkSmartPointer<vtkTexture>::New();
    backgroundtexture->SetInputConnection(backgroundreader->GetOutputPort());

    // This generates a cube to cover the entire window
    vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
    cube->SetCenter(0, 0, 0);
    cube->SetXLength(10000);
    cube->SetYLength(10000);
    cube->SetZLength(10000);
    cube->Update();

    // This maps the texure onto the cube
    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputData(cube->GetOutput());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(cubeMapper);
    actor->SetTexture(backgroundtexture);

    // This adds the actor to the renderer
    renderer->AddActor(actor);

    /* Addjust the camera range */
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}

/**
  *     @brief Updates the renderer window
  */
void MainWindow::updateRender() {


    /* Update the Render and then rendering */
    renderer->RemoveAllViewProps();
    renderer->AddActor(floor);
    vtkSmartPointer<vtkJPEGReader> backgroundreader = vtkSmartPointer<vtkJPEGReader>::New();

    //when checking please change the path accordingly to your computer
    backgroundreader->SetFileName("C:/Users/Hisham/2022_GROUP_10/Worksheet6/Exercise3/Worksheet5/QtProject1/background.jpg");
    vtkSmartPointer<vtkTexture> backgroundtexture = vtkSmartPointer<vtkTexture>::New();
    backgroundtexture->SetInputConnection(backgroundreader->GetOutputPort());

    // This generates a cube to cover the entire window
    vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
    cube->SetCenter(0, 0, 0);
    cube->SetXLength(10000);
    cube->SetYLength(10000);
    cube->SetZLength(10000);
    cube->Update();

    // This maps the texure onto the cube
    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputData(cube->GetOutput());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(cubeMapper);
    actor->SetTexture(backgroundtexture);

    // This adds the actor to the renderer
    renderer->AddActor(actor);

    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    ModelPart* parent = selectedPart->parentItem();
    if (!parent->visible()) {
        return;
    }
    updateRenderFromTree(index);
    renderer->Render();
    renderWindow->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    /**
    * @param index Represents the tree view
    */
    if (index.isValid()) {

        /* Retrieve actor from selected part */
        ModelPart* selectedPart = static_cast <ModelPart*>(index.internalPointer());

        if (selectedPart->visible()) {
            vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
            if (actor != nullptr) {
                /* adding to existing renderer*/
                renderer->AddActor(actor);
                renderer->ResetCamera();
                renderer->ResetCameraClippingRange();
            }
        }
    }

    /* Check to see if this part have any children */
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    /* Loop through children and add their actors*/
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}

/**
* Push button (opens the colour changer) to display whether the dialog is accepted or rejected
*/
void MainWindow::on_pushButton_2_clicked()
{
    this->on_actionactionItem_Options_triggered();
}

/**
* @brief Advanced colour picker to select from a wide variety for the model
*/
void MainWindow::on_actionactionItem_Options_triggered()
{
    /**
    * @brief sets the colour using the dialog box
    */
    //OptionDialog dialog(this);

    /* Get index of selected item */
    QModelIndex index = ui->treeView->currentIndex();
    /* Get a pointer to the item from the index */
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    /* We will retrieve the name string from the internal QVariant data array */
    QString item_name = selectedPart->data(0).toString();

    QColor color = QColorDialog::getColor(Qt::white, this, "Select model color").rgb();

        if (color.isValid()) {
            QString new_file_name = selectedPart->data(0).toString();

            selectedPart->setColour(color.red(), color.green(), color.green());

            int r_colour = color.red();
            int g_colour = color.green();
            int b_colour = color.blue();

            if (selectedPart->getActor()) {
                selectedPart->getActor()->GetProperty()->SetColor(r_colour / 255., g_colour / 255., b_colour / 255.);
            }
            updateRender();
        }
}

/**
* @brief Retrieves the model, renders using thr VRRenderThread class and starts the VR thread.
*/
void MainWindow::on_actionactionstartVR_triggered()
{
    /* Retrieve the selected index and obtian a pointer to a ModelPart*/
    //renderWindow->SetInteractor(nullptr);
    thread = new VRRenderThread();

    //prepare VR floor
    vtkNew<vtkPlaneSource> VRplaneSource;
    VRplaneSource->SetOrigin(0.0, 0.0, 0.0);
    VRplaneSource->SetPoint1(250.0, 0.0, 0.0);
    VRplaneSource->SetPoint2(0.0, 250.0, 0.0);
    VRplaneSource->SetCenter(90.0, 0.0, 0.0);
    VRplaneSource->Update();
    vtkPolyData* VRplane = VRplaneSource->GetOutput();
    vtkNew<vtkPolyDataMapper> VRbgmapper;
    VRbgmapper->SetInputData(VRplane);
    VRfloor = vtkSmartPointer<vtkActor>::New();
    VRfloor->SetMapper(VRbgmapper);
    VRfloor->GetProperty()->SetColor(1., 1., 1.);
    thread->addActorOffline(VRfloor);

    QModelIndex index = ui->treeView->currentIndex();
    startVRFromTree(index, thread);
    thread->start();
}

void MainWindow::startVRFromTree(const QModelIndex& index, VRRenderThread* thread) {
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        // retrieve actor from selected part and add to renderer
        if (selectedPart->visible()) {
            vtkSmartPointer<vtkActor> newActor = selectedPart->getNewActor();
            if (newActor != nullptr) {
                // if actor is initialized
                thread->addActorOffline(newActor);
            }
        }
    }
    // check to see if this part has any children
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }
    // loop through children and add their actors
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        startVRFromTree(partList->index(i, 0, index), thread);
    }
}


/** @brief This slot function allows folder to be selected (Open file dialog box, directory, option dialog box)(CAD STL files) and loads all the stl file and updates the rendering
* Contains the camera range
*/

void MainWindow::on_actionOpen_Folder_triggered()
{
    emit statusUpdateMessage(QString("Open folder action triggered"), 0);
    // get index
    QModelIndex parentIndex = ui->treeView->currentIndex();
    // get selected modelPart
    ModelPart* selectedPart = static_cast<ModelPart*>(parentIndex.internalPointer());
    // get folder name
    QString folderPath = QFileDialog::getExistingDirectory(this,
        tr("Open Folder"),
        "C:\\",
        QFileDialog::ShowDirsOnly);
    // check folder
    if (!folderPath.isEmpty()) {
        QDir dir(folderPath);
        if (dir.exists()) {
            // change item name to foler name
            QFileInfo folderInfo(folderPath);
            QString folderName = folderInfo.fileName();
            selectedPart->set(0, folderName);
            // check every file inside
            QStringList fileNameList = dir.entryList(QDir::Files);
            for (int i = 0; i < fileNameList.size(); i++) {
                // is the file STL?
                QFileInfo fileInfo(fileNameList[i]);
                QString fileType = fileInfo.completeSuffix();
                if (fileType == QString("stl")) {
                    QString filePath = dir.absoluteFilePath(fileNameList[i]);
                    loadFile(filePath, parentIndex);
                }
            }
        }
        else {
            emit statusUpdateMessage(QString("Folder not found"), 0);
        }
    }
    else {
        // No file selected or opened
        emit statusUpdateMessage(QString("No folder selected."), 0);
    }
}

/**
    * @brief loads the stl files
    */

void MainWindow::loadFile(QString fileName, QModelIndex parentIndex) {
    // File selected or opened successfully
    // get file name
    QFileInfo info(fileName);
    QString File_Name = info.baseName();
    // create new item and attach it to the list
    QString visible("true");
    QModelIndex newIndex = partList->appendChild(parentIndex, { File_Name, visible });
    // get pointer to the new item
    ModelPart* newItem = static_cast<ModelPart*>(newIndex.internalPointer());
    // load STL
    newItem->loadSTL(fileName);
    emit statusUpdateMessage(File_Name + QString(" selected"), 0);
    updateRender();
}

/**
* @brief Reset button to bring the model to initial state and remove all the colors/model
*/

void MainWindow::on_pushButton_3_clicked()
{
    emit statusUpdateMessage(QString("Reset button was clicked"), 0);

    renderer->RemoveAllViewProps();

    /* Remove File Name from Tree View */
    QModelIndex index = ui->treeView->currentIndex();

    // Remove the selected item from the model
    partList->removeRow(index.row(), index.parent());
    // Update the view to reflect the changes in the model
    ui->treeView->update();


    /* Create an object and add to renderer (this will change later to display a CAD
    model)) */
    /* Will just copy and paste cylinder example from before */
    // This creates a polygonal cylinder model with eight circumferential facets
    // (i.e, in practice an octagonal prism).
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    // The mapper is responsible for pushing the geometry in to the graphics
    // library. It may also do color mapping, if scalars or other attributes are
    // defined.
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // The actor is a grouping mechanism: besides the geometry (mapper), it
    // also has a property, transformation matrix, and/or texture map.
    // Here we set its color and rotate it around the X and Y axes.
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 1., 1.);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    /* Reset Camera (probably needs to go in its own function that is called whenever
     model is changed) */
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}

/* Reset Model to initial Color */

/**
* @brief Button for model to set initial color
*/
void MainWindow::on_pushButton_clicked()
{
    /* Get index of selected item */
    QModelIndex index = ui->treeView->currentIndex();

    /* Retrieve actor from selected part */
    ModelPart* selectedPart = static_cast <ModelPart*>(index.internalPointer());

    /* adding to existing renderer*/
    renderer->AddActor(selectedPart->getActor());
    selectedPart->getActor()->GetProperty()->SetColor(1., 1., 1.);
}

/**
* @brief Double click for the model parts instead of selecting everytime
*/
void MainWindow::on_treeView_doubleClicked(const QModelIndex& index)
{
    this->updateRender();
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    QModelIndex index = ui->treeView->currentIndex();
    // get selected modelPart
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString visible_str;
    if (arg1) {
        visible_str = QString("true");
    }
    else {
        visible_str = QString("false");
    }
    selectedPart->setVisible(getIsVisible());
    selectedPart->set(1, visible_str);
    this->updateRender();
}


void MainWindow::on_actionStopVR_triggered()
{
    thread->stop();
    thread->exit();
    thread->wait();
    thread->deleteLater();
    renderWindow->SetInteractor(interactor);
}


void MainWindow::on_actionRename_File_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    // get selected modelPart
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    emit statusUpdateMessage(QString("File Rename was clicked"), 0);
    OptionDialog dialog(this);
    dialog.setWindowTitle("Rename File");
    dialog.setTextName(selectedPart->data(0).toString());
    if (dialog.exec() == QDialog::Accepted) {
        // get and set name
        selectedPart->set(0, dialog.getTextName());
    }
}

