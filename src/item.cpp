#include "headers/item.h"
#include <cstdlib>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <Windows.h>
#include <fstream>
#include <sstream>

Item::Item() : nom("No Name"), valeur(0), min(0), max(100), tendance(0), variation(0.25), historique(std::map<float, float>())
{

}

Item::Item(std::string name, float valeur, float min, float max, int tendance, float variation)
{
    this->nom = name;
    this->valeur = valeur;
    this->min = min;
    this->max = max;
    this->tendance = tendance;
    this->variation = variation/100;
    std::map<float, float> tmp = std::map<float, float>();
    tmp[0] = valeur;
    this->historique = tmp;
}

void Item::setName(std::string name)
{
    this->nom = name;
}

void Item::setValeur(float val)
{
    this->valeur = std::abs(val);
}

void Item::setMin(float min)
{
    if((min < this->max) && (min > 0)){
        this->min = min;
    }else{
        float tmp = std::abs(min-max);
        if(tmp < min){
            this->min = tmp;
        }else{
            this->min = 0;
        }
    }
}

void Item::setVariation(float x){
    this->variation = (std::abs(x))/100; //input in %
}

void Item::setMax(float max)
{
    if(max > this->min && max > 0){
        this->max = max;
    }else{
        float tmp = std::abs(min-max);
        if(tmp > min){
            this->max = tmp;
        }else{
            this->max = std::abs(min)*2;
        }
    }
}

void Item::setTendance(int x){
    this->tendance = x;
}
std::string Item::getName()
{
    return this->nom;
}

float Item::getValeur()
{
    return this->valeur;
}

float Item::getMin()
{
    return this->min;
}

float Item::getMax()
{
    return this->max;
}

int Item::getTendance()
{
    return this->tendance;
}

float Item::getVariation(){
    return this->variation;
}

float Item::getVariationPercent(){
    return (this->variation*100);
}

std::map<float, float> *Item::getHistorique()
{
    return &this->historique;
}

void Item::addValueToHistorique(float x, float y)
{
    this->historique[x] = y;
}

std::list<Item> Item::liItem;

void Item::createList() {
    Item bois("Bois", 50, 15, 100, 0, 10);
    Item fer("Fer", 150, 50, 300, 1, 10);
    Item grain("Grain", 10, 5, 35, -1, 10);
    Item::liItem.emplace_back(bois);
    Item::liItem.emplace_back(fer);
    Item::liItem.emplace_back(grain);
}


float generateRandomNumber(int min, int max) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    std::uniform_real_distribution<> distr(0, min+max);
    float tmp = distr(eng);
    return tmp-min;
}

float generateRandomFloat(float min, float max) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dis(min, max);
    float tmp =  dis(gen);
    return tmp;
}

void Item::computeVariation(){
    float valeurActuelle = this->getValeur();
    float valeurMin = this->getMin();
    float valeurMax = this->getMax();

    float facteurVariation = this->getVariation(); // Facteur de variation de x%

    // Génération d'un nombre aléatoire entre des bornes:
    float distToLow = valeurActuelle-valeurMin;
    //float distToHigh = valeurMax-valeurActuelle;
    float total = valeurMax-valeurMin;

    float signeVariation = generateRandomFloat(0, 100);
    int exceptionalValue = generateRandomNumber(1, 25);

    float qte = generateRandomFloat(1, 100*facteurVariation);
    float nombreAleatoire;

    // Calcul de la variation
    float nouvelleValeur;

    if((signeVariation/100) > (distToLow/total)){
        //add
        nombreAleatoire = 1 + std::abs((qte/100));
    }else{
        //Reduce
        nombreAleatoire = 1 - std::abs((qte/100));
    }

    if(exceptionalValue == 5){
        float mult = generateRandomFloat(1.01,1.5);
        nouvelleValeur = valeurActuelle * (nombreAleatoire * mult);
    }else {
        nouvelleValeur = valeurActuelle * nombreAleatoire;
    }


    // Calcul de la nouvelle valeur
    // Limiter la nouvelle valeur dans la plage définie par valeurMin et valeurMax
    if(nouvelleValeur > valeurMax){
        nouvelleValeur = valeurMax;
    }
    if(nouvelleValeur < valeurMin){
        nouvelleValeur = valeurMin;
    }

    /*
    std::cout  << valeurActuelle<< std::endl;
    std::cout << " nouvelle Valeur " << nouvelleValeur<< std::endl;
    std::cout << " Nb aléa " << nombreAleatoire<< std::endl;
  if(this->getName() == "zef"){
    std::cout << "Nom: " << this->getName() << " Dist to Low: " << valeurActuelle-valeurMin << " Dist to High: " << valeurMax-valeurActuelle << "\nSigne var" \
                     << signeVariation << " seuil: " << distToLow/total <<  " exceptionalValue: " << exceptionalValue << " Nombre aleatoire: " << nombreAleatoire << std::endl;
    }
*/

    this->setValeur(nouvelleValeur);

    if(valeurActuelle > nouvelleValeur){
        this->setTendance(-1);
    }else if(valeurActuelle < nouvelleValeur){
        this->setTendance(1);
    }else{
        this->setTendance(0);
    }

    //add the new value to the history
    std::vector<float> cles;
    std::map<float, float> *historique = this->getHistorique();
    if(historique){
        for(auto elm = historique->begin(); elm != historique->end(); elm++){
            cles.push_back(elm->first);
        }
    }
    //get the max:
    float max = -1;
    if (!cles.empty()) {
        max = *std::max_element(cles.begin(), cles.end());
    }
    (*historique)[max+1] = nouvelleValeur;
    return;
}

void Item::serializeItems() {
    // Show a file dialog to choose the save location
    QString saveFileName = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Data Files (*.dat)");

    if (!saveFileName.isEmpty()) {
        // Serialize the items and save the file at the chosen location
        std::ofstream outputFile(saveFileName.toStdString(), std::ios::binary);
        if (outputFile) {
            // Serialize the items
            size_t itemCount = liItem.size();
            outputFile.write(reinterpret_cast<const char*>(&itemCount), sizeof(itemCount));
            for (Item& item : liItem) {
                // Serialize the item properties
                std::string name = item.getName();
                float value = item.getValeur();
                float minValue = item.getMin();
                float maxValue = item.getMax();
                int trend = item.getTendance();
                float variation = item.getVariation();

                // Get the historical data
                std::map<float, float>* historique = item.getHistorique();
                int historiqueSize = historique->size();

                // Write the size of the name string to the file
                size_t nameSize = name.size();
                outputFile.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));

                // Write the item properties to the file
                outputFile.write(name.c_str(), nameSize + 1);
                outputFile.write(reinterpret_cast<char*>(&value), sizeof(float));
                outputFile.write(reinterpret_cast<char*>(&minValue), sizeof(float));
                outputFile.write(reinterpret_cast<char*>(&maxValue), sizeof(float));
                outputFile.write(reinterpret_cast<char*>(&trend), sizeof(int));
                outputFile.write(reinterpret_cast<char*>(&variation), sizeof(float));

                // Write the size of the historical data map
                outputFile.write(reinterpret_cast<char*>(&historiqueSize), sizeof(int));

                // Write the historical data to the file
                for (auto& entry : *historique) {
                    float key = entry.first;
                    float value = entry.second;

                    outputFile.write(reinterpret_cast<char*>(&key), sizeof(float));
                    outputFile.write(reinterpret_cast<char*>(&value), sizeof(key));
                }
            }

            outputFile.close();

            // Display a message box indicating the end of serialization and the chosen file name
            QString message = "Save completed. File name: " + saveFileName;
            QMessageBox::information(nullptr, "Serialization", message);
        } else {
            // Display an error message if the file couldn't be opened
            QMessageBox::critical(nullptr, "Error", "Failed to open the file for writing.");
        }
    }
}

void Item::deserializeItems() {
    // Show a file dialog to choose the file to deserialize
    QString openFileName = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Data Files (*.dat)");

    if (!openFileName.isEmpty()) {
        // Deserialize the items from the chosen file
        std::ifstream file(openFileName.toStdString(), std::ios::binary);
        if (file) {
            // Clear the existing list of items
            liItem.clear();

            // Deserialize the items
            size_t itemCount;
            file.read(reinterpret_cast<char*>(&itemCount), sizeof(itemCount));

            for (size_t i = 0; i < itemCount; ++i) {
                size_t nameSize;

                //Read the size of string
                file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));

                /*
                std::vector<char> nameBuffer(nameSize);
                file.read(nameBuffer.data(), nameSize);
                std::string name(nameBuffer.begin(), nameBuffer.end());
                */
                std::string name(nameSize, '\0');
                file.read(&name[0], nameSize);


                float valeur, min, max;
                int tendance;
                float variation;
                file.read(reinterpret_cast<char*>(&valeur), sizeof(float));
                file.read(reinterpret_cast<char*>(&min), sizeof(float));
                file.read(reinterpret_cast<char*>(&max), sizeof(float));
                file.read(reinterpret_cast<char*>(&tendance), sizeof(int));
                file.read(reinterpret_cast<char*>(&variation), sizeof(float));
                int historySize;
                file.read(reinterpret_cast<char*>(&historySize), sizeof(int));

                std::map<float, float> historique;
                for (int j = 0; j < historySize; ++j) {
                    float key, value;
                    file.read(reinterpret_cast<char*>(&key), sizeof(key));
                    file.read(reinterpret_cast<char*>(&value), sizeof(value));
                    historique[key] = value;
                }
                liItem.emplace_back(name, valeur, min, max, tendance, variation);
                liItem.back().getHistorique()->swap(historique);
            }

            file.close();

            // Display a message box indicating the completion of deserialization
            QMessageBox::information(nullptr, "Deserialization", "Deserialization completed successfully.");
        } else {
            // Display an error message if the file couldn't be opened
            QMessageBox::critical(nullptr, "Error", "Failed to open the file for reading.");
        }
    }
}

void Item::serializeItemsTxt() {
    // Show a file dialog to choose the save location
    QString saveFileName = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Data Files (*.csv)");

    if (!saveFileName.isEmpty()) {
        // Open the file for writing
        std::ofstream outputFile(saveFileName.toStdString());
        if (outputFile) {
            // Serialize the items
            for (Item& item : liItem) {
                // Serialize the item properties
                outputFile << item.getName() << ","
                           << item.getValeur() << ","
                           << item.getMin() << ","
                           << item.getMax() << ","
                           << item.getTendance() << ","
                           << item.getVariation() << "\n";

                // Serialize the historical data
                const std::map<float, float>& historique = *item.getHistorique();
                for (auto& entry : historique) {
                    outputFile << entry.first << "," << entry.second << "\n";
                }

                // Add a separator line between items
                outputFile << "---\n";
            }

            outputFile.close();

            // Display a message box indicating the end of serialization and the chosen file name
            QString message = "Save completed. File name: " + saveFileName;
            QMessageBox::information(nullptr, "Serialization", message);
        } else {
            // Display an error message if the file couldn't be opened
            QMessageBox::critical(nullptr, "Error", "Failed to open the file for writing.");
        }
    }
}

void Item::deserializeItemsTxt() {
    // Show a file dialog to choose the file to deserialize
    QString openFileName = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Data Files (*.csv)");

    if (!openFileName.isEmpty()) {
        // Open the file for reading
        std::ifstream inputFile(openFileName.toStdString());
        if (inputFile) {
            // Clear the existing items list
            liItem.clear();

            // Deserialize the items
            std::string line;
            while (std::getline(inputFile, line)) {
                if (line == "---") {
                    // Separator line between items, move to the next item
                    continue;
                }

                std::istringstream iss(line);
                std::string token;
                std::vector<std::string> tokens;
                while (std::getline(iss, token, ',')) {
                    tokens.push_back(token);
                }

                if (tokens.size() >= 6) {
                    // Deserialize the item properties
                    std::string name = tokens[0];
                    float value = std::stof(tokens[1]);
                    float minValue = std::stof(tokens[2]);
                    float maxValue = std::stof(tokens[3]);
                    int trend = std::stoi(tokens[4]);
                    float variation = std::stof(tokens[5]);

                    // Create a new item with the deserialized properties
                    Item item(name, value, minValue, maxValue, trend, variation);

                    // Deserialize the historical data
                    for (size_t i = 6; i < tokens.size(); i += 2) {
                        float key = std::stof(tokens[i]);
                        float value = std::stof(tokens[i + 1]);
                        item.addValueToHistorique(key, value);
                    }

                    // Add the deserialized item to the list
                    liItem.push_back(item);
                }
            }

            inputFile.close();

            // Display a message box indicating the end of deserialization and the chosen file name
            QString message = "Load completed. File name: " + openFileName;
            QMessageBox::information(nullptr, "Deserialization", message);
        } else {
            // Display an error message if the file couldn't be opened
            QMessageBox::critical(nullptr, "Error", "Failed to open the file for reading.");
        }
    }
}


