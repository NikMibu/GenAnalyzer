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

// Bewertet den Genotyp eines SNPs mit Punkten
int Analyzer::scoreGenotype(GenotypeStatus status) {
    switch (status) {
        case GenotypeStatus::HomozygousRecessive: return 2;
        case GenotypeStatus::Heterozygous:        return 1;
        default:                                  return 0;
    }
}

// Führt die Analyse eines Genoms auf risikoassoziierte SNPs durch
void Analyzer::runAnalysis(const Genome& g, const Disease& d) {
    this->genome = &g;

    AnalysisResult result(d);
    result.disease = d;  // Kopie der geladenen Disease
    result.total_score = static_cast<int>(d.getRiskSNPs().size());

    // Vergleicht jedes Risk-SNP mit dem geladenen Genom
    for (const auto& riskSNP : d.getRiskSNPs()) {
        if (g.hasSNP(riskSNP.rsID)) {
            const SNP* snp = g.getSNPByID(riskSNP.rsID);
            result.matchedSNPs.push_back(snp);
            result.matchedDiseaseSNPs.push_back(riskSNP);
            result.score += scoreGenotype(snp->getGenotypeStatus());
        }        
    }

    analyses.push_back(std::move(result));
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
    if (!genome || analyses.empty()) {
        std::cout << " Keine Analyseergebnisse vorhanden." << std::endl;
        return;
    }

    for (const auto& res : analyses) {
        RiskLevel level = getRiskLevel(res.score);

        std::cout << "---------------------------" << std::endl;
        std::cout << "Analyse für Krankheit: " << res.disease.getName() << std::endl;
        std::cout << "Gefundene Risiko-SNPs für: " << genome->getSampleID() << "\n\n";

        for (size_t i = 0; i < res.matchedSNPs.size(); ++i) {
            const SNP* snp = res.matchedSNPs[i];
            const DiseaseSNP& info = res.matchedDiseaseSNPs[i];

            std::cout << snp->getRSID() << "\t"
                      << snp->getGenotype() << "\t"
                      << genotypeStatusToString(snp->getGenotypeStatus()) << "\t"
                      << info.gene << "\t"
                      << info.function << std::endl;
        }

        std::cout << "\nRisiko-Score: " << res.score << " / " << (res.total_score * 2)
                  << " → " << riskLevelToString(level) << std::endl;
    }

    std::cout << "---------------------------" << std::endl;
}

// Schreibt alle Analyseergebnisse in eine Datei (z. B. für Dokumentation)
void Analyzer::saveResults(const std::string& filename) const {
    if (!genome || analyses.empty()) {
        throw std::runtime_error("Analyzer wurde nicht richtig initialisiert oder enthält keine Analysen.");
    }

    std::filesystem::create_directories("data/output");

    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Datei konnte nicht geschrieben werden: " + filename);
    }

    time_t now = time(0);
    char* dt = ctime(&now);

    out << "Analysis Date: " << dt;
    out << "SampleID: " << genome->getSampleID() << "\n";
    out << "SNPCount: " << genome->getSNPCount() << "\n";
    out << "========================================\n";

    for (const auto& res : analyses) {
        RiskLevel level = getRiskLevel(res.score);

        out << "Krankheit: " << res.disease.getName() << "\n";
        out << "Treffer: " << res.matchedSNPs.size() << " / " << res.total_score << "\n";
        out << "Risiko-Score: " << res.score << " (" << riskLevelToString(level) << ")\n";

        out << std::left;
        out << std::setw(12) << "rsID"
            << std::setw(10) << "Genotyp"
            << std::setw(25) << "Status"
            << std::setw(10) << "Gen"
            << "Funktion" << "\n";
        out << "----------------------------------------------------------------------------\n";

        for (size_t i = 0; i < res.matchedSNPs.size(); ++i) {
            const SNP* snp = res.matchedSNPs[i];
            const DiseaseSNP& info = res.matchedDiseaseSNPs[i];

            out << std::setw(12) << snp->getRSID()
                << std::setw(10) << snp->getGenotype()
                << std::setw(25) << genotypeStatusToString(snp->getGenotypeStatus())
                << std::setw(10) << info.gene
                << info.function << "\n";
        }

        out << "----------------------------------------------------------------------------\n\n";
    }

    out.close();
}

// Prüft, ob ein Genom geladen wurde und mindestens eine Analyse existiert
bool Analyzer::isInitialized() const {
    return genome != nullptr && !analyses.empty();
}
