#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <map>
#include <cmath>
#include <List>


class Item
{
private:
    std::string nom;
    float valeur;
    float min;
    float max;
    int tendance;
    float variation;
    std::map<float, float> historique; //to store old value and print a graph


public:
    Item(); //Constructeur
    Item(std::string name, float valeur, float min, float max, int tendance, float variation); //Constructeur
    void setName(std::string name);
    void setValeur(float val);
    void setMin(float min);
    void setMax(float max);
    void setTendance(int x);
    void setVariation(float x);
    std::string getName();
    float getValeur();
    float getMin();
    float getMax();
    int getTendance();
    float getVariation();
    float getVariationPercent();
    std::map<float, float> *getHistorique();
    void addValueToHistorique(float x, float y);
    void computeVariation();
    static std::list<Item> liItem;
    static void createList();
    static void serializeItems();
    static void deserializeItems();
    static void serializeItemsTxt();
    static void deserializeItemsTxt();

};

#endif // ITEM_H
