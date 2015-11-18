#include "helpDialog.h"
#include <QWidget>
helpDialog::helpDialog(const QString &path, const QString &page, QWidget *parent)
{
    this->setWindowFlags( Qt::WindowStaysOnTopHint|Qt::SubWindow);
    this ->setWindowIcon(QIcon(":images/icon.png"));//:images/icon.png
    setFixedSize(565,540);
    setFocus();
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_GroupLeader);
    textBrowser=new QTextBrowser;
    textBrowser->setOpenLinks(true);

    homeButton=new QToolButton();
    QPixmap homeButtonpix(":/images/home.png");
    homeButton->setIcon(homeButtonpix);
    homeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    homeButton->setIconSize(homeButtonpix.size());
    homeButton->setText(" 首  页 ");
    homeButton->setShortcut(tr("Home"));


    upSection=new QToolButton();
    QPixmap upSectionpix(":/images/up.png");
    upSection->setIcon(upSectionpix);
    upSection->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    upSection->setIconSize(upSectionpix.size());
    upSection->setText("上一节");
    upSection->setToolTip("已经是首页了◕‿◕");
    upSection->setShortcut(tr("Left"));

    downSection=new QToolButton();
    QPixmap downSectionpix(":/images/down.png");
    downSection->setIcon(downSectionpix);
    downSection->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    downSection->setIconSize(downSectionpix.size());
    downSection->setText("下一节");
    downSection->setShortcut(tr("Right"));

    //backButton=new QToolButton(tr("&Back"));
    closeButton=new QToolButton();
    QPixmap closeButtonpix(":/images/out.png");
    closeButton->setIcon(closeButtonpix);
    closeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    closeButton->setIconSize(closeButtonpix.size());
    closeButton->setText(" 退  出 ");
    closeButton->setShortcut(tr("Esc"));

    QHBoxLayout *buttonlayout=new QHBoxLayout;
    buttonlayout->addWidget(homeButton);
    buttonlayout->addWidget(upSection);
    buttonlayout->addWidget(downSection);

    //buttonlayout->addStretch();
    buttonlayout->addWidget(closeButton);
    buttonlayout->setSpacing(70);
    QVBoxLayout *mainLayout =new QVBoxLayout;
    mainLayout->addLayout(buttonlayout);
    mainLayout->addWidget(textBrowser);
    setLayout(mainLayout);

    connect(homeButton,SIGNAL(clicked()),textBrowser,SLOT(home()));
    //connect(backButton,SIGNAL(clicked()),textBrowser,SLOT(backward()));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));

    connect(upSection,SIGNAL(clicked()),this,SLOT(forwardSection()));
    connect(downSection,SIGNAL(clicked()),this,SLOT(backwardSection()));
    connect(textBrowser,SIGNAL(sourceChanged(const QUrl &)),this,SLOT(updateWindowTitle()));
    textBrowser->setSearchPaths(QStringList()<< path <<":/images");
    textBrowser->setSource(page);
    setMouseTracking(true);
}

void helpDialog::updateWindowTitle()
{
   QString titlevalue=  textBrowser->documentTitle();
   titlevalue=titlevalue+"         ";
   setWindowTitle(titlevalue);
}
void helpDialog::mouseMoveEvent(QMouseEvent *event)
{
  event->accept();
  qDebug()<<"find";
  //fileMenu->clearFocus();
  QString windowTitleString=textBrowser->documentTitle();
  int eresult=QString::localeAwareCompare(windowTitleString,"使用说明");
  if(0==eresult)
  {
   upSection->setToolTip("已经是首页了◕‿◕");
  }else{
   upSection->setToolTip("");
  }
  int presult=QString::localeAwareCompare(windowTitleString,"其他功能说明");
  if(0==presult)
  {
   downSection->setToolTip("已经是最后一节了◕‿◕");
  }else{
   downSection->setToolTip("");
  }
}
void helpDialog::forwardSection()
{
    qDebug()<<"start";
    downSection->setToolTip("");
    QString windowTitleString=textBrowser->documentTitle();
    int eresult=QString::localeAwareCompare(windowTitleString,"信息编辑与输出说明");
    if(0==eresult)
    {
     upSection->setToolTip("已经是首页了◕‿◕");
     textBrowser->setSource(QUrl("main.html"));
     qDebug()<<eresult;
    }

    int hresult=QString::localeAwareCompare(windowTitleString,"历史记录说明");
    if(0==hresult)
    {
     upSection->setToolTip("");
     textBrowser->setSource(QUrl("infoedit.html"));
     qDebug()<<hresult;
    }

    int presult=QString::localeAwareCompare(windowTitleString,"属性设置说明");
    if(0==presult)
    {
     upSection->setToolTip("");
     textBrowser->setSource(QUrl("histroyrecord.html"));
     qDebug()<<presult;
    }

    int oresult=QString::localeAwareCompare(windowTitleString,"其他功能说明");
    if(0==oresult)
    {
     upSection->setToolTip("");
     textBrowser->setSource(QUrl("propertyset.html"));
     qDebug()<<presult;
    }

    int cresult=QString::localeAwareCompare(windowTitleString,"使用说明");
    if(0==cresult)
    {
     //textBrowser->setSource(QUrl("histroyrecord.html"));
        upSection->setToolTip("已经是首页了◕‿◕");
        qDebug()<<cresult;
    }

}

void helpDialog::backwardSection()
{
    upSection->setToolTip("");
    qDebug()<<"start";
    QString windowTitleString=textBrowser->documentTitle();
    int eresult=QString::localeAwareCompare(windowTitleString,"信息编辑与输出说明");
    if(0==eresult)
    {
     downSection->setToolTip("");
     textBrowser->setSource(QUrl("histroyrecord.html"));
     qDebug()<<eresult;
    }

    int hresult=QString::localeAwareCompare(windowTitleString,"历史记录说明");
    if(0==hresult)
    {
     //downSection->setToolTip("已经是最后一节了◕‿◕");
     textBrowser->setSource(QUrl("propertyset.html"));
     qDebug()<<hresult;
    }

    int presult=QString::localeAwareCompare(windowTitleString,"属性设置说明");
    if(0==presult)
    {
     textBrowser->setSource(QUrl("others.html"));
     downSection->setToolTip("已经是最后一节了◕‿◕");
     qDebug()<<presult;
    }
    int oresult=QString::localeAwareCompare(windowTitleString,"其他功能说明");
    if(0==oresult)
    {
     downSection->setToolTip("已经是最后一节了◕‿◕");
     qDebug()<<presult;
    }
    int cresult=QString::localeAwareCompare(windowTitleString,"使用说明");
    if(0==cresult)
    {
     downSection->setToolTip("");
     textBrowser->setSource(QUrl("infoedit.html"));
     qDebug()<<cresult;
    }

}

QDir directoryOf(const QString &subdir)
{
    QDir dir("/usr/share/doc/fangdeQC");//QApplication::applicationDirPath()
    dir.cd(subdir);
    dir.cd(subdir);
    return dir;
}

void helpDialog::showPage(const QString &page)
{
    QString path=directoryOf("doc").absolutePath();
    qDebug()<<path;
    qDebug()<<page;
    //directoryOf("doc").absoluteFilePath(dbzhang800.txt);
    helpDialog *browser=new helpDialog(path,page);
    browser->resize(500,400);
    browser->show();
}

void helpDialog::closeEvent(QCloseEvent * event)
{
        event ->ignore();
        this ->hide();
}
