#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/item.h"
#include "headers/addItemDialog.h"
#include "headers/setItemDialog.h"
#include "headers/plotWidget.h"

#include <QDebug> // Include the necessary header for qDebug()
#include <iostream>

#include <List>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidgetItem>
#include <QtGui/QStandardItemModel>
#include <QString>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->pattern = "";
    QWidget *mcentralwidget;

    // Create the buttons
    saveButton = new QPushButton("Save", this);
    loadButton = new QPushButton("Load", this);
    stepButton = new QPushButton("Step", this);
    setButton = new QPushButton("Set", this);
    addButton = new QPushButton("Add", this);
    //


    // Create the line edit widget
    lineEdit = new QLineEdit(this);

    // Create the table view widget
    //QTableView *tableView = new QTableView(this);
    tableWidget = new QTableWidget(this);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // Create the layout
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralwidget);

    // Create a widget to hold the buttons
    QWidget *buttonWidget = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(stepButton);
    buttonLayout->addWidget(setButton);
    buttonLayout->addWidget(addButton);

    // Set the size policy for the buttons
    QSizePolicy buttonSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    loadButton->setSizePolicy(buttonSizePolicy);
    stepButton->setSizePolicy(buttonSizePolicy);
    setButton->setSizePolicy(buttonSizePolicy);

    // Add the button widget to the main layout
    mainLayout->addWidget(buttonWidget);

    // Add the line edit widget to the main layout
    mainLayout->addWidget(lineEdit);

    // Add the table view widget to the main layout
    //mainLayout->addWidget(tableView);
    mainLayout->addWidget(tableWidget);

    mainLayout->setStretchFactor(tableWidget, 1);

    //process data for qTableView
    //Item::createList();
    std::list<Item> list = Item::liItem;

    std::size_t length = list.size();

    QStringList headers;
    headers << "Nom" << "Valeur" << "Tendance" << "Graph" << "Delete";
    tableWidget->setRowCount(length);
    tableWidget->setColumnCount(5);

    tableWidget->setHorizontalHeaderLabels(headers);


    // Set the horizontal stretch factor for each column
    for (int column = 0; column < tableWidget->columnCount(); ++column) {
        tableWidget->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
    }


    int row = 0;
    for (Item& elm : Item::liItem) {
        //std::cout<<"------------Item: " << elm.getName()<< " valeur : " << elm.getTendance() <<std::endl;
        if (pattern.isEmpty() || QString::fromStdString(elm.getName()).toLower().contains(pattern.toLower())){
            QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(elm.getName()));
            tableWidget->setItem(row, 0, itemName);

            QTableWidgetItem* itemValue = new QTableWidgetItem(QString::number(elm.getValeur()));
            tableWidget->setItem(row, 1, itemValue);

            QString tendance;
            if(elm.getTendance() < 0){
                tendance = "Baisse";
            }else if (elm.getTendance() > 0){
                tendance = "Hausse";
            }else{
                tendance = "Maintient";
            }
            itemValue = new QTableWidgetItem(tendance);
            tableWidget->setItem(row, 2, itemValue);

            QPushButton* graphButton = new QPushButton("Graph");
            tableWidget->setCellWidget(row, 3, graphButton);
            connect(graphButton, &QPushButton::clicked, this, &MainWindow::handleGraphButtonClicked);

            QPushButton* delButton = new QPushButton("Delete");
            tableWidget->setCellWidget(row, 4, delButton);
            connect(delButton, &QPushButton::clicked, this, &MainWindow::handleButtonClicked);

            row++;
        }

    }
    // Set the main layout for the central widget
    mcentralwidget = new QWidget(this);
    mcentralwidget->setLayout(mainLayout);
    this->setCentralWidget(mcentralwidget);

    // Connect the "Save" button clicked signal to a slot
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveClicked);
    // Connect the "Load" button clicked signal to a slot
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadClicked);
    // Connect the "Step" button clicked signal to a slot
    connect(stepButton, &QPushButton::clicked, this, &MainWindow::stepClicked);
    // Connect the "Set" button clicked signal to a slot
    connect(setButton, &QPushButton::clicked, this, &MainWindow::setClicked);
    // Connect the line edit's return pressed signal to a slot
    connect(lineEdit, &QLineEdit::returnPressed, this, &MainWindow::lineEditReturnPressed);
    //Connect the add button clicked signal to a slot
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addClicked);
    //Connect the delete button clicked signal to a slot

    //connect(tableWidget, &QTableWidget::cellClicked, this, &MainWindow::deleteButtonClicked);
    //connect(tableWidget, &QTableWidget::cellClicked, this, &MainWindow::cellClicked);
    //connect(delButton, &QPushButton::clicked, this, &MainWindow::handleButtonClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete saveButton;
    delete loadButton;
    delete stepButton;
    delete setButton;
    delete addButton;
    delete lineEdit;
    // Clear the table widget contents
    tableWidget->clearContents();

    // Delete the table widget items
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int column = 0; column < tableWidget->columnCount(); ++column) {
            QTableWidgetItem* item = tableWidget->takeItem(row, column);
            delete item;
        }
    }

    // Delete the table widget
    delete tableWidget;
}

// Slot for the "Save" button clicked signal
void MainWindow::saveClicked()
{
    // Implementation for the save button clicked
    //qDebug() << "Save button clicked";
    Item::serializeItemsTxt();
}

// Slot for the "Load" button clicked signal
void MainWindow::loadClicked()
{
    // Implementation for the load button clicked
    //qDebug() << "Load button clicked";
    Item::deserializeItemsTxt();
    refreshTableView();
}

// Slot for the "Step" button clicked signal
void MainWindow::stepClicked()
{
    // Implementation for the step button clicked
    for (Item& elm : Item::liItem) {
        elm.computeVariation();
    }
    //qDebug() << "Step button clicked";
    refreshTableView();
}

// Slot for the "Set" button clicked signal
void MainWindow::setClicked()
{
    // Implementation for the set button clicked
    //qDebug() << "Set button clicked";

    // Create and show the SetItemDialog
    SetItemDialog setItemDialog(this);
    if (setItemDialog.exec() == QDialog::Accepted) {
        // Refresh the table view to display the updated item properties
        refreshTableView();
    }

}

void MainWindow::addClicked()
{
    // Implementation for the set button clicked
    //qDebug() << "Add button clicked";
    AddItemDialog *addItemDialog = new AddItemDialog(this);
    connect(addItemDialog, &AddItemDialog::addItem, this, &MainWindow::addItemToList);
    addItemDialog->exec();
}

// Slot for the line edit's return pressed signal
void MainWindow::lineEditReturnPressed(){
    // Implementation for the line edit return pressed
    QLineEdit* senderLineEdit = qobject_cast<QLineEdit*>(sender());
    if (senderLineEdit) {
        pattern = senderLineEdit->text(); // Store the line edit's text as the pattern
    }
    refreshTableView();
}

void MainWindow::addItemToList(QString& name, int value, int min, int max, int variation){
    //Item::liItem.clear();
    Item newItem(name.toStdString(), value, min, max, 0, variation);
    Item::liItem.push_back(newItem);

    // Refresh the table view to display the new item
    refreshTableView();
    /*
    for (Item& elm : Item::liItem) {
        qDebug() <<"------------Item: " << elm.getName()<< " valeur : " << elm.getTendance();
    }*/
}

void MainWindow::refreshTableView(){
    // Clear the existing table
    this->tableWidget->clearContents();

    // Process data for QTableView
    std::size_t length = Item::liItem.size();
    tableWidget->setRowCount(length);

    int row = 0;
    for (Item &elm : Item::liItem) {
        if (pattern.isEmpty() || QString::fromStdString(elm.getName()).toLower().contains(pattern.toLower())){
            QTableWidgetItem *itemName = new QTableWidgetItem(QString::fromStdString(elm.getName()));
            tableWidget->setItem(row, 0, itemName);

            QTableWidgetItem *itemValue = new QTableWidgetItem(QString::number(elm.getValeur()));
            tableWidget->setItem(row, 1, itemValue);

            QString tendance;
            if (elm.getTendance() < 0) {
                tendance = "Baisse";
            } else if (elm.getTendance() > 0) {
                tendance = "Hausse";
            } else {
                tendance = "Maintient";
            }
            itemValue = new QTableWidgetItem(tendance);
            tableWidget->setItem(row, 2, itemValue);

            QPushButton *graphButton = new QPushButton("Graph");
            tableWidget->setCellWidget(row, 3, graphButton);
            connect(graphButton, &QPushButton::clicked, this, &MainWindow::handleGraphButtonClicked);

            QPushButton *delButton = new QPushButton("Delete");
            tableWidget->setCellWidget(row, 4, delButton);
            connect(delButton, &QPushButton::clicked, this, &MainWindow::handleButtonClicked);


            row++;
        }
    }
}

void MainWindow::deleteButtonClicked(int row){
    qDebug() << "Delete Button clicked called: " << row;
}

void MainWindow::handleButtonClicked(){
    //qDebug() << "handleButtonClicked called: ";

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button){
        // Perform the desired action here
        int row = tableWidget->indexAt(button->pos()).row();

        QTableWidgetItem* itemName = tableWidget->item(row, 0);
        QString name = itemName->text();
        // Remove the item from the liItem list
        auto iter = std::find_if(Item::liItem.begin(), Item::liItem.end(),
                                 [&name](Item& item) { return item.getName() == name.toStdString(); });

        if (iter != Item::liItem.end()) {
            Item::liItem.erase(iter);
        }

        // Remove the row from the table widget
        tableWidget->removeRow(row);
        refreshTableView();
    }
}
void MainWindow::handleGraphButtonClicked(){

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int row = tableWidget->indexAt(button->pos()).row();

        // Iterate to the desired position in the list
        std::list<Item>::iterator iter = Item::liItem.begin();
        std::advance(iter, row);

        // Check if the iterator is valid
        if (iter != Item::liItem.end()) {
            // Access the item at the desired position
            Item& item = *iter;

            // Create a new dialog window
            QDialog *dialog = new QDialog(this);
            dialog->setWindowTitle("Plot Window");

            // Create a PlotWidget instance and pass the historique data
            PlotWidget *plotWidget = new PlotWidget(item.getHistorique(), dialog);

            // Set the plot widget as the main layout of the dialog
            QVBoxLayout *layout = new QVBoxLayout(dialog);
            layout->addWidget(plotWidget);
            dialog->setLayout(layout);

            // Set the dialog window as modal
            dialog->setModal(true);

            // Set the size of the dialog window
            dialog->resize(800, 600); // Set your desired width and height

            // Show the dialog window
            dialog->show();
        }
    }
}


