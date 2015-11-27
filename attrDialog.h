#ifndef ATTRDIALOG_H
#define ATTRDIALOG_H
#include <QtGui>
#include <QCheckBox>
#include <QtSql>

class attrDialog : public QDialog
{
    Q_OBJECT
public:
    QTextCodec *code;
    QLabel *cutLabel;
    QLabel *setLabel;
    QLabel *hotkeyLabel;
    QLabel *historyhotkeyLabel;
    QLabel *editKeyLabel;
    QLabel *fontSizeLabel;

    QComboBox *fontSizeBox;

    QCheckBox *cutselectbox;
    //QCheckBox *cutcopybox;
    QCheckBox *setboldbox;
    QCheckBox *setrebootbox;
    QCheckBox *setautobox;
    QCheckBox *setdesktopstratbox;
    QCheckBox *viewBox;

    QPushButton * okButton;
    QPushButton * cancelButton;

    QLineEdit *historyhotkeyEdit;
    QLineEdit *editKeyEdit;

    explicit attrDialog(QWidget *parent = 0);
    int exec();
    void recoverySetting();
    bool copyFileToPath();
private:
    void readingSettings();
    void writeSettings();
private slots:

    void ok_button_event();
    //void cancel_button_event();
protected:
        void closeEvent(QCloseEvent *);
        void keyPressEvent(QKeyEvent *);
};
#endif // ATTRDIALOG_H
