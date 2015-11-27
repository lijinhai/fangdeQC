#include "cutDialog.h"

static int cliked = 1;

cutDialog::cutDialog(QWidget *parent) : QDialog(parent)
{
        initHistoryDialog();
        viewListWidget();
        this ->setTabOrder(viewAllButton,findButton);
        this ->setTabOrder(findButton,dateEdit);
        this ->setTabOrder(dateEdit,selectRadioButton);
        this ->setTabOrder(selectRadioButton,orderButton);
         this ->setTabOrder(orderButton,listWidget);
        this ->setTabOrder(listWidget,deleteButton);
         this ->setTabOrder(deleteButton,clearButton);
        this ->setTabOrder(clearButton,editButton);
         this ->setTabOrder(editButton,quitButton);
      //  QWidget::setTabOrder();

}
int cutDialog::exec()
{
    QDialog::exec();
    return 1;
}


bool cutDialog::readFile()
{
    QString str;
    QSqlQuery query;
    query.exec("select id,time,content from info where id >= 1");
    while(query.next())
    {
        int ele0 = query.value(0).toInt();
        QString ele1 = "日期：" + query.value(1).toString() + "\n";
        str += ele1;
        QString ele2 = "内容：" + query.value(2).toString() + "\n";
        str += ele2;
    }
     allEdit ->setPlainText(str);
     allEdit ->setReadOnly(true);
}

void cutDialog::find_by_date()
{
        state = 1;
        cliked = 1;
        orderButton->setIcon(QIcon(":/images/desc.png"));
        orderButton->setToolTip("升序排列");
        orderButton->setFixedSize(21,21);
        orderButton->setIconSize(QSize(25,25));
        disconnect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,0);
        listWidget ->clear();
        allEdit ->clear();
        connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));
        int row = 0;
        QString date = dateEdit ->text();
        QString selectSql = "select * from info where create_time > datetime('" + date + "','00:00:00')"
                + " and create_time < datetime('" + date + "','12:00:00')order by time desc";
        QSqlQuery selectQuery;
        selectQuery.exec(selectSql);
        while(selectQuery.next())
        {
            QString ele1 = "<-----------------------日期：" + selectQuery.value(1).toString() + "------------------->"+"\n";
            QString ele2 = selectQuery.value(3).toString();
            ele2 = ele2.replace(QRegExp("\n"),"");
            if(ele2.length() >= 50)
            {
                ele2 = ele2.left(20) + "..." + ele2.right(20);
            }
            else
            {
                ele2 = ele2;
            }

            listWidget ->addItem(ele1+ele2);
            QListWidgetItem * item = listWidget ->item(row);
            row++;
            item ->setData(Qt::UserRole,selectQuery.value(0).toInt());
        }
        if(row == 0)
        {
            QMessageBox message(QMessageBox::Information, "查找             ",
                                "查找失败 !"
                                ,QMessageBox::Ok ,this,Qt::Tool);
            this ->activateWindow();
            message.exec();
        }

}

/****************************************PRIVATE SLOT****************************************/

void cutDialog::edit_current_row()
{

    QItemSelectionModel *selectionModel = listWidget->selectionModel();
    QModelIndexList selected = selectionModel->selectedIndexes();
    int row = listWidget ->currentRow();
    QListWidgetItem * item = listWidget ->item(row);

    if(selected.length() != 1)
    {
        QMessageBox message(QMessageBox::Critical, "编辑错误        ",
                            "<p>请选择单条文本内容进行编辑 !</font></strong></p>"
                            ,QMessageBox::Ok ,this,Qt::Tool);
        this ->activateWindow();
        message.exec();
        return;
    }

    if(row >= 0 && item ->isSelected())
    {
        editDialog = new QDialog(this);
        cutEdit = new QTextEdit(editDialog);
        QPushButton *okButton = new QPushButton("确定",editDialog);
        connect(okButton,SIGNAL(clicked()),this,SLOT(ok_edit_button()));
        QPushButton *cancelButton = new QPushButton("取消",editDialog);
        connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancel_edit_button()));

        QListWidgetItem * item = listWidget ->item(row);
        int key = item->data(Qt::UserRole).toInt();
        QString sqlStr = "select * from info where id = " + QString::number(key);
        QSqlQuery query;
        query.exec(sqlStr);
        while(query.next())
        {
            if(query.value(2).toString() == "文本")
            {
                   cutEdit ->setPlainText(query.value(3).toString());
             }
             else
             {
                   QMessageBox message(QMessageBox::Warning, "编辑提示        ",
                                       "文件不能编辑 !"
                                       ,QMessageBox::Ok ,this,Qt::Tool);
                   this ->activateWindow();
                   message.exec();
                   return;
             }
        }

        QHBoxLayout *hboxLayout = new QHBoxLayout;
        hboxLayout ->addWidget(okButton);
        hboxLayout ->addStretch();
        hboxLayout ->addWidget(cancelButton);

        QVBoxLayout *vboxLayout = new QVBoxLayout;
        vboxLayout ->addWidget(cutEdit);
        vboxLayout ->addLayout(hboxLayout);
        editDialog ->setLayout(vboxLayout);
        editDialog ->setWindowTitle("编辑");
        //editDialog ->resize(400,500);
        editDialog ->setFixedSize(400,500);
        editDialog ->exec();

    }
    else
    {
        QMessageBox message(QMessageBox::Critical, "编辑错误        ",
                            "<p>请选择编辑的内容 !</font></strong></p>"
                            ,QMessageBox::Ok ,this,Qt::Tool);
        this ->activateWindow();
        message.exec();
    }
}


void cutDialog::ok_edit_button()
{
    int row = listWidget ->currentRow();

    QListWidgetItem * item = listWidget ->item(row);
    int key = item->data(Qt::UserRole).toInt();
    QString newContent = cutEdit ->toPlainText();
    newContent = newContent.replace(QRegExp("'"),"''");
    QString updateSql = "update info set content = '" + newContent + "' where id = " +
            QString::number(key);
    QSqlQuery query;
    query.exec(updateSql);

    QString selectSql = "select * from info where id = " + QString::number(key);
    query.exec(selectSql);
    QString ele1;
    QString ele2;
    while (query.next())
    {
        allEdit ->clear();
        QString eleEdit1 = "日期：" + query.value(1).toString() + "\n";
        QString eleEdit2 = "类型：" + query.value(2).toString() + "\n";
        QString eleEdit3 = "内容：" + query.value(3).toString() + "\n";
        allEdit ->setPlainText(eleEdit1 + eleEdit2 + eleEdit3);
        ele1 = "<-----------------------日期：" + query.value(1).toString() + "------------------->"+"\n";
        ele2 = query.value(3).toString();
    }

    if(ele2.length() >= 50)
    {
        ele2 = ele2.left(20) + "..." + ele2.right(20);
    }
    else
    {
        ele2 = ele2;
    }

    updateCurrentRow(ele1+ele2);
    editDialog ->close();
    delete editDialog;
}

void cutDialog::cancel_edit_button()
{
    editDialog ->close();
    delete editDialog;
}


void cutDialog::clear_history_record()
{
     QMessageBox message(QMessageBox::Question, "清除历史记录        ",
                         "确定要清除历史记录 ?"
                         ,QMessageBox::Ok  | QMessageBox::Cancel,this,Qt::Tool);
      this ->activateWindow();
     if(QMessageBox::Ok ==  message.exec())
     {
         disconnect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,0);
         listWidget ->clear();
         allEdit ->clear();
         connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));
         QString clearSql = "delete from info";
         QSqlQuery query;
         query.exec(clearSql);
     }
     else
     {
         return;
     }
 }

void cutDialog::get_current_info()
{
//    int row = listWidget ->currentRow();
//    QListWidgetItem * item = listWidget ->item(row);
//    int key = item->data(Qt::UserRole).toInt();
//    QString sqlStr = "select * from info where id = " + QString::number(key);
//    QSqlQuery query;
//    query.exec(sqlStr);
//    while(query.next())
//    {
//        QString ele1 = "日期：" + query.value(1).toString() + "\n";

//        QString ele3 = "内容：";
//        QString ele4;
//        if(query.value(2).toString() == "文本")
//        {
//             QString ele2 = "类型：" + query.value(2).toString() + "\n";
//             ele3 += query.value(3).toString() + "\n";
//             allEdit ->setPlainText(ele1 + ele2 + ele3);
//        }
//        else
//        {
//            QString ele2 = "类型：" + query.value(2).toString();
//            ele4 += query.value(4).toString() + "\n";
//            allEdit ->setPlainText(ele1 + ele2 + ele4);
//        }
//    }

    int row = listWidget ->currentRow();
    QListWidgetItem * item = listWidget ->item(row);
    int key = item->data(Qt::UserRole).toInt();
    QString sqlStr = "select * from info where id = " + QString::number(key);
    QSqlQuery query;
    query.exec(sqlStr);
    while(query.next())
    {
      QString ele1 = "日期：" + query.value(1).toString() + "\n";
      QString ele2 = "类型：" + query.value(2).toString() + "\n";
      QString ele3 = "内容：";
      if(query.value(2).toString() == "文本")
        ele3 += query.value(3).toString() + "\n";
      else
      {
            ele3 += query.value(3).toString().replace(QRegExp("\n"),", ") + "\n\n"
                  " <--------------------------------文件属性-------------------------------->" +"\n"
                   + query.value(4).toString();
      }
      allEdit ->setPlainText(ele1 + ele2 + ele3);
    }
    }

void cutDialog::delete_current_row()
{
    QItemSelectionModel *selection = listWidget->selectionModel();
    QModelIndexList selected = selection->selectedIndexes();
    QMap<int,int> rowMap;
     foreach (QModelIndex index,selected )
     {
         rowMap.insert(index.row(),0);
     }
     int rowToDel;
     QMapIterator<int,int>rowMapIterator(rowMap);
     rowMapIterator.toBack();
     if(selected.count()!= 0)
     {
         QMessageBox message(QMessageBox::Question, "删除记录        ",
                             "确定要删除此条历史记录 ?"
                             ,QMessageBox::Ok  | QMessageBox::Cancel,this,Qt::Tool);
          this ->activateWindow();
         if(QMessageBox::Ok ==  message.exec())
     {
       while (rowMapIterator.hasPrevious())
       {
            rowMapIterator.previous();
            rowToDel = rowMapIterator.key();
            QListWidgetItem * item = listWidget ->item(rowToDel);
            int key = item->data(Qt::UserRole).toInt();
            deleteOneData(key);
            disconnect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,0);
            delete listWidget ->takeItem(rowToDel);
            connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));
            int row = listWidget ->currentRow();
            listWidget ->setItemSelected(listWidget ->item(row),false);
        }
     }

      allEdit ->clear();
     }
     else
     {
         QMessageBox message(QMessageBox::Information, "删除提示        ",
                             "请选择删除的内容 !"
                             ,QMessageBox::Ok ,this,Qt::Tool);
         this ->activateWindow();
         message.exec();
         return;
      }

 }

//wangxu
//    int row = listWidget ->currentRow();
//    int count = listWidget ->count();
//    QListWidgetItem * item = listWidget ->item(row);
//    if(row >= 0  && item ->isSelected())
//    {
//        int key = item->data(Qt::UserRole).toInt();
//        deleteOneData(key);
//        if(count == 1)
//        {
//            disconnect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,0);
//            delete listWidget ->takeItem(row);
//            connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));
//        }
//        else
//        {
//            delete listWidget ->takeItem(row);
//        }
//        allEdit ->clear();

//    }
//    else
//    {
//        QMessageBox::information(this,"删除","请选择删除的内容",QMessageBox::Ok);
//        return;
//    }

//    row = listWidget ->currentRow();
//    listWidget ->setItemSelected(listWidget ->item(row),false);




void cutDialog::view_all_data()
{
    state = 0;
    cliked = 1;
    orderButton->setIcon(QIcon(":/images/desc.png"));
    orderButton->setToolTip("升序排列");
    orderButton->setFixedSize(21,21);
    orderButton->setIconSize(QSize(25,25));
    disconnect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,0);
    listWidget ->clear();
    allEdit ->clear();
    viewListWidget();
    connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));
}


/****************************************PRIVATE****************************************/
void cutDialog::initHistoryDialog()
{
    code = QTextCodec::codecForLocale();
    cliked = 1;

    QString titleValue="历史记录";
    titleValue+="         ";
    this ->setWindowTitle(titleValue);
    this ->setWindowIcon(QIcon(":images/icon.png"));//:images/icon.png

    allEdit = new QTextEdit(this);
    allEdit ->setReadOnly(true);

    listWidget = new QListWidget(this);
    listWidget->setSelectionMode(abstractItemView->ExtendedSelection);
    connect(listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(get_current_info()));
    connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));

    selectRadioButton = new QRadioButton(code ->toUnicode("设置剪切板"));

    orderButton = new QPushButton();
    orderButton->setFixedSize(21,21);
    orderButton->setIcon(QIcon(":/images/desc.png"));
    orderButton->setIconSize(QSize(25,25));
    orderButton->setToolTip("升序排列");
    connect(orderButton,SIGNAL(clicked()),this,SLOT(order_by_time()));

    deleteButton = new QPushButton(code ->toUnicode("删除"));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(delete_current_row()));

    clearButton = new QPushButton(code ->toUnicode("清空"));
    connect(clearButton,SIGNAL(clicked()),this,SLOT(clear_history_record()));

    editButton = new QPushButton(code ->toUnicode("编辑"));
    connect(editButton,SIGNAL(clicked()),this,SLOT(edit_current_row()));

    quitButton = new QPushButton(code ->toUnicode("退出"));
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));

    QFont font;
    font.setBold(true);
    QLabel *temp = new QLabel("详细：",this);
    temp ->setFont(font);

    findButton = new QPushButton("查找");
    connect(findButton,SIGNAL(clicked()),this,SLOT(find_by_date()));

    viewAllButton = new QPushButton("显示所有");
    connect(viewAllButton,SIGNAL(clicked()),this,SLOT(view_all_data()));

    dateEdit = new QDateEdit(this);
    dateEdit ->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setSelectedSection(QDateTimeEdit::DaySection);
    dateEdit ->setDate(QDate::currentDate());

    /******************布局***********************/

    QHBoxLayout *htopLayout = new QHBoxLayout;
    htopLayout ->addWidget(viewAllButton);
    htopLayout ->addWidget(findButton);
    htopLayout ->addWidget(dateEdit);
    htopLayout ->addStretch();
    //htopLayout ->addSpacing(20);

    QHBoxLayout *hboxButtonLayout = new QHBoxLayout;
    hboxButtonLayout ->addWidget(deleteButton);
    hboxButtonLayout ->addWidget(clearButton);
   // hboxButtonLayout ->addStretch();
    hboxButtonLayout ->addWidget(editButton);

    QHBoxLayout *horderButtonLayout = new QHBoxLayout;
    horderButtonLayout ->addWidget(selectRadioButton);
    horderButtonLayout->addStretch();
    horderButtonLayout ->addWidget(orderButton);

    QVBoxLayout * vboxLayout = new QVBoxLayout;
    vboxLayout ->addLayout(htopLayout);
    vboxLayout ->addLayout(horderButtonLayout);
    //vboxLayout ->addWidget(selectRadioButton);
    //vboxLayout ->addWidget(calendarWidget);
    vboxLayout ->addWidget(listWidget);
    //vboxLayout ->addLayout(hboxLayout);
    vboxLayout ->addLayout(hboxButtonLayout);

    QHBoxLayout * hboxLayout1 = new QHBoxLayout;
    hboxLayout1 ->addSpacing(270);
    hboxLayout1 ->addWidget(quitButton);

    QVBoxLayout * vboxRightLayout = new QVBoxLayout;
    vboxRightLayout ->addSpacing(32);
    vboxRightLayout ->addWidget(temp);
    vboxRightLayout ->addSpacing(5);
    vboxRightLayout ->addWidget(allEdit);
    vboxRightLayout ->addLayout(hboxLayout1);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout ->addLayout(vboxLayout);
    hboxLayout ->addLayout(vboxRightLayout);

    setLayout(hboxLayout);
    //this ->resize(800,500);
    this ->setFixedSize(800,500);
}


void cutDialog::updateCurrentRow(QString text)
{
    int row = listWidget ->currentRow();
    QListWidgetItem * item = listWidget ->item(row);
    item ->setText(text);

}

void cutDialog::deleteOneData(unsigned int id)
{
    QString deleteSql = "delete from info where id = " + QString::number(id);
    QSqlQuery query;
    query.exec(deleteSql);
}

void cutDialog::viewListWidget()
{
    QSqlQuery query;
    query.exec("select * from info order by time desc");
    int row = 0;
    while(query.next())
    {

        QString ele1 = "<-----------------------日期：" + query.value(1).toString() + "------------------->"+"\n";
        QString ele2 = query.value(3).toString();
        ele2 = ele2.replace(QRegExp("\n"),"");
        if(ele2.length() >= 50)
        {
            ele2 = ele2.left(20) + "..." + ele2.right(20);
        }
        else
        {
            ele2 = ele2;
        }
        listWidget ->addItem(ele1+ele2);
        QListWidgetItem * item = listWidget ->item(row);
        row++;
        item ->setData(Qt::UserRole,query.value(0).toInt());
    }
}

void cutDialog ::order_by_time()
{
        QSqlQuery query;
        int row = 0;
        switch(cliked%2)
        {
            case 0:
                cliked ++;
                orderButton->setIcon(QIcon(":/images/desc.png"));
                orderButton->setToolTip("升序排列");
                orderButton->setIconSize(QSize(25,25));
                orderButton->setFixedSize(21,21);
                if(state == 0)
                {
                disconnect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,0);
                listWidget ->clear();
                allEdit ->clear();
                connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));
                query.exec("select * from info order by time desc");

                while(query.next())
                {

                    QString ele1 = "<-----------------------日期：" + query.value(1).toString() + "------------------->"+"\n";
                    QString ele2 = query.value(3).toString();
                    ele2 = ele2.replace(QRegExp("\n"),"");
                    if(ele2.length() >= 50)
                    {
                        ele2 = ele2.left(20) + "..." + ele2.right(20);
                    }
                    else
                    {
                        ele2 = ele2;
                    }
                    listWidget ->addItem(ele1+ele2);
                    QListWidgetItem * item = listWidget ->item(row);
                    row++;
                    item ->setData(Qt::UserRole,query.value(0).toInt());
                }
                }
                else
                {
                    disconnect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,0);
                    listWidget ->clear();
                    allEdit ->clear();
                    connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));
                    int row = 0;
                    QString date = dateEdit ->text();
                    QString selectSql = "select * from info where create_time > datetime('" + date + "','00:00:00')"
                            + " and create_time < datetime('" + date + "','12:00:00') order by time desc ";
                    QSqlQuery selectQuery;
                    selectQuery.exec(selectSql);
                    while(selectQuery.next())
                    {
                        QString ele1 = "<-----------------------日期：" + selectQuery.value(1).toString() + "------------------->"+"\n";
                        QString ele2 = selectQuery.value(3).toString();
                        ele2 = ele2.replace(QRegExp("\n"),"");
                        if(ele2.length() >= 50)
                        {
                            ele2 = ele2.left(20) + "..." + ele2.right(20);
                        }
                        else
                        {
                            ele2 = ele2;
                        }

                        listWidget ->addItem(ele1+ele2);
                        QListWidgetItem * item = listWidget ->item(row);
                        row++;
                        item ->setData(Qt::UserRole,selectQuery.value(0).toInt());
                    }

                }

                break;
            case 1:
                cliked ++;
                orderButton->setIcon(QIcon(":/images/asc.png"));
                orderButton->setToolTip("降序排列");
                orderButton->setFixedSize(21,21);
                orderButton->setIconSize(QSize(25,25));
                if(state == 0)
                {
                disconnect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,0);
                listWidget ->clear();
                allEdit ->clear();
                connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));
                query.exec("select * from info order by time asc");
                while(query.next())
                {

                    QString ele1 = "<-----------------------日期：" + query.value(1).toString() + "------------------->"+"\n";
                    QString ele2 = query.value(3).toString();
                    ele2 = ele2.replace(QRegExp("\n"),"");
                    if(ele2.length() >= 50)
                    {
                        ele2 = ele2.left(20) + "..." + ele2.right(20);
                    }
                    else
                    {
                        ele2 = ele2;
                    }
                    listWidget ->addItem(ele1+ele2);
                    QListWidgetItem * item = listWidget ->item(row);
                    row++;
                    item ->setData(Qt::UserRole,query.value(0).toInt());
                }
                }
                else
                {

                    disconnect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,0);
                    listWidget ->clear();
                    allEdit ->clear();
                    connect(listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(get_current_info()));
                    int row = 0;
                    QString date = dateEdit ->text();
                    QString selectSql = "select * from info where create_time > datetime('" + date + "','00:00:00')"
                            + " and create_time < datetime('" + date + "','12:00:00') order by time asc ";
                    QSqlQuery selectQuery;
                    selectQuery.exec(selectSql);
                    while(selectQuery.next())
                    {
                        QString ele1 = "<-----------------------日期：" + selectQuery.value(1).toString() + "------------------->"+"\n";
                        QString ele2 = selectQuery.value(3).toString();
                        ele2 = ele2.replace(QRegExp("\n"),"");
                        if(ele2.length() >= 50)
                        {
                            ele2 = ele2.left(20) + "..." + ele2.right(20);
                        }
                        else
                        {
                            ele2 = ele2;
                        }

                        listWidget ->addItem(ele1+ele2);
                        QListWidgetItem * item = listWidget ->item(row);
                        row++;
                        item ->setData(Qt::UserRole,selectQuery.value(0).toInt());
                    }
                }
        }
}

/****************************************PROTECT****************************************/

void cutDialog::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "dsdsd";
    return;
}

void cutDialog::closeEvent(QCloseEvent * event)
{
    QItemSelectionModel *selectionModel = listWidget->selectionModel();
    QModelIndexList selected = selectionModel->selectedIndexes();
    if(selectRadioButton ->isChecked())
    {
         if(selected.length() != 1)
        {
             QMessageBox message(QMessageBox::Critical, "设置剪切板错误        ",
                                 "<p>设置剪切板失败,请选择单条内容设置剪切板 !</font></strong></p>"
                                 ,QMessageBox::Ok ,this,Qt::Tool);
            this ->activateWindow();
            message.exec();
            event ->ignore();
            return;
        }
        int row = listWidget ->currentRow();
        if(row >=0 )
        {
            QListWidgetItem * item = listWidget ->item(row);
            int key = item->data(Qt::UserRole).toInt();
            QString sqlStr = "select * from info where id = " + QString::number(key);
            QSqlQuery query;
            query.exec(sqlStr);
            while(query.next())
            {
                QString type = query.value(2).toString();
                QString content = query.value(3).toString();
                if(type == "文本")
                {
                        QApplication::clipboard()->setText(content);
                }
                 else
                {
                    QStringList text = content.split("\n");
                    QList<QUrl>url;
                    QMimeData *data = new QMimeData;
                    for(int i =  text.length() - 1;i >= 0;i --)
                    {
                        QFile file(text[i]);
                        if(file.exists())
                        {
                                url.append(QUrl("file://" + text[i]));
                        }
                        else
                        {
                            QMessageBox message(QMessageBox::Critical, "设置剪切板错误        ",
                                                "<p>源文件不存在,设置剪切板失败 !</font></strong></p>"
                                                ,QMessageBox::Ok ,this,Qt::Tool);
                            this ->activateWindow();
                            message.exec();
                            event ->ignore();
                            return;
                        }

                    }
                    data ->setText(content);
                    data ->setUrls(url);
                    QApplication::clipboard() ->setMimeData(data);
                    //qDebug() << QApplication::clipboard() ->mimeData() ->urls();
                }
            }
        }
    }
}
