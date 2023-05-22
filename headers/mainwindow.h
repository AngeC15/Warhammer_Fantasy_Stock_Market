#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QtWidgets/QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTableWidget *tableWidget;
    void refreshTableView();
    //void addItemToList(QString name, int value, int min, int max);
    QString pattern;

    QPushButton *saveButton;
    QPushButton *loadButton;
    QPushButton *stepButton;
    QPushButton *setButton;
    QPushButton *addButton;
    QLineEdit *lineEdit;


public slots:
    void saveClicked();
    void loadClicked();
    void stepClicked();
    void setClicked();
    void addClicked();
    void lineEditReturnPressed();
//
    void addItemToList(QString& name, int value, int min, int max, int variation);
    void deleteButtonClicked(int row);
    void handleButtonClicked();
    void handleGraphButtonClicked();



private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
