/**     @file optiondialog.cpp
  *     The Mainwindow class contains various functions and the constructors for the user-interface
  *     Group 10
  */

#include "optiondialog.h"
#include "ui_optiondialog.h"



OptionDialog::OptionDialog(QWidget* parent) :    QDialog(parent),    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    void pushButton_2();
}


OptionDialog::~OptionDialog()
{
    delete ui;
}

//get functions to receive the r,g,b values as entered by user from the slider
//set functions to set the colour to the values as received by the get functions

/**
* @brief Gets the R value of the RGB (color)
*/

void OptionDialog::setTextName(QString _Name) {

    ui->lineEdit->setText(_Name);
}

QString OptionDialog::getTextName() {

    return ui->lineEdit->text();
}

//void OptionDialog::_setTransparency(int _Transparency) {
//
//    ui->horizontalSlider->setValue(_Transparency*100);
//}
//
//int OptionDialog::_GetTransparency() {
//    return ui->horizontalSlider->value();
//}
//
//int OptionDialog::_GetRValue() {
//    return ui->horizontalSlider->value();
//}
//
///**
//* @brief sets the R value of the RGB (color)
//*/
//void OptionDialog::SetRValue(int r) {
//    return ui->horizontalSlider->setValue(r);
//}
//
///**
//* @brief Gets the G value of the RGB (color)
//*/
//int OptionDialog::_GetGValue() {
//    return ui->horizontalSlider_2->value();
//}
//
///**
//* @brief sets the G value of the RGB (color)
//*/
//void OptionDialog::SetGValue(int g) {
//    return ui->horizontalSlider_2->setValue(g);
//}
//
///**
//* @brief Gets the B value of the RGB (color)
//*/
//int OptionDialog::_GetBValue() {
//    return ui->horizontalSlider_3->value();
//}
//
///**
//* @brief sets the B value of the RGB (color)
//*/
//void OptionDialog::SetBValue(int b) {
//    return ui->horizontalSlider_3->setValue(b);
//}

/**
* @brief sets colour and visibility checkbox
*/
//void OptionDialog::SetCheckBox(bool v) {
//    ui->checkBox->setChecked(v);
//}
//
///**
//* @brief gets colour and visibility checkbox
//*/
//bool OptionDialog::GetCheckBox() {
//    return ui->checkBox->isChecked();
//}
