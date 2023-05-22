#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(QWidget *parent = nullptr);

private:
    QLineEdit *nameLineEdit;
    QLineEdit *valueLineEdit;
    QLineEdit *minLineEdit;
    QLineEdit *maxLineEdit;
    QLineEdit *varLineEdit;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

private slots:
    void confirmClicked();
    void cancelClicked();

signals:
    void addItem(QString& name, int value, int min, int max, int variation);
};

#endif // ADDITEMDIALOG_H
