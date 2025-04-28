#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>
#include <filesystem>
#include <ctime>
#include <iomanip>  // Wichtig für std::setw -> Formatieren des Reports

#include "Genome.h"
#include "SNP.h"
#include "Disease.h"
#include "Analyzer.h"

// Analysiert ein Genome auf Risiko-SNPs
void Analyzer::runAnalysis(const Genome& genome, const Disease& disease){
    // Speichern der übergebenen Genome- und Disease-Objekte als Zeiger
    // -> Ermöglicht späteren Zugriff in anderen Methoden wie saveResults() und printSummary()
    this->genome = &genome;
    this->disease = &disease;

    for (size_t i = 0; i < disease.getRiskSNPs().size(); i++) {
        const auto& riskSNP = disease.getRiskSNPs()[i]; // aktueller RisikoSNP
    
        // überprüft ob dieses RisikoSNP in Genom enthalten ist 
        if (genome.hasSNP(riskSNP.rsID)) {
            const SNP* snp = genome.getSNPByID(riskSNP.rsID);
    
            // speichern in matchedSNPS & matchedDiseaseSNP
            matchedSNPs.push_back(snp);
            matchedDiseaseSNPs.push_back(riskSNP);
        }
    }
    
}

// Stochastische Berechnung des Risikos des in Genom enthaltener Disease RisikoSNPs - matchedSNPs
// soll nur eine Anwendung darstellen und keine richtig validierte Risiko Analyse
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
// Zuweisen des berechneten Risk Scores zu Risk Level
RiskLevel Analyzer::getRiskLevel(int score) const {
    if (score <= 3)
        return RiskLevel::Low;
    else if (score <= 7)
        return RiskLevel::Medium;
    else
        return RiskLevel::High;
}
// Konvertiert das Risk Level zu einem String 
std::string Analyzer::riskLevelToString(RiskLevel level) {
    switch (level) {
        case RiskLevel::Low: return "Gering";
        case RiskLevel::Medium: return "Mäßig erhöht";
        case RiskLevel::High: return "Hoch";
        default: return "Unbekannt";
    }
}

// interne Summary Ausgabe
void Analyzer::printSummary() const{
    int score = calculateRiskScore();
    RiskLevel level = getRiskLevel(score);

    std::cout << "---------------------------" << std::endl;
    std::cout << "Found Risk Snps in Genome of :"<< genome->getSampleID() << std::endl;
    for (size_t i = 0; i < matchedSNPs.size(); i++) {
        const SNP* snp = matchedSNPs[i];
        const DiseaseSNP& info = matchedDiseaseSNPs[i];

        std::cout << snp->getRSID() << "\t"
                << snp->getGenotype() << "\t"
                << genotypeStatusToString(snp->getGenotypeStatus()) << "\t"
                << info.gene << "\t"
                << info.function << std::endl;
    }
    std::cout << "Risiko-Score: " << score << " → "<< riskLevelToString(level) << std::endl;
    std::cout << "---------------------------" << std::endl;
}

// Schreiben einer Analyse-Zusammenfassung
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

    // Header
    // std::left, std::setw -> zum formatieren der Datei
    out << std::left;
    out << std::setw(12) << "rsID"
        << std::setw(10) << "Genotyp"
        << std::setw(25) << "Status"
        << std::setw(10) << "Gen"
        << "Funktion" << "\n";
    out << "----------------------------------------------------------------------------\n";

    // Inhalt
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



