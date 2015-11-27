#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QtGui>
#include <QWidget>
class helpDialog:public QDialog
{
 Q_OBJECT
public:
    helpDialog(const QString &path,const QString &page,QWidget *parent=0);

    static void showPage(const QString &page);
    void mouseMoveEvent(QMouseEvent *e);
private slots:
    void updateWindowTitle();
    void forwardSection();
    void backwardSection();
private:
    QTextCodec *code;
    QTextBrowser *textBrowser;

    QToolButton *homeButton;
    QToolButton *upSection;
    QToolButton *downSection;
    QToolButton *closeButton;
protected:
    void closeEvent(QCloseEvent *);

};
#endif // HELPDIALOG_H
