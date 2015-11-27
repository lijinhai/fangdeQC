#ifndef EDITDIALOG_H
#define EDITDIALOG_H
#include <QtGui>
#include "attrDialog.h"
class editDialog : public QDialog
{
    Q_OBJECT
private:
    QTextCodec *code;
    QPushButton * saveButton;
    QPushButton * cancelButton;

    QLabel *infoLabel;
    QTextEdit *editText;
    QString content;
    attrDialog *aDialog;

    void initEditDialog();
public:
    editDialog(QWidget *parent = 0);
    int exec();
private slots:
    void ok_button_changed();
    void cancel_button();
    void closeEvent(QCloseEvent *event);
};
#endif // EDITDIALOG_H
