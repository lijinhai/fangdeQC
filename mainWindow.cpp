#include "mainWindow.h"

static  int threadFlag = 1;
static int shortCut = 1;

mainWindow::mainWindow()
{
    createActions();
    createMenus();
    createToolBars();
    createTray();
    initWindow();
    setAllShortCut();
}

mainWindow::~mainWindow()
{
    delete aDialog;
    delete cDialog;
    delete eDialog;
}

int mainWindow::dnum = 0;
/*-------------------------------------------------PRIVATE-------------------------------------------------*/

void mainWindow::initWindow()
{
    aDialog = new attrDialog(this);
    aDialog ->copyFileToPath();
    cDialog = NULL;
    eDialog = NULL;
    fileInput = new QFileDialog(this);

    cOpenNum = 0;
    eOpenNum = 0;

    code = QTextCodec::codecForLocale();
    historyShort = NULL;
    editShort = NULL;
    cutBoard = new QTextEdit(this);
   cutBoard ->setReadOnly(true);
  // help = NULL;
   help = new helpDialog("/usr/share/doc/fangdeQC/doc" ,"main.html");

    progressTimer = new QTimer(this);
    connect(progressTimer,SIGNAL(timeout()),this,SLOT(finishTimeProgress()));
    thread = new Thread;
    progressDlg = new QProgressDialog(this);
    progressDlg->setFont(font);
    progressDlg ->setModal(true);
    progressDlg->setWindowModality(Qt::WindowModal);
    progressDlg->setMinimumDuration(5);
    progressDlg->setWindowTitle("文件输出            ");
    connect(progressDlg, SIGNAL(canceled()), this, SLOT(progressDlgcancel()));

    this ->setCentralWidget(cutBoard);
    this ->setWindowIcon(QIcon(":images/icon.png"));
    this ->setWindowTitle(code ->toUnicode("剪切板管理工具"));
    this ->setMinimumSize(300,350);
    this ->setGeometry(0,0,400,450);
    font.setPointSize(aDialog ->fontSizeBox ->currentText().toInt());
    if(aDialog ->setboldbox ->isChecked())
    {
        font.setBold(true);
    }
    else
    {
        font.setBold(false);
    }

    connect(QApplication::clipboard(),SIGNAL(dataChanged()),this,SLOT(cut_board_changed()));
    if(!(QApplication::clipboard()->text().isEmpty()))
    {
        initCutBoard();
    }
    else
    {
         clearInfoAction ->setEnabled(false);
    }

}


void mainWindow::initCutBoard()
{
    QString str = QApplication::clipboard() ->text();
    QString fileType;
    QString content = "\n";
    QStringList text = str.split("\n");
     if(judgeIsFile())
     {
         getFileContent(content,fileType,text);
    }
    else
    {
        content = str;
        fileType = "文本";
    }

    if(aDialog ->cutselectbox ->isChecked())
    {
        writeFile(str,fileType,content);
    }

    if(aDialog ->viewBox ->isChecked())
    {
        cutBoard -> setPlainText(content);
        cutBoard ->setFont(font);
        clearInfoAction ->setEnabled(true);
    }
    else
    {
         clearInfoAction ->setEnabled(false);
    }
}


void mainWindow::getFileContent(QString &content,QString &fileType,QStringList text)
{

        content = "\n";
        for(int i = 0;i < text.length();i++)
        {
            QFileInfo fileInfo(text[i]);
            if(fileInfo.isDir())
            {
                if(fileType.isEmpty())
                {
                    fileType = "文件夹";
                }
                else if(fileType == "文件")
                {
                        fileType += ", 文件夹";
                }
                content += code ->toUnicode("文件名: ") + text[i].split("/")[text[i].split("/").length() - 1];
                content +="\n" + code ->toUnicode("文件路径: ") + text[i];
                content += "\n" + code ->toUnicode("文件夹大小: ") +getFIleSize(getFolderSize(text[i]));
                content += "\n" + code ->toUnicode("文件夹修改时间: ") + fileInfo.created().toString("yyyy-MM-dd ddd hh:mm:ss");
                content += "\n" + code ->toUnicode("文件夹创建者: ") + fileInfo.owner();
                content += "\n" + code ->toUnicode("文件夹所属组: ") + fileInfo.group();
                content += "\n" + code ->toUnicode("文件类型：文件夹");
                content += "\n" + code ->toUnicode("文件夹权限： ");
                if(fileInfo.isReadable())
                    content += code ->toUnicode("可读");
                if(fileInfo.isWritable())
                    content += code ->toUnicode("/可写");
                if(fileInfo.isExecutable())
                    content += code ->toUnicode("/可执行");
               // fileType += "文件夹";
                content += "\n\n";
            }
            else
            {
                if(fileType.isEmpty())
                {
                    fileType = "文件";
                }
                else if(fileType == "文件夹")
                {
                        fileType += ", 文件";
                }
                content += code ->toUnicode("文件名: ") + text[i].split("/")[text[i].split("/").length() - 1];
                content +="\n" + code ->toUnicode("文件路径: ") + text[i];
               // str += "\n" + code ->toUnicode("文件类型： ") + fileInfo.suffix() + code ->toUnicode("文件");
                content += "\n" + code ->toUnicode("文件大小: ") +getFIleSize(fileInfo.size());
                content += "\n" +  code ->toUnicode("文件修改时间: ") +  fileInfo.created().toString("yyyy-MM-dd ddd hh:mm:ss");
                //str += "\n lastModified: " + fileInfo.lastModified().toString();
                //str += "\n lastRead: " + fileInfo.lastRead().toString();
                content += "\n" + code ->toUnicode("文件创建者: ") + fileInfo.owner();
                content += "\n" + code ->toUnicode("文件所属组: ") + fileInfo.group();
                if(fileInfo.suffix().isEmpty())
                {
                    content += "\n" + code ->toUnicode("文件类型： ") + "文件";
                }
                else
                {
                    content += "\n" + code ->toUnicode("文件类型： ") + "." + fileInfo.suffix() + "文件";
                }
                content += "\n" + code ->toUnicode("文件权限： ");
                if(fileInfo.isReadable())
                    content += code ->toUnicode("可读");
                if(fileInfo.isWritable())
                    content += code ->toUnicode("/可写");
                if(fileInfo.isExecutable())
                    content += code ->toUnicode("/可执行");
                //str += "\n group: " + fileInfo.permission()
                //fileType += "." + fileInfo.suffix() + "文件";
                content += "\n\n";
            }
        }
}


void mainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("文件(&F)");
    editMenu = menuBar()->addMenu("编辑(&E)");
    toolsMenu = menuBar()->addMenu("工具(&T)");
    helpMenu = menuBar()->addMenu("帮助(&H)");
     fileMenu->addAction(openAction);
     fileMenu ->addAction(removeFileAction);
     fileMenu ->addSeparator();
     fileMenu ->addAction(saveAction);
     fileMenu ->addSeparator();
     fileMenu ->addAction(attributeAction);
     fileMenu ->addSeparator();
     fileMenu ->addAction(exitAction);

     editMenu ->addAction(editInfoAction);
     editMenu ->addAction(clearInfoAction);

     toolsMenu ->addAction(cutPictureAction);

     wpsMenu = new QMenu("WPS工具",this);
     wpsMenu ->setIcon(QIcon(":images/wps.png"));
     wpsMenu ->addAction(wps1Action);
     wpsMenu ->addAction(wps2Action);
     wpsMenu ->addAction(wps3Action);

     toolsMenu ->addMenu(wpsMenu);
     toolsMenu ->addAction(geditAction);

     helpMenu ->addAction(aboutAction);
     helpMenu ->addAction(intructionsAction);
}

void mainWindow::createActions()
{

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    openAction = new QAction("打开历史记录", this);
    openAction->setIcon(QIcon(":/images/history.png"));
    openAction->setStatusTip("打开历史记录文件");
    connect(openAction, SIGNAL(triggered()), this, SLOT(openCutDialog()));

    saveAction = new QAction("输出",this);
    saveAction ->setIcon(QIcon(":/images/save.png"));
    saveAction->setStatusTip("将当前剪切板内容保存到文件中");
    saveAction ->setShortcut(QKeySequence::Save);
    connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));

    removeFileAction = new QAction("清除历史记录",this);
    removeFileAction->setStatusTip("清除所有历史记录");
    //removeFileAction = new QAction(tr("Delete History Record..."),this);
    removeFileAction ->setIcon(QIcon(":/images/remove.png"));
    removeFileAction ->setShortcut(tr("Ctrl+R"));
    connect(removeFileAction,SIGNAL(triggered()),this,SLOT(clear_history_event()));

    aboutAction = new QAction("关于", this);
    aboutAction ->setShortcut(tr("ALT+A"));
    aboutAction->setStatusTip("版本与版权说明");
    aboutAction->setIcon(QIcon(":/images/about.png"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(softwareInfo()));

    attributeAction = new QAction("属性",this);
    attributeAction ->setShortcut(tr("Ctrl+A"));
    attributeAction->setStatusTip("设置剪切板属性");
    attributeAction ->setIcon(QIcon(":/images/settings.png"));
    connect(attributeAction,SIGNAL(triggered()),this,SLOT(openAttrDialog()));

    intructionsAction = new QAction("使用说明",this);
    intructionsAction ->setShortcut(tr("ALT+I"));
    intructionsAction->setStatusTip("剪切板使用说明");
    intructionsAction ->setIcon(QIcon(":/images/help.png"));
    connect(intructionsAction,SIGNAL(triggered()),this,SLOT(intructions_clicked_data()));

    editInfoAction = new QAction("编辑信息",this);
    editInfoAction->setStatusTip("编辑当前剪切板内容");
    editInfoAction ->setIcon(QIcon(":/images/edit.png"));
    connect(editInfoAction,SIGNAL(triggered()),this,SLOT(openEditDialog()));
    //editInfoAction ->setShortcut(tr("Ctrl+E"));

    clearInfoAction = new QAction("清除信息",this);
    clearInfoAction->setStatusTip("删除当前剪切板内容");
    clearInfoAction ->setIcon(QIcon(":/images/delete.png"));
    connect(clearInfoAction,SIGNAL(triggered()),this,SLOT(del()));
    clearInfoAction->setShortcut(QKeySequence::Delete);

    cutPictureAction = new QAction("GIMP 图片编辑器",this);
    cutPictureAction ->setIcon(QIcon(":images/draw.png"));
    connect(cutPictureAction,SIGNAL(triggered()),this,SLOT(openlibreoffice()));

    wps1Action = new QAction("wps文字",this);
    wps1Action ->setIcon(QIcon(":images/word.png"));
    connect(wps1Action,SIGNAL(triggered()),this,SLOT(openwps()));
    wps2Action = new QAction("wps演示",this);
    wps2Action ->setIcon(QIcon(":images/ppt.png"));
    connect(wps2Action,SIGNAL(triggered()),this,SLOT(openwpp()));
    wps3Action = new QAction("wps表格",this);
    wps3Action ->setIcon(QIcon(":images/excel.png"));
    connect(wps3Action,SIGNAL(triggered()),this,SLOT(openet()));

    geditAction = new QAction("Gedit 编辑器",this);
    geditAction ->setIcon(QIcon(":images/gedit.png"));
    connect(geditAction,SIGNAL(triggered()),this,SLOT(opengedit()));

    exitAction = new QAction("退出",this);
    exitAction ->setShortcut(tr("Ctrl+Q"));
    exitAction ->setIcon(QIcon(":images/close.png"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
}

void mainWindow::createToolBars()
{
    cutToolBar = addToolBar("显示");
    cutToolBar ->addAction(clearInfoAction);
    cutToolBar ->addAction(editInfoAction);
    cutToolBar ->addAction(saveAction);

    cutToolBar ->addSeparator();
    cutToolBar ->addAction(openAction);
    cutToolBar ->addAction(removeFileAction);
    cutToolBar ->addSeparator();
    cutToolBar ->addAction(attributeAction);
    cutToolBar ->addSeparator();
    cutToolBar ->addAction(intructionsAction);
    cutToolBar->setFloatable(false);
    statusBar();
}


void mainWindow::createTray()
{
    miniSizeAction = new QAction("最小化",this);
//    miniSizeAction = new QAction("最小化(&H)",this);
    connect(miniSizeAction,SIGNAL(triggered()),this,SLOT(hide()));

    maxSizeAction = new QAction("最大化",this);
//     maxSizeAction = new QAction("最大化(&M)",this);
    connect(maxSizeAction,SIGNAL(triggered()),this,SLOT(showMaximized()));

    quitAction = new QAction("退出",this);
//    quitAction = new QAction("退出(&Q)",this);
    connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));

    trayAboutAction = new QAction("关于(&A)",this);
    connect(trayAboutAction,SIGNAL(triggered()),this,SLOT(softwareInfo()));


    openBoardAction = new QAction("打开主面板",this);
//    openBoardAction = new QAction("打开主面板(&S)",this);
    connect(openBoardAction,SIGNAL(triggered()),this,SLOT(showNormal()));


    trayMenu = new QMenu((QWidget*)QApplication::desktop());
    //trayMenu ->setFocus();
    trayMenu ->addAction(miniSizeAction);
    trayMenu ->addAction(maxSizeAction);
    trayMenu ->addAction(quitAction);
    trayMenu ->addSeparator();
    trayMenu ->addAction(openBoardAction);
    trayMenu ->addSeparator();
    trayMenu ->addAction(trayAboutAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon ->setIcon(QIcon(":/images/icon.png"));
    trayIcon ->setToolTip("剪切板管理工具");
    trayIcon ->setContextMenu(trayMenu);
    trayIcon ->show();
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconAivated(QSystemTrayIcon::ActivationReason)));
}



void mainWindow::softwareInfo()
{
    QString titleValue="版本与版权";
    titleValue+="             ";

    QMessageBox message(QMessageBox::NoIcon, titleValue,
                        "<p>版本号 2.0.0</p>"
                        "<p>剪切板管理工具是一款具有良好易用性及可操作性的系统软件</p>"
                        "<p>版权所有 © 2015-2020 中科方德软件有限公司</p>"
                        ,QMessageBox::Ok ,this);
    message.setIconPixmap(QPixmap(":/images/board.png"));
    message.setWindowIcon(QIcon(":/images/icon.png"));
//    message.setWindowFlags(Qt::Tool);
    if(message.exec() == QMessageBox::Ok )
    {
        if(!this ->isVisible())
        {
            this ->show();
            this ->hide();
        }
    }
}

bool mainWindow::writeFile(QString content,const QString fileType,const QString &file)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_time = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    QSqlQuery query;
    content = content.replace(QRegExp("'"),"''");
    QString header = "内容：" + content + "\n";
    header = header.replace(QRegExp("\n"),"");
    if(header.length() >= 50)
    {
        header = header.left(20) + "..." + header.right(20);
    }
    else
    {
        header = header;
    }


    QString deleteSql = "delete from info where content = '" + content + "'";
    query.exec(deleteSql);

//    QString timeSql = "insert into info values(" + QString::number(count) + ",'" + current_time + "','"
//                + fileType + "','"+ content + "')";
    QString insertSql = "insert into info(id,time,type,content,file) values(null,'"  + current_time + "','"
                + fileType + "','" + content + "','" + file + "')";
    query.exec(insertSql);
}

bool mainWindow::copyText(const QString &fileName,const QString &text)
{
    FILE * fd = fopen(code ->fromUnicode(fileName).data(),"w");
    if (fd)
    {
        fwrite(code ->fromUnicode(text).data(),strlen(code ->fromUnicode(text).data()),1,fd);
        fclose(fd);
    }
    else
    {
        qDebug() << "open file failed";
        return false;
    }
    return true;
}


bool mainWindow::judgeIsFile()
{
    QList<QUrl>urls = QApplication::clipboard() ->mimeData() ->urls();
    if(urls.isEmpty())
        return false;
    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty())
        return false;
    return true;
}

 QString mainWindow::getFIleSize(qint64 size)
 {
        QString unitName;
        QString fileSize;
        QString minUnit;
        qint64 remianSize = size;
        int count = 0;
        double dsize = (double)size;

        while( (remianSize /= 1024 ) != 0)
        {
                count ++;
        }

        switch(count)
        {
                case 0: unitName = " B";
                break;
                case 1: unitName = " KB";
                break;
                case 2: unitName = " MB";
                break;
                case 3: unitName = " GB";
                break;
                case 4: unitName = " TB";
                break;
                default: unitName = " unknown file size";
                break;
        }
        minUnit = "( " + QString::number(size) +" bytes )";
        if(count != 0)
        {
            for(int i = 0; i < count; i++)
                    dsize = dsize / 1024;
        }

        if(unitName == " B")
        {
                fileSize = QString::number(size) + unitName + minUnit;
        }
        else
        {
                fileSize = QString::number(dsize,'f',1) + unitName + minUnit;
        }


        return fileSize;
 }


qint64 mainWindow::getFileNum(const QString &path)
{      
    QDir dir(path);
    if (!dir.exists())
        return 0;
    qint64 fileNum = 0;
    QFileInfoList list = dir.entryInfoList();
    for(int i=0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if (fileInfo.isFile())
            fileNum ++;
        else if(fileInfo.isDir())
        {
            fileNum += getFileNum(fileInfo.filePath());
        }
    }
    return fileNum;
}


qint64 mainWindow::getFolderSize(const QString &path)
{
    QDir dir(path);
    QFileInfo file(path);
     qint64 totalsize = 0;
    if(!file.exists())
         return 0;
    if(file.isDir())
    {
        QFileInfoList list = dir.entryInfoList();
        for(int i=0; i < list.size(); i++)
        {
               QFileInfo fileInfo = list.at(i);
               if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
                   continue;
               if (fileInfo.isFile())
               {
                   totalsize += fileInfo.size();
               }
               else if(fileInfo.isDir())
               {
                   totalsize += getFolderSize(fileInfo.filePath());
               }
           }
    }
    else
    {
        totalsize = file.size();
    }

       return totalsize;
}

/*cut board content must be file or folder*/
qint64 mainWindow::getBoardSize(QString content)
{
        QStringList fileList = content.split("\n");
        qint64 totalSize = 0;
        for(int i = 0;i < fileList.length();i++)
        {
            totalSize += getFolderSize(fileList[i]);
        }
        return totalSize;
}


bool mainWindow::fileIsExit(QString content,QString toPath)
{
        QStringList fileList = content.split("\n");
        QFileInfo fileInfo(fileList[0]);
        QString fileName;
        qint64 length = fileList.length();

        if(toPath == fileInfo.absolutePath())
        {
                return true;
        }

        for(int i = 0;i <length;i++)
        {
            if(toPath != "/")
            {
                    fileName = toPath + "/" +  fileList[i].split("/")[fileList[i].split("/").length() - 1] ;
            }
            else
            {
                 fileName = toPath +  fileList[i].split("/")[fileList[i].split("/").length()  - 1] ;
            }
            qDebug() << fileName;
            QFile file(fileName);
            if(file.exists())
            {
                return false;
            }
        }

        return true;
}

void mainWindow::setAllShortCut()
{
    QString history;
    QString edit;
    int autoCopy;
    history = aDialog ->historyhotkeyEdit->text();
    edit = aDialog ->editKeyEdit ->text();
    autoCopy = aDialog ->setautobox ->isChecked();
     history = history.replace(QRegExp(" "),"");
    openAction ->setShortcut(history);
    qDebug() << openAction ->shortcut().toString();
    if(historyShort)
    {
         historyShort ->setShortcut(QKeySequence(history));
    }
    else
    {
        historyShort = new QxtGlobalShortcut(QKeySequence(history), this);
        connect(historyShort, SIGNAL(activated()),this, SLOT(history_shortcut_event()));
    }
    edit = edit.replace(QRegExp(" "),"");
    editInfoAction ->setShortcut(edit);
    if(editShort)
    {
        editShort ->setShortcut(QKeySequence(edit));
    }
    else
    {
        editShort = new QxtGlobalShortcut(QKeySequence(edit), this);
        connect(editShort, SIGNAL(activated()),this, SLOT(edit_shortcut_event()));
    }

    if(autoCopy == 0)
    {
        disconnect(QApplication::clipboard(),SIGNAL(selectionChanged()),this,0);
    }
    else
    {
        connect(QApplication::clipboard(),SIGNAL(selectionChanged()),this,SLOT(select_board_changed()));
    }
}

/*-------------------------------------------------SLOT-------------------------------------------------*/


void mainWindow::cut_board_changed()
{
     QString str = QApplication::clipboard()->text();
     QApplication::clipboard()->mimeData();
    QString fileType;
    font.setPointSize(aDialog ->fontSizeBox ->currentText().toInt());
    QString content = "\n";
    if(!str.isEmpty())
    {
        QStringList text = str.split("\n");
        //QFile cFile(text[0]);
        // if(cFile.exists())
        if(judgeIsFile())
        {
            getFileContent(content,fileType,text);
        }
        else
        {
            content = str;
            fileType = "文本";
        }

        if(aDialog ->viewBox ->isChecked())
        {
            cutBoard -> setPlainText(content);
            cutBoard ->setFont(font);
            clearInfoAction ->setEnabled(true);
        }
        if(aDialog ->cutselectbox ->isChecked())
        {
            writeFile(str,fileType,content);
        }
    }
    else
    {
        cutBoard -> setPlainText(content);
        cutBoard ->setFont(font);
        clearInfoAction ->setEnabled(false);
    }
}


void mainWindow::select_board_changed()
{
    QString str = QApplication::clipboard()->text(QClipboard::Selection);
    QApplication::clipboard()->setText(str);
}



void mainWindow::openAttrDialog()
{
    aDialog ->exec();
    setAllShortCut();
    font.setPointSize(aDialog ->fontSizeBox ->currentText().toInt());
    if(aDialog ->setboldbox ->isChecked())
    {
        font.setBold(true);
    }
    else
    {
        font.setBold(false);
    }

     if(!aDialog ->viewBox ->isChecked())
    {
        cutBoard ->clear();
        clearInfoAction ->setEnabled(false);
    }
    else
    {
         QString str = QApplication::clipboard() ->text();
         QString fileType;
         QString content = "\n";
         QStringList text = str.split("\n");
          if(judgeIsFile())
          {
              getFileContent(content,fileType,text);
         }
         else
         {
             content = str;
             fileType = "文本";
         }
        cutBoard ->setPlainText(content);
        cutBoard ->setFont(font);
        if(str.isEmpty())
        {
            clearInfoAction ->setEnabled(false);
        }
        else
        {
            clearInfoAction ->setEnabled(true);
        }
    }


}


void mainWindow::openCutDialog()
{
    cOpenNum++;
    if(cOpenNum == 1)
    {
        cDialog = new cutDialog(this);
        //cDialog ->show();
        cDialog ->exec();
        delete cDialog;
        cDialog = NULL;
        cOpenNum = 0;
    }
}

void mainWindow::openEditDialog()
{
    if(!judgeIsFile())
    {
        eOpenNum ++;
        if(eOpenNum == 1)
        {
            eDialog = new editDialog(this);
            eDialog ->exec();
            delete eDialog;
            eDialog = NULL;
            eOpenNum = 0;
        }
    }
    else
    {
        QMessageBox::warning(this,"提示            ","文件禁止编辑!");
    }
}

void mainWindow::save()
{
    QString content;
    QString str = cutBoard -> toPlainText();
    if(str.isEmpty())
    {
        content = str;
    }
    else
    {
        content = QApplication::clipboard()->text();
    }
    QString titleValue="剪切板输出";
    titleValue+="          ";
    fileInput ->setWindowTitle(titleValue);
    fileInput ->setAcceptMode(QFileDialog::AcceptSave);
    fileInput ->setViewMode(QFileDialog::Detail);
    fileInput ->setDirectory("/");

    foreach (QObject * obj, fileInput ->children())
    {
        if(obj ->objectName() == "toParentButton")
        {
            QToolButton * parentButton = qobject_cast<QToolButton *>(obj);
            parentButton ->setToolTip("上一级目录");
        }
    }

    if(!str.isEmpty() && judgeIsFile())
    {        
        fileInput ->setFileMode(QFileDialog::Directory);
        fileInput ->setFilter("所有文件(*)");

        if( fileInput ->exec() == QFileDialog::Accepted)
        {
            if(!fileIsExit(content,fileInput ->selectedFiles()[0]))
            {
                   int ret = QMessageBox::warning(this, "输出警告            ",
                                                           "<p>目标路径存在同名文件，是否继续执行输出操作 ?</p>"
                                                             "<p><font color=red><strong>警告: </strong>继续执行会覆盖同名文件 !</font></p>",
                                                           QMessageBox::Yes | QMessageBox::No);

                   if(ret ==  QMessageBox::No)
                   {
                       return;
                   }
            }

                qint64 size =  getBoardSize(content) / (1024 * 1024 * 10) ;
                progressDlg ->setRange(0,size);
                progressDlg->setLabelText("正在向" + fileInput ->selectedFiles()[0] + " 路径下输出...... " );
                shortCut = 0;
                thread ->setInformation(fileInput ->selectedFiles(),content);
                progressTimer ->start(0);
                thread ->start();
        }
    }
     else
    {
        QStringList format;
        format.append("所有txt文档文件(*.txt)");
        format.append("所有wps文档文件(*.wps)");
        format.append("所有文件(*)");
        fileInput ->setFilters(format);
        fileInput ->setFileMode(QFileDialog::AnyFile);
        if( fileInput ->exec() == QFileDialog::Accepted)
        {
            QStringList filename  = fileInput ->selectedFiles();
            if(!copyText(filename[0],QApplication::clipboard()->text()))
            {
                 QMessageBox::information(this,"剪切板输出提示            ","系统权限拒绝，信息输出失败！");
            }
            else
            {
                QMessageBox::information(this,"剪切板输出提示            ","信息输出成功！");
            }
         }
    }
}


void mainWindow::del()
{
    int ret = QMessageBox::question(this, "清除剪切板内容            ",
                                   "确定要清除剪切板内容？",
                                   QMessageBox::Ok | QMessageBox::Cancel);
    if(QMessageBox::Ok == ret)
    {
         QApplication::clipboard() ->clear();
    }
}


void mainWindow::openlibreoffice()
{
    QFile file("/usr/bin/gimp");
    if(file.exists())
    {
        QProcess*proc = new QProcess();
         proc->start("/usr/bin/gimp");
    }
    else
    {
        QMessageBox::warning(this,"打开工具提示           ","未发现此软件");
    }
}


void mainWindow::openwps()
{
    QFile file("/opt/kingsoft/wps-office/office6/wps");
    if(file.exists())
    {
        QProcess*proc = new QProcess();
        proc->start("/opt/kingsoft/wps-office/office6/wps");
    }
    else
    {
        QMessageBox::warning(this,"打开工具提示           ","未发现此软件");
    }
}

void mainWindow::openwpp()
{
    QFile file("/opt/kingsoft/wps-office/office6/wpp");
    if(file.exists())
    {
        QProcess*proc = new QProcess();
        proc->start("/opt/kingsoft/wps-office/office6/wpp");
    }
    else
    {
        QMessageBox::warning(this,"打开工具提示            ","未发现此软件");
    }
}

void mainWindow::openet()
{
    QFile file("/opt/kingsoft/wps-office/office6/et");
    if(file.exists())
    {
        QProcess*proc = new QProcess();
        proc->start("/opt/kingsoft/wps-office/office6/et");
    }
    else
    {
        QMessageBox::warning(this,"打开工具提示           ","未发现此软件");
    }
}

void mainWindow::opengedit()
{
    QFile file("/usr/bin/gedit");
    if(file.exists())
    {
        QProcess*proc = new QProcess();
        proc->start("gksu /usr/bin/gedit");
    }
    else
    {
        QMessageBox::warning(this,"打开工具            ","未发现此软件");
    }


}

void mainWindow::intructions_clicked_data()
{
        help ->hide();
        help ->showNormal();
}

void mainWindow::showWindowNormal()
{
    if(this->isVisible())
        {
          if(this->isMinimized())
          {
            this->setWindowFlags(Qt::WindowStaysOnTopHint);
            this->setWindowState(Qt::WindowActive);
            this->setGeometry(this->geometry());
            this->show();
          }
          else
            this->hide();
        }
         else
        {
          this->setWindowFlags(Qt::WindowStaysOnTopHint);
            this->setWindowState(Qt::WindowActive);
            this->setGeometry(this->geometry());
            this->show();
        }
}


 void mainWindow::iconAivated(QSystemTrayIcon::ActivationReason reason)
 {


     switch(reason)
     {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::MiddleClick:
         showWindowNormal();
         break;
        case QSystemTrayIcon::Context:
        {
                    if(progressDlg ->isVisible())
                    {
                        trayMenu ->close();
                    }
                    qDebug() << "Context";
     }
         break;
        case QSystemTrayIcon::Unknown:
         qDebug() << "Unknown";
         break;
        default:
         qDebug() << "error";
         break;
     }
 }


void mainWindow::history_shortcut_event()
{
    if(!fileInput->isVisible()&&!aDialog->isVisible()){
    cOpenNum++;
    if(cOpenNum == 1)
    {
        cDialog = new cutDialog(this);
        cDialog ->exec();
        if(this ->isVisible())
        {
             this ->show();
        }
        else
        {
            this ->show();
            this ->hide();
        }
        delete cDialog;
        cDialog = NULL;
        cOpenNum = 0;
    }
       }
 }


void mainWindow::edit_shortcut_event()
{
    if(!judgeIsFile())
    {
        if(!fileInput->isVisible()&&!aDialog->isVisible()){
        eOpenNum ++;
        if(eOpenNum == 1)
        {
            eDialog = new editDialog(this);
            eDialog ->exec();
            if(this ->isVisible())
            {
                this ->show();
            }
            else
            {
                this ->show();
                this ->hide();
            }
            delete eDialog;
            eDialog = NULL;
            eOpenNum = 0;
        }
        }
    }
    else
    {
        trayIcon ->showMessage("编辑警告            ","文件禁止编辑！",QSystemTrayIcon::Warning);
    }
}

void mainWindow::clear_history_event()
{
    QString clearSql = "delete from info";
    QSqlQuery query;
    int ret=QMessageBox::question(this, "清除历史记录            ",
                         "确定要清除历史记录？",
                         QMessageBox::Ok | QMessageBox::Cancel);

    if(QMessageBox::Ok == ret)
    {
         query.exec(clearSql);
    }
}


void mainWindow::finishTimeProgress()
{
    progressDlg ->setValue(dnum);
    if(thread ->getCopyFlag())
    {
         progressTimer ->stop();
         dnum =0;
         shortCut = 1;
         threadFlag = 0;
         progressDlg ->close();
         if(thread ->getInputFlag() == 1)
         {
            QMessageBox::information(this,"剪切板输出提示            ","文件输出成功！");
         }
         else if(thread ->getInputFlag() == -1)
         {
              QMessageBox::information(this,"剪切板输出提示            ","非法操作,文件输出失败!");
         }
         else if(thread ->getInputFlag() == -2)
         {
             QMessageBox::information(this,"剪切板输出提示            ","源文件不存在,文件输出失败!");
         }

    }
}

void mainWindow::progressDlgcancel()
{
             shortCut = 1;
            if(threadFlag)
            {
                progressTimer ->stop();
                thread ->overThreadStop();
                QMessageBox::information(this,"剪切板输出提示            ","输出中断！");
            }
            else
            {
                threadFlag = 1;
            }

            dnum = 0;
}



 /*-------------------------------------------------PROTECT-------------------------------------------------*/

 void mainWindow::changeEvent(QEvent *e)
 {
     if((e ->type() == QEvent::WindowStateChange) && this ->isMinimized())
    {
         if (this ->isActiveWindow() && !(cDialog !=NULL || aDialog ->isVisible() || eDialog != NULL))
         {
                QTimer::singleShot(100,this,SLOT(hide()));
         }
    }
}


void mainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::question(this, "退出剪切板            ",
                                   "是否要退出剪切板？",
                                   QMessageBox::Ok | QMessageBox::Cancel);
    if(QMessageBox::Ok == ret)
    {
        if(aDialog ->setrebootbox ->isChecked())
        {
            aDialog ->recoverySetting();
        }

        event ->accept();
    }
    else
    {
       event ->ignore();
       if(!this ->isVisible())
       {
           this ->show();
           this ->hide();
       }
    }
}

bool mainWindow::eventFilter(QObject *object, QEvent *event)
{
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);


    if(event ->type() == QEvent::KeyPress)
    {
        if(keyEvent ->key() == Qt::Key_Alt)
        {

                if(fileMenu ->isActiveWindow())
                {
                     fileMenu ->close();
                }

                if(editMenu ->isActiveWindow())
                {
                     editMenu ->close();
                }

                if(toolsMenu ->isActiveWindow())
                {
                     toolsMenu ->close();
                }

                if(helpMenu ->isActiveWindow())
                {
                     helpMenu ->close();
                }

                if(wpsMenu ->isActiveWindow())
                {
                     wpsMenu ->close();
                }
        }
        else if(keyEvent ->key() == Qt::Key_F && (keyEvent ->modifiers() & Qt::AltModifier))
        {

        }
        else if(keyEvent ->key() == Qt::Key_R && (keyEvent ->modifiers() & Qt::ControlModifier) && shortCut == 1)
        {
            clear_history_event();
        }
        else if(keyEvent ->key() == Qt::Key_S && (keyEvent ->modifiers() & Qt::ControlModifier) && shortCut == 1)
        {
            save();
        }
        else if(keyEvent ->key() == Qt::Key_A && (keyEvent ->modifiers() & Qt::ControlModifier) && shortCut == 1)
        {
            openAttrDialog();
        }
        else if(keyEvent ->key() == Qt::Key_Q && (keyEvent ->modifiers() & Qt::ControlModifier)&& shortCut == 1)
        {
            close();
        }
        else if(keyEvent ->key() == Qt::Key_Delete && shortCut == 1)
        {
            del();
        }
        else
        {
            return false;

        }
        return true;
  }

  else
        return false;
    }



