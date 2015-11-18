#include <QtGui>
#include <QtSql>
#include <QApplication>
#include <QtNetwork>
#include "qtsingleapplication/qtsingleapplication.h"
#include "mainWindow.h"

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/usr/lib/fangdeQC/data.db");
    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
        return false;
    }
    return true;
}
void createFakeData()
{
    QSqlQuery query;
    query.exec("Create table info ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "time VARCHAR NOT NULL, "
               "type VARCHAR NOT NULL, "
               "content TEXT , "
               "file TEXT,"
               "create_time TIMESTAMP NOT NULL DEFAULT current_timestamp)");
}

int main(int argc,char **argv)
{
    QTextCodec* qc= QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForLocale(qc);
    QTextCodec::setCodecForCStrings(qc);
    QTextCodec::setCodecForTr(qc);
    if(!QFile::exists("/usr/lib/fangdeQC/data.db"))
    {
        createConnection();
        createFakeData();
    }
    else
    {
        createConnection();
    }
    QTranslator translator;
    translator.load(":/qt_zh_CN");
    QtSingleApplication  app(argc,argv);
    app.installTranslator(&translator);
    if (app.isRunning())
      {
          QString titleValue="软件启动信息";
          titleValue+="        ";
          QMessageBox message(QMessageBox::NoIcon, titleValue,
                              " <br> "
                              "<font size='4' color='#5C3317'>剪切板管理工具已运行！</font>"
                              "<br>"
                               );
          message.setIconPixmap(QPixmap(":/images/board.png"));
          message.setWindowIcon(QIcon(":/images/icon.png"));
          message.setWindowFlags(Qt::Tool);
          message.exec();
         return 0;
      }
    mainWindow *w = new mainWindow;
    app.installEventFilter(w);
    w ->show();
    return app.exec();
}
