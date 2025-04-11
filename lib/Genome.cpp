#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>

#include "Genome.h"
#include "SNP.h"

void Genome::loadFromFile(const std::string& filename, size_t maxLines) {
    // Datei öffnen
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Datei konnte nicht geöffnet werden: " + filename);
    }

    std::string line;
    size_t count = 0; // Anzahl erfolgreich geladener SNPs

    // Datei zeilenweise einlesen
    while (std::getline(infile, line)) {
        // Leere Zeilen oder Kommentare überspringen
        if (line.empty() || line[0] == '#') continue;

        // Einzelne Werte aus der Zeile extrahieren
        std::istringstream ss(line);
        std::string rsid, chromosome, positionStr;
        char allele1 = '-', allele2 = '-';
        int position;

        // Zeile korrekt im Tab-Format einlesen
        if (std::getline(ss, rsid, '\t') &&
            std::getline(ss, chromosome, '\t') &&
            std::getline(ss, positionStr, '\t') &&
            ss >> allele1 >> allele2)
        {
            try {
                // Position als Integer parsen
                position = std::stoi(positionStr);

                // Neues SNP-Objekt erstellen und hinzufügen
                SNP snp(rsid, chromosome, position, allele1, allele2);
                addSNP(snp);
                count++;

                // Abbruch, falls maxLines erreicht
                if (maxLines > 0 && count >= maxLines) break;
            } catch (...) {
                // Fehlerhafte Zeile überspringen und melden
                std::cerr << "Fehlerhafte Zeile übersprungen: " << line << std::endl;
            }
        }
    }

    // Zusammenfassung der Ladeaktion ausgeben
    std::cout << count << " SNPs erfolgreich geladen aus Datei: " << filename << std::endl;
}



Genome::Genome(const std::string& sampleID)
    : m_sampleID(sampleID)
{}
// Sample Functions:
void Genome::setSampleID(const std::string& id) {
    m_sampleID = id;
}

std::string Genome::getSampleID() const {
    return m_sampleID;
}

// SNPs Functions: 
const SNP* Genome::getSNPByID(const std::string& rsID) const {
    for (const auto& snp : m_snps) {
        if (snp.getRSID() == rsID) {
            return &snp;
        }
    }
    return nullptr; // SNP nicht gefunden
}

void Genome::addSNP(const SNP& snp) {
    m_snps.push_back(snp);
}

// Funktion liefert Pointer auf den gefundenen SNP, wenn er existiert, sonst nullptr
bool Genome::hasSNP(const std::string& rsID) const {
    return getSNPByID(rsID) != nullptr;
}


size_t Genome::getSNPCount() const {
    return m_snps.size();
}

void Genome::printSummary() const{
    std::cout << "---------Summary---------" << std::endl;
    std::cout << "SampleID: " << getSampleID() << std::endl;
    std::cout << "SNPCount: " << getSNPCount() << std::endl;
    std::cout << "-------------------------" << std::endl;
}