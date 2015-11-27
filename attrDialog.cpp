
#include "attrDialog.h"
#include <stdlib.h>
#include <QSettings>

static bool closeFlag = true;
bool attrDialog::copyFileToPath()
{

       QString toDir;
       bool coverFileIfExist=true;
       bool lastcheckstate=setdesktopstratbox->checkState();
       char *path = getenv("HOME");

       if(lastcheckstate)
      {
       qDebug()<<"is";
       QString sourceDir="/usr/lib/fangdeQC/剪切板管理工具";
       QString toDir=QString(QLatin1String(path))+"/.gnome2/nemo-scripts/剪切板管理工具";
       toDir.replace("\\","/");
       //qDebug()<<toDir;
       if (sourceDir == toDir){
           return true;
       }
       if (!QFile::exists(sourceDir)){
           return false;
       }
       QDir *createfile = new QDir;
       bool exist = createfile->exists(toDir);
       if (exist){
           if(coverFileIfExist){
               createfile->remove(toDir);
           }
       }
       //qDebug()<<"begin";
       if(!QFile::copy(sourceDir,toDir))
       {
           //qDebug()<<"inging";
           return false;
       }
       return true;
       }else{
          qDebug()<<"else";
          QString filename=QString(QLatin1String(path))+"/.gnome2/nemo-scripts/剪切板管理工具";
          filename.replace("\\","/");
          QFile file(filename);
          if(file.exists())
          {
           file.setPermissions(QFile::WriteOwner);
           file.remove();
           return true;
          }
       }
}


attrDialog::attrDialog(QWidget *parent) : QDialog(parent)
{

    code = QTextCodec::codecForLocale();
    QString titleValue="属性设置";
    titleValue+="         ";
    this ->setWindowTitle(titleValue);
    this ->setWindowIcon(QIcon(":images/icon.png"));//:images/icon.png
    cutLabel = new QLabel(code ->toUnicode("剪切板"));
    cutselectbox = new QCheckBox(code ->toUnicode("开启记录功能"));
    viewBox= new QCheckBox(code ->toUnicode("开启显示功能"));

    fontSizeLabel = new QLabel(code ->toUnicode("剪切板字体大小"));
    fontSizeBox = new QComboBox(this);
    for(int i = 1;i <= 30;i ++)
    {
        fontSizeBox ->addItem(QString::number(i));
    }

    QHBoxLayout *hsizeLayout = new QHBoxLayout;
    hsizeLayout ->addSpacing(24);
    hsizeLayout ->addWidget(fontSizeLabel);
    hsizeLayout ->addWidget(fontSizeBox);
    hsizeLayout ->addStretch();

    QVBoxLayout *vcutLayout = new QVBoxLayout;
    vcutLayout ->addWidget(cutLabel);
    vcutLayout ->addWidget(viewBox);
    vcutLayout ->addWidget(cutselectbox);
   // vcutLayout ->addWidget(cutcopybox);
    vcutLayout ->addLayout(hsizeLayout);


    setLabel = new QLabel(code ->toUnicode("行为设置"));
    setboldbox = new QCheckBox(code ->toUnicode("以粗体形式显示当前Ctrl+C复制的内容"));
    setrebootbox = new QCheckBox(code ->toUnicode("启动时剪切板恢复默认设置"));
    setautobox = new QCheckBox(code ->toUnicode("开启自动文本复制模式  (鼠标中键可实现粘贴功能)"));
    setdesktopstratbox = new QCheckBox(code ->toUnicode("右键桌面启动入口"));

    QVBoxLayout *vsetLayout = new QVBoxLayout;
    vsetLayout ->addWidget(setLabel);
    vsetLayout ->addWidget(setboldbox);
    vsetLayout ->addWidget(setrebootbox);
    vsetLayout ->addWidget(setautobox);
    vsetLayout ->addWidget(setdesktopstratbox);


    hotkeyLabel = new QLabel(code ->toUnicode("热键设置 (热键最终设置结果以主页菜单显示为准)"
                                              "\n\t例如：'Ctrl+O','Ctrl+Alt+O','Alt+O','F1'"));
    historyhotkeyLabel = new QLabel(code ->toUnicode("历史记录热键:"));
    editKeyLabel = new QLabel(code ->toUnicode("编辑热键:"));

    QRegExp regExp("[a-zA-z0-9+]+$");
    historyhotkeyEdit = new QLineEdit;
    historyhotkeyEdit ->setValidator(new QRegExpValidator(regExp,this));
    editKeyEdit = new QLineEdit;
    editKeyEdit ->setValidator(new QRegExpValidator(regExp,this));


    QHBoxLayout *hhistoryhotkeyLayout = new QHBoxLayout;
    hhistoryhotkeyLayout ->addWidget(historyhotkeyLabel);
    hhistoryhotkeyLayout ->addWidget(historyhotkeyEdit);
    hhistoryhotkeyLayout ->addStretch();
    QHBoxLayout *hmenuhotkeyLayout = new QHBoxLayout;
    hmenuhotkeyLayout ->addWidget(editKeyLabel);
    hmenuhotkeyLayout ->addWidget(editKeyEdit);
    hmenuhotkeyLayout ->addStretch();

    QVBoxLayout *vhotkeyLayout = new QVBoxLayout;
    vhotkeyLayout ->addWidget(hotkeyLabel);
    vhotkeyLayout ->addLayout(hhistoryhotkeyLayout);
    vhotkeyLayout ->addLayout(hmenuhotkeyLayout);

    okButton = new QPushButton(code ->toUnicode("确定"));
    connect(okButton,SIGNAL(clicked()),this,SLOT(ok_button_event()));


    cancelButton = new QPushButton(code ->toUnicode("取消"));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

    QHBoxLayout *hbuttonLayout = new QHBoxLayout;
    hbuttonLayout ->addWidget(okButton);
    hbuttonLayout ->addStretch();
    hbuttonLayout ->addWidget(cancelButton);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout ->addLayout(vcutLayout);
    vboxLayout ->addStretch();
    vboxLayout ->addLayout(vsetLayout);
    vboxLayout ->addStretch();
    vboxLayout ->addLayout(vhotkeyLayout);
    vboxLayout ->addStretch();
    vboxLayout ->addLayout(hbuttonLayout);
    setLayout(vboxLayout);
    this ->setTabOrder(cutselectbox,fontSizeBox);
    this ->setFixedSize(400,400);
    readingSettings();
}


void attrDialog::writeSettings()
{

    QSettings settings("NFS-HLJ","Qcopy");
    /*check*/
    settings.setValue("cutselectbox",cutselectbox->isChecked());
    //settings.setValue("cutcopybox",cutcopybox->isChecked());
    settings.setValue("viewbox",viewBox->isChecked());
    settings.setValue("setboldbox",setboldbox->isChecked());
    settings.setValue("setrebootbox",setrebootbox->isChecked());
    settings.setValue("setautobox",setautobox->isChecked());
    settings.setValue("setdesktopstratbox",setdesktopstratbox->isChecked());
    settings.setValue("historyhotkeyEdit",historyhotkeyEdit->text());
    settings.setValue("editKeyEdit",editKeyEdit->text());
    settings.setValue("fontSizeBox",fontSizeBox->currentText());

}
void attrDialog::readingSettings()
{
    QSettings settings("NFS-HLJ","Qcopy");
    /*check*/
    bool cutselectboxcheck=settings.value("cutselectbox",true).toBool();
    cutselectbox->setChecked(cutselectboxcheck);

    bool viewboxcheck=settings.value("viewbox",true).toBool();
    viewBox->setChecked(viewboxcheck);

    //bool cutcopyboxcheck=settings.value("cutcopybox").toBool();
    //cutcopybox->setChecked(cutcopyboxcheck);

    bool setboldboxcheck=settings.value("setboldbox").toBool();
    setboldbox->setChecked(setboldboxcheck);

    bool setrebootboxcheck=settings.value("setrebootbox").toBool();
    setrebootbox->setChecked(setrebootboxcheck);

    bool setautoboxcheck=settings.value("setautobox").toBool();
    setautobox->setChecked(setautoboxcheck);

    bool dtlastcheck=settings.value("setdesktopstratbox").toBool();
    setdesktopstratbox->setChecked(dtlastcheck);

    QString historyhotkeyStr=settings.value("historyhotkeyEdit").toString();
    historyhotkeyEdit->setText(historyhotkeyStr);

    QString editKeyEditStr=settings.value("editKeyEdit").toString();
    editKeyEdit->setText(editKeyEditStr);

    QString sizeFontBoxEdit=settings.value("fontSizeBox").toString();
    if(!sizeFontBoxEdit.isEmpty())
    {
        fontSizeBox ->setCurrentIndex(sizeFontBoxEdit.toInt() - 1);
    }
    else
    {
        fontSizeBox ->setCurrentIndex(9);
    }

}


void attrDialog::recoverySetting()
{
    cutselectbox ->setChecked(true);
    viewBox ->setChecked(true);
    fontSizeBox ->setCurrentIndex(9);

    setboldbox ->setChecked(false);
    setrebootbox ->setChecked(false);
    setautobox ->setChecked(false);
    setdesktopstratbox ->setChecked(false);

    historyhotkeyEdit ->setText("Ctrl+O");
    editKeyEdit ->setText("Ctrl+E");
    writeSettings();
}

int attrDialog::exec()
{
    QDialog::exec();
    return 1;
}

void attrDialog::ok_button_event()
{
    QStringList keyList;
    QStringList nameList;
    QString historyText = historyhotkeyEdit ->text();
    QString editText = editKeyEdit ->text();
    QString historyKey = QKeySequence(historyText).toString();
    QString editKey = QKeySequence(editText).toString();
    int historyL = historyText.length();
    int editL = editText.length();
    if((!editText.isEmpty() && editKey.isEmpty()) ||(editKey  =="WWW" ) || (editL != editKey.length()) || (editText.right(1) == "+"))
    {
        QMessageBox message(QMessageBox::Critical, "属性设置错误        ",
                            "<p>编辑热键设置非法 ！</p>"
                            "<p>""请重新设置 !" "</font></strong></p>"
                            ,QMessageBox::Ok ,this,Qt::Tool);
        this ->activateWindow();
        message.exec();
        return;
    }

    if((!historyText.isEmpty() && historyKey.isEmpty()) || (historyKey == "WWW") || (historyL != historyKey.length()) || (historyText.right(1) == "+"))
    {
        QMessageBox message(QMessageBox::Critical, "属性设置错误        ",
                            "<p>历史记录热键设置非法 ！</p>"
                            "<p>""请重新设置 !" "</font></strong></p>"
                            ,QMessageBox::Ok ,this,Qt::Tool);
        this ->activateWindow();
        message.exec();
        return;
    }

    keyList << "Ctrl+R" << "Ctrl+S" << "Ctrl+A" << "Ctrl+Q"
                   <<"Alt+A" << "Alt+I" << "Alt+F" << "Alt+E" << "Alt+T" << "Alt+H"
                   <<"Alt+C" << "Alt+O" << "Alt+N" << "Alt+S";
    nameList << "清除历史记录(Ctrl+R)" << "输出(Ctrl+S)" << "属性(Ctrl+A)" << "退出(Ctrl+Q)"
                        << "关于(Alt+A)" << "使用说明(Alt+I)" << "文件(Alt+F)" << "编辑(Alt+E)" << "工具(Alt+T)" << "帮助(Alt+H)"
                        << "取消(Alt+C)" << "确定(Alt+O)" << "否(Alt+N)"<< "保存(Alt+S)";
    if(!historyKey.isEmpty())
    {
        if(historyKey == editKey)
        {
            QMessageBox message(QMessageBox::Critical, "属性设置错误            ",
                                "<p>禁止将历史记录与编辑的热键设置为同一热键</p>"
                                "<p>""请重新设置 !" "</font></strong></p>"
                                ,QMessageBox::Ok ,this,Qt::Tool);
            this ->activateWindow();
            message.exec();
            return;
        }
    }

    for(int i = 0;i < keyList.length();i++)
    {
        if(historyKey == keyList[i])
        {
            QMessageBox message(QMessageBox::Critical, "属性设置错误        ",
                                 "<p>""历史记录热键设置与 '"+ nameList[i] +"' 快捷方式冲突" "</p>"
                                "<p>""请重新设置 !" "</font></strong></p>"
                                ,QMessageBox::Ok ,this,Qt::Tool);
            this ->activateWindow();
            message.exec();
            return;
        }
    }

    for(int i = 0;i < keyList.length();i++)
    {
        if(editKey == keyList[i])
        {
            QMessageBox message(QMessageBox::Critical, "属性设置错误        ",
                                "<p>""编辑热键设置与 '"+ nameList[i] +"' 快捷方式冲突" "</p>"
                                "<p>""请重新设置 !" "</font></strong></p>"
                                ,QMessageBox::Ok ,this,Qt::Tool);
            this ->activateWindow();
            message.exec();
            return;
        }
    }

    closeFlag = false;
    this ->close();
}

void attrDialog::closeEvent(QCloseEvent *)
{
        if(closeFlag)
        {
                    readingSettings();
        }
        else
        {
                  writeSettings();
        }

        copyFileToPath();
        closeFlag = true;

}


void attrDialog::keyPressEvent(QKeyEvent *event)
{
        if(event ->key() == Qt::Key_Escape)
        {
                this ->close();
                return;
        }
       else  if(event ->key() == Qt::Key_Enter ||event ->key() == Qt::Key_Return)
        {
            if(setboldbox ->hasFocus())
            {
                setboldbox->setChecked(!setboldbox->isChecked());
            }
            if( setrebootbox ->hasFocus())
            {
                setrebootbox->setChecked(!setrebootbox->isChecked());
            }
            if( setautobox ->hasFocus())
            {
               setautobox->setChecked(!setautobox->isChecked());
            }
            if( setdesktopstratbox ->hasFocus())
            {
                setdesktopstratbox->setChecked(!setdesktopstratbox->isChecked());
            }
            if(cutselectbox->hasFocus())
            {
               cutselectbox->setChecked(!cutselectbox->isChecked());
            }
            if( viewBox->hasFocus())
            {
                viewBox->setChecked(!viewBox->isChecked());
            }
        }
}
