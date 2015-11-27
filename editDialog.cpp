#include "editDialog.h"

editDialog::editDialog(QWidget *parent) : QDialog(parent)
{
    initEditDialog();
}

int editDialog::exec()
{
    QDialog::exec();
    return 1;
}

void editDialog::ok_button_changed()
{
    QString str = editText ->toPlainText();
    content = editText ->toPlainText();
    QApplication::clipboard() ->setText(str);
    QMessageBox message(QMessageBox::Information, "编辑提示             ",
                        "保存成功 !"
                        ,QMessageBox::Ok ,this,Qt::Tool);
   this ->activateWindow();
   message.exec();


}

void editDialog::cancel_button()
{
    this ->close();
//    QString str = editText ->toPlainText();
//    if( str == content)
//    {
//        close();
//    }
//    else
//    {

//        QMessageBox message(QMessageBox::Question, "编辑提示            ",
//                            "是否将更改保存到剪切板 ?"
//                            ,QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel ,this,Qt::Tool);
//        int ret = message.exec();
//        if(ret == QMessageBox::Save)
//        {
//            QApplication::clipboard() ->setText(str);
//            close();
//        }
//        else if( ret == QMessageBox::No)
//        {
//            close();
//        }
//        else
//        {
//            return;
//        }
//    }
}


void editDialog::initEditDialog()
{
    code = QTextCodec::codecForLocale();
    QString titleValue="编辑系统剪切板";
    titleValue+="         ";
    setWindowTitle(titleValue);
    this ->setWindowIcon(QIcon(":images/icon.png"));//:images/icon.png
    saveButton = new QPushButton(code ->toUnicode("保存"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(ok_button_changed()));

    cancelButton = new QPushButton(code ->toUnicode("退出"));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancel_button()));

    infoLabel = new QLabel(code ->toUnicode("剪切板信息:"));
    QFont font;
    font.setBold(true);
    infoLabel ->setFont(font);

    editText = new QTextEdit;
    content = QApplication::clipboard()->text();

    QString str = QApplication::clipboard()->text();
    editText ->setPlainText(str);
    editText ->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
    QFormLayout *formLayout = new QFormLayout;
    formLayout ->addRow(infoLabel,editText);
    formLayout ->addRow(saveButton,cancelButton);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout ->addLayout(formLayout);
    setLayout(vboxLayout);
    //this ->setMinimumWidth(400);
   this ->setFixedSize(400,240);
}

void editDialog::closeEvent(QCloseEvent *event)
{
    QString str = editText ->toPlainText();
    if( str == content)
     {
        close();
     }
      else
     {
        QMessageBox message(QMessageBox::Question, "编辑提示            ",
                                "是否将更改保存到剪切板 ?",
                                         QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel ,this,Qt::Tool);;
       this ->activateWindow();
       int ret = message.exec();
       if(ret == QMessageBox::Save)
     {
         QApplication::clipboard() ->setText(str);
         close();
         event ->accept();
     }
      if(ret == QMessageBox::Cancel)
      {
          event->ignore();
      }
     else
      close();
     }
}
