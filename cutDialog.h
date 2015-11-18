#ifndef CUTDIAOLOG_H
#define CUTDIAOLOG_H
#include <QtGui>
#include <QtSql>
class cutDialog : public QDialog
{
    Q_OBJECT
    QTextCodec * code;
    QTextEdit * allEdit;
    QTextEdit * cutEdit;

    QDateEdit *dateEdit;

    QListWidget * listWidget;
    QPushButton * deleteButton;
    QPushButton *clearButton;
    QPushButton *editButton;
    QPushButton *quitButton;
    QPushButton * findButton;
    QPushButton * viewAllButton;
    QPushButton * orderButton;
    QRadioButton * selectRadioButton;
    QAbstractItemView *abstractItemView;
    int state = 0;
    QDialog * editDialog;
public:
    cutDialog(QWidget *parent = 0);
    int exec();
private:
    void initHistoryDialog();
    bool readFile();
    void viewListWidget();
    void deleteOneData(unsigned int id);
    void updateCurrentRow(QString text);
private slots:
    void delete_current_row();
    void find_by_date();
    void get_current_info();
    void clear_history_record();
    void edit_current_row();
    void ok_edit_button();
    void cancel_edit_button();
    void view_all_data();
    void order_by_time();
protected:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *);

};

#endif // CUTDIAOLOG_H
