/**     @file ModelPart.cpp
  *
  *     EEEE2046 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"


/* Commented out for now, will be uncommented later when you have
 * installed the VTK library
 */

 /* VTK headers */

#include <vtkDataSetMapper.h>
#include <vtkSTLReader.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>




ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent) {

    /* You probably want to give the item a default colour */
}


ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}


void ModelPart::appendChild( ModelPart* item ) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}


ModelPart* ModelPart::child( int row ) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}


int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const {
    /* Return the data associated with a column of this item 
     *  Note on the QVariant type - it is a generic placeholder type
     *  that can take on the type of most Qt classes. It allows each 
     *  column or property to store data of an arbitrary type.
     */
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}


void ModelPart::set(int column, const QVariant &value) {
    /* Set the data associated with a column of this item 
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}


ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}


int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    /* This is a placeholder function that will be used in the next worksheet */
    //setting new colours
     r = R;
     b = B;
     g = G;

    //changing colour in the actor
    
    
    /* As the name suggests ... */
}

void ModelPart::setTransparency(const unsigned char Tr) {
    m_opacity = Tr;
}

unsigned char ModelPart::GetTransparency() {
    /* This is a placeholder function that will be used in the next worksheet */

    /* As the name suggests ... */
    return m_opacity;   // needs updating
}

unsigned char ModelPart::getColourR() {
    /* This is a placeholder function that will be used in the next worksheet */
    
    /* As the name suggests ... */
    //return colour.GetRed;   // needs updating
    return r;
}

unsigned char ModelPart::getColourG() {
    /* This is a placeholder function that will be used in the next worksheet */
    
    /* As the name suggests ... */
    return g;   // needs updating
}


unsigned char ModelPart::getColourB() {
    /* This is a placeholder function that will be used in the next worksheet */
    
    /* As the name suggests ... */
    return b;   // needs updating
}


void ModelPart::setVisible(bool _isVisible) {
    /* This is a placeholder function that will be used in the next worksheet */
    
    /* As the name suggests ... */
    isVisible = _isVisible;
}

bool ModelPart::visible() {
    /* This is a placeholder function that will be used in the next worksheet */
    
    /* As the name suggests ... */
    return isVisible;
}

void ModelPart::loadSTL( QString fileName ) {
    /* This is a placeholder function that will be used in the next worksheet */
    
    /* 1. Use the vtkSTLReader class to load the STL file 
     *     https://vtk.org/doc/nightly/html/classvtkSTLReader.html
     */

    vtkNew<vtkSTLReader> reader;

    reader->SetFileName(fileName.toStdString().c_str());
    reader->Update();

    /* 2. Initialise the part's vtkMapper */

    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());
    
    /* 3. Initialise the part's vtkActor and link to the mapper */
    
    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //actor->AddPosition(-150, -50, -100);
}

vtkSmartPointer<vtkActor> ModelPart::getActor() {
    /* This is a placeholder function that will be used in the next worksheet */
    
    /* Needs to return a smart pointer to the vtkActor to allow
     * part to be rendered.
     */
    return actor;
}

vtkActor* ModelPart::getNewActor() {
    /* This is a placeholder function that will be used in the next worksheet.
     * 
     * The default mapper/actor combination can only be used to render the part in 
     * the GUI, it CANNOT also be used to render the part in VR. This means you need
     * to create a second mapper/actor combination for use in VR - that is the role
     * of this function. */
     
    if (file != nullptr) {
        /* 1. Create new mapper */
        vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper2->SetInputConnection(file->GetOutputPort());

        /* 2. Create new actor and link to mapper */
        vtkActor* actor2 = vtkActor::New();
        actor2->SetMapper(mapper2);

        /* 3. Link the vtkProperties of the original actor to the new actor. This means
         *    if you change properties of the original part (colour, position, etc), the
         *    changes will be reflected in the GUI AND VR rendering.
         *
         *    See the vtkActor documentation, particularly the GetProperty() and SetProperty()
         *    functions.
         */

        actor2->SetProperty(actor->GetProperty());


        /* The new vtkActor pointer must be returned here */
        return actor2;
    }
    else {
        vtkSmartPointer<vtkActor> actor2;
        return actor2;
    }
    
}

