#ifndef SETITEMDIALOG_H
#define SETITEMDIALOG_H

#include <QDialog>

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

class SetItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetItemDialog(QWidget *parent = nullptr);
    void updateNameOptions();
    void onNameIndexChanged(int index);

private:
    QComboBox *nameComboBox;
    QSpinBox *valueSpinBox;
    QSpinBox *minSpinBox;
    QSpinBox *maxSpinBox;
    QSpinBox *variationSpinBox;

private slots:
    void onOkClicked();
};

#endif // SETITEMDIALOG_H
