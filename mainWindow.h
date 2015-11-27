#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <QtSql>
#include <QtGui/qkbd_qws.h>
#include "attrDialog.h"
#include "cutDialog.h"
#include "editDialog.h"
#include "helpDialog.h"
#include "thread.h"
#include "qxtglobalshortcut.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTextCodec * code;
    QTextEdit * cutBoard;
    cutDialog *cDialog;
    editDialog *eDialog;
    attrDialog *aDialog;
    helpDialog * help;

    QToolBar *fileToolBar;
    QToolBar *cutToolBar;
    QToolBar *attributeBar;
    QToolBar *intructionsBar;

    QFont font;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QMenu *wpsMenu;

    QAction *openAction;
    QAction *saveAction;
    QAction *aboutAction;
    QAction *removeFileAction;
    QAction *attributeAction;
    QAction *intructionsAction;
    QAction *editInfoAction;
    QAction *clearInfoAction;
    QAction *cutPictureAction;
    QAction *wps1Action;
    QAction *wps2Action;
    QAction *wps3Action;
    QAction *geditAction;
    QAction *exitAction;
    QAction *trayAboutAction;
    QAction *miniSizeAction;
    QAction *maxSizeAction;
    QAction *quitAction;
    QAction *openBoardAction;

    QMenu *trayMenu;
    QSystemTrayIcon *trayIcon;

    QxtGlobalShortcut * historyShort;
    QxtGlobalShortcut * editShort;

    QFileDialog * fileInput;


    QTimer *progressTimer;
    QProgressDialog *progressDlg;
    Thread  *thread;

    int cOpenNum;
    int eOpenNum;

    void initWindow();
    void initCutBoard();
    void getFileContent(QString &content,QString &fileType,QStringList text);
    void createActions();
    void createMenus();
    void createToolBars();
    void createTray();
    void setAllShortCut();
    bool writeFile(QString content,const QString fileType,const QString &file);
    bool copyText(const QString &fileName,const QString &text);
    bool judgeIsFile();
    QString getFIleSize(qint64 size);
    qint64 getFileNum(const QString &path);
    qint64 getFolderSize(const QString &path);
    /*cut board must be file or folder*/
    qint64 getBoardSize(QString content);
    bool fileIsExit(QString content,QString toPath);
public:
    mainWindow();
    static int dnum;
    ~mainWindow();
private slots:
      void cut_board_changed();
      void select_board_changed();
      void openCutDialog();
      void openEditDialog();
      void openAttrDialog();
      void save();
      void del();
      void openlibreoffice();
      void openwps();
      void openwpp();
      void openet();
      void opengedit();
      void intructions_clicked_data();
      void iconAivated(QSystemTrayIcon::ActivationReason reason);
      void history_shortcut_event();
      void edit_shortcut_event();
      void clear_history_event();
      void softwareInfo();
      void finishTimeProgress();
      void progressDlgcancel();
      void showWindowNormal();

protected:
      void changeEvent(QEvent *e);
      void closeEvent(QCloseEvent *);
      bool eventFilter(QObject *, QEvent *);

};

#endif // MAINWINDOW_H
