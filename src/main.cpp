#include "headers/mainwindow.h"
#include "headers/item.h"

#include <QApplication>

#include <List>
#include <iostream>
#include <random>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //Item::createList();
    /*
    for(Item& elm : Item::liItem){
        std::cout<<"Item: " << elm.getName()<< " valeur : " << elm.getValeur() <<std::endl;
        elm.computeVariation();
        elm.computeVariation();
        elm.computeVariation();
        elm.computeVariation();
    }
    for(Item& elm : Item::liItem){
        std::cout<<"Item: " << elm.getName() << " Nouvelle valeur " << elm.getValeur() << std::endl;
        auto iter = elm.getHistorique()->begin();
        while (iter != elm.getHistorique()->end()) {
            std::cout << "[" << iter->first << ","
                 << iter->second << "]\n";
            ++iter;
        }
        std::cout << std::endl;
    }
    */


    return a.exec();
}



