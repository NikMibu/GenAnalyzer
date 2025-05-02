#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>
#include <filesystem>
#include <ctime>
#include <iomanip>  // Für std::setw → Formatieren des Reports

#include "Genome.h"
#include "SNP.h"
#include "Disease.h"
#include "Analyzer.h"

// Führt die Analyse eines Genoms auf risikoassoziierte SNPs durch
void Analyzer::runAnalysis(const Genome& genome, const Disease& disease) {
    this->genome = &genome;
    this->disease = &disease;

    // Vergleicht jedes Risk-SNP mit dem geladenen Genom
    for (const auto& riskSNP : disease.getRiskSNPs()) {
        if (genome.hasSNP(riskSNP.rsID)) {
            const SNP* snp = genome.getSNPByID(riskSNP.rsID);
            matchedSNPs.push_back(snp);
            matchedDiseaseSNPs.push_back(riskSNP);
        }
    }
}

// Berechnet den Risiko-Score nach einfacher Heuristik
// Heterozygot = 1 Punkt, Homozygot rezessiv = 2 Punkte
int Analyzer::calculateRiskScore() const {
    int score = 0;
    for (const SNP* snp : matchedSNPs) {
        switch (snp->getGenotypeStatus()) {
            case GenotypeStatus::HomozygousRecessive: score += 2; break;
            case GenotypeStatus::Heterozygous:        score += 1; break;
            default: break;
        }
    }
    return score;
}

// Weist einem numerischen Score ein RiskLevel zu
RiskLevel Analyzer::getRiskLevel(int score) const {
    if (score <= 3)
        return RiskLevel::Low;
    else if (score <= 7)
        return RiskLevel::Medium;
    else
        return RiskLevel::High;
}

// Gibt einen lesbaren String zum RiskLevel zurück
std::string Analyzer::riskLevelToString(RiskLevel level) {
    switch (level) {
        case RiskLevel::Low: return "Gering";
        case RiskLevel::Medium: return "Mäßig erhöht";
        case RiskLevel::High: return "Hoch";
        default: return "Unbekannt";
    }
}

// Gibt alle Treffer + Risk-Score im Terminal aus
void Analyzer::printSummary() const {
    int score = calculateRiskScore();
    RiskLevel level = getRiskLevel(score);

    std::cout << "---------------------------" << std::endl;
    std::cout << "Gefundene Risiko-SNPs für: " << genome->getSampleID() << "\n\n";

    for (size_t i = 0; i < matchedSNPs.size(); i++) {
        const SNP* snp = matchedSNPs[i];
        const DiseaseSNP& info = matchedDiseaseSNPs[i];

        std::cout << snp->getRSID() << "\t"
                  << snp->getGenotype() << "\t"
                  << genotypeStatusToString(snp->getGenotypeStatus()) << "\t"
                  << info.gene << "\t"
                  << info.function << std::endl;
    }

    std::cout << "\nRisiko-Score: " << score << " → " << riskLevelToString(level) << std::endl;
    std::cout << "---------------------------" << std::endl;
}

// Schreibt die Analyseergebnisse in eine Datei (z. B. für Dokumentation)
void Analyzer::saveResults(const std::string& filename) const {
    if (!genome || !disease) {
        throw std::runtime_error("Analyzer wurde nicht richtig initialisiert");
    }

    std::filesystem::create_directories("data/output");

    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Datei konnte nicht geschrieben werden: " + filename);
    }

    int score = calculateRiskScore();
    RiskLevel level = getRiskLevel(score);

    time_t now = time(0);
    char* dt = ctime(&now);

    out << "Analysis Date: " << dt;
    out << "----------------------------------------\n";
    out << "SampleID: " << genome->getSampleID() << "\n";
    out << "SNPCount: " << genome->getSNPCount() << "\n";
    out << "Disease: " << disease->getName() << "\n";
    out << "Treffer: " << matchedSNPs.size() << "\n";
    out << "Risiko-Score: " << score << " (" << riskLevelToString(level) << ")\n";
    out << "----------------------------------------\n";

    // Tabellenheader
    out << std::left;
    out << std::setw(12) << "rsID"
        << std::setw(10) << "Genotyp"
        << std::setw(25) << "Status"
        << std::setw(10) << "Gen"
        << "Funktion" << "\n";
    out << "----------------------------------------------------------------------------\n";

    // Trefferzeilen
    for (size_t i = 0; i < matchedSNPs.size(); ++i) {
        const SNP* snp = matchedSNPs[i];
        const DiseaseSNP& info = matchedDiseaseSNPs[i];
        out << std::setw(12) << snp->getRSID()
            << std::setw(10) << snp->getGenotype()
            << std::setw(25) << genotypeStatusToString(snp->getGenotypeStatus())
            << std::setw(10) << info.gene
            << info.function << "\n";
    }

    out << "----------------------------------------------------------------------------\n";
    out.close();
}
