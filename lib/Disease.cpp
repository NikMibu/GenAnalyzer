#include <fstream>
#include <sstream>
#include <iostream>

#include "Disease.h"

Disease::Disease(const std::string& name) : name(name) {}

//Lesen und Erstellen einer Krankheit von einer Datei aus
void Disease::loadRiskSNPsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Datei konnte nicht geöffnet werden: " + filename);
    }

    std::string line;
    std::getline(file, line); // Header überspringen

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string rsID, gene, function;

        if (std::getline(ss, rsID, '\t') &&
            std::getline(ss, gene, '\t') &&
            std::getline(ss, function)) // Rest der Zeile als Funktion
        {
            riskSNPs.push_back(DiseaseSNP(rsID, gene, function));
        }
    }

    std::cout << riskSNPs.size() << " Risiko-SNPs für '" << name << "' geladen." << std::endl;
}

const std::string& Disease::getName() const {
    return name;
}
//Gibt die RisikoSNPs der Krankheit aus
const std::vector<DiseaseSNP>& Disease::getRiskSNPs() const {
    return riskSNPs;
}
// Erstellt eine Übersicht über diese RiskSNPs der Krankheit
void Disease::printRiskSNPs() const {
    std::cout << "---------------------------"<< std::endl;
    std::cout << "RiskSNPs of Disease: " << getName() << std::endl;
    for (const auto& snp : getRiskSNPs()) {
        std::cout << "✔ " << snp.rsID << " (" << snp.gene << ") – " << snp.function << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
}

