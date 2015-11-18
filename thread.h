#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QtGui>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();

//    void stop();
    void setInformation(QStringList tDirecory,QString content);
    bool getCopyFlag();
    int getInputFlag();
    void overThreadStop();
    QString changeFileName(QString name);
protected:
    void run();
private:
    QStringList toDirecory;
    QString content;
    bool copyDirectoryFiles(const QString &fromDir,const QString &toDir);
    bool copyFile(QString sPath,QString tPath);
    int copy(QString sPath,QString tPath);
    volatile bool stopped;
    int inputFlag;
    bool stop;
};


#endif // THREAD_H
