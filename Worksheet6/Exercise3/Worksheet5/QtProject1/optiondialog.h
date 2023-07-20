#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
    class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

       
public:
    explicit OptionDialog(QWidget* parent = nullptr);
    ~OptionDialog();
    //Ui::OptionDialog* getUi();
    ////declaring get and set functions for the r,g,b colours entered by the user
    //int _GetRValue();
    //int _GetGValue();
    //int _GetBValue();
    //int _GetTransparency();
    void setTextName(QString _Name);
    //void SetRValue(int r);
    //void SetGValue(int g);
    //void SetBValue(int b);
    //void SetCheckBox(bool v);
    //bool GetCheckBox();
    QString getTextName();
    //void _setTransparency(int _Transparency);

private slots:


private:
    Ui::OptionDialog* ui;
};

#endif // OPTIONDIALOG_H
