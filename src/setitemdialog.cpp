#include "headers/setItemdialog.h"
#include "headers/item.h"

#include <QComboBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QPushButton>
#include <iostream>

SetItemDialog::SetItemDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Set Item");

    nameComboBox = new QComboBox(this);
    valueSpinBox = new QSpinBox(this);
    minSpinBox = new QSpinBox(this);
    maxSpinBox = new QSpinBox(this);
    variationSpinBox = new QSpinBox(this);

    valueSpinBox->setMaximum(100000); // Set the desired maximum value
    minSpinBox->setMaximum(100000);   // Set the desired maximum value
    maxSpinBox->setMaximum(100000);   // Set the desired maximum value
    variationSpinBox->setMaximum(200);
    variationSpinBox->setMinimum(0);


    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("Name:", nameComboBox);
    formLayout->addRow("Value:", valueSpinBox);
    formLayout->addRow("Min:", minSpinBox);
    formLayout->addRow("Max:", maxSpinBox);
    formLayout->addRow("% Variation:", variationSpinBox);

    QPushButton *okButton = new QPushButton("OK", this);
    formLayout->addWidget(okButton);

    connect(okButton, &QPushButton::clicked, this, &SetItemDialog::onOkClicked);
    connect(nameComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SetItemDialog::onNameIndexChanged);
    // Update the name options
    updateNameOptions();
}

void SetItemDialog::onOkClicked(){

    QString name = nameComboBox->currentText();
    int value = valueSpinBox->value();
    int min = minSpinBox->value();
    int max = maxSpinBox->value();
    int variation = (variationSpinBox->value());

    // Find the item in the list and update its properties
    for (Item &item : Item::liItem) {
        if (item.getName() == name.toStdString()) {
            item.setValeur(value);
            item.setMin(min);
            item.setMax(max);
            item.setVariation(variation);
            item.setTendance(0);
            auto lastElement = *item.getHistorique()->rbegin();
            float lastKey = lastElement.first;
            item.addValueToHistorique(lastKey+1,value);
            break;
        }
    }

    accept();
}

void SetItemDialog::updateNameOptions(){

    // Clear the current options
    nameComboBox->clear();

    // Add the names of items to the combo box
    for (Item &item : Item::liItem) {
        nameComboBox->addItem(QString::fromStdString(item.getName()));
    }
}

void SetItemDialog::onNameIndexChanged(int index){

    if (index >= 0 && index < nameComboBox->count()) {
        QString selectedName = nameComboBox->currentText();

        // Find the selected item in the list
        for (Item &item : Item::liItem) {
            if (QString::fromStdString(item.getName()) == selectedName) {
                // Update the form elements with the item's values
                valueSpinBox->setValue(item.getValeur());
                minSpinBox->setValue(item.getMin());
                maxSpinBox->setValue(item.getMax());
                variationSpinBox->setValue(item.getVariationPercent());
                break;
            }
        }
    }
}
