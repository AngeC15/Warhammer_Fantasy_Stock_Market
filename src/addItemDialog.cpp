// AddItemDialog.cpp
#include "headers/AddItemDialog.h"

#include <QDebug> // Include the necessary header for qDebug()
#include <iostream>

#include <List>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidgetItem>
#include <QtGui/QStandardItemModel>
#include <QString>
#include <QTableWidget>
#include <QLabel>

AddItemDialog::AddItemDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Add Item");

    nameLineEdit = new QLineEdit(this);
    valueLineEdit = new QLineEdit(this);
    minLineEdit = new QLineEdit(this);
    maxLineEdit = new QLineEdit(this);
    varLineEdit = new QLineEdit(this);
    confirmButton = new QPushButton("Confirm", this);
    cancelButton = new QPushButton("Cancel", this);

    connect(confirmButton, &QPushButton::clicked, this, &AddItemDialog::confirmClicked);
    connect(cancelButton, &QPushButton::clicked, this, &AddItemDialog::cancelClicked);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Name:", this));
    layout->addWidget(nameLineEdit);
    layout->addWidget(new QLabel("Value:", this));
    layout->addWidget(valueLineEdit);
    layout->addWidget(new QLabel("Min:", this));
    layout->addWidget(minLineEdit);
    layout->addWidget(new QLabel("Max:", this));
    layout->addWidget(maxLineEdit);
    layout->addWidget(new QLabel("Variation en %:", this));
    layout->addWidget(varLineEdit);
    layout->addWidget(confirmButton);
    layout->addWidget(cancelButton);
}

void AddItemDialog::confirmClicked()
{
    QString name = nameLineEdit->text();
    int value = valueLineEdit->text().toInt();
    int min = minLineEdit->text().toInt();
    int max = maxLineEdit->text().toInt();
    int variation = varLineEdit->text().toInt();

    emit addItem(name, value, min, max, variation);
    close();
}

void AddItemDialog::cancelClicked()
{
    close();
}
