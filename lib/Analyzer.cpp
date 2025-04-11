#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>
#include <filesystem>

#include "Genome.h"
#include "SNP.h"
#include "Disease.h"
#include "Analyzer.h"

void Analyzer::runAnalysis(const Genome& genome, const Disease& disease){
    this->genome = &genome;
    this->disease = &disease;
    for (size_t i = 0; i < disease.getRiskSNPs().size(); i++) {
        const auto& riskSNP = disease.getRiskSNPs()[i];
    
        if (genome.hasSNP(riskSNP.rsID)) {
            const SNP* snp = genome.getSNPByID(riskSNP.rsID);
    
            // z.B. speichern:
            matchedSNPs.push_back(snp);
            matchedDiseaseSNPs.push_back(riskSNP);
        }
    }
    
}

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
RiskLevel Analyzer::getRiskLevel(int score) const {
    if (score <= 3)
        return RiskLevel::Low;
    else if (score <= 7)
        return RiskLevel::Medium;
    else
        return RiskLevel::High;
}
std::string Analyzer::riskLevelToString(RiskLevel level) {
    switch (level) {
        case RiskLevel::Low: return "Gering";
        case RiskLevel::Medium: return "Mäßig erhöht";
        case RiskLevel::High: return "Hoch";
        default: return "Unbekannt";
    }
}

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

void Analyzer::saveResults(const std::string& filename) const {
    std::filesystem::create_directories("data/output"); // Ordner anlegen

    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error(" Datei konnte nicht geschrieben werden: " + filename);
    }

    int score = calculateRiskScore();
    RiskLevel level = getRiskLevel(score);

    out << "SampleID: " << genome->getSampleID() << "\n";
    out << "SNPCount: " << genome->getSNPCount() << "\n";
    out << "Disease: " << disease->getName() << "\n";
    out << "Treffer: " << matchedSNPs.size() << "\n";
    out << "Risiko-Score: " << score << " (" << riskLevelToString(level) << ")\n";
    out << "----------------------------------------\n";

    for (size_t i = 0; i < matchedSNPs.size(); ++i) {
        const SNP* snp = matchedSNPs[i];
        const DiseaseSNP& info = matchedDiseaseSNPs[i];
        out << snp->getRSID() << "\t"
            << snp->getGenotype() << "\t"
            << genotypeStatusToString(snp->getGenotypeStatus()) << "\t"
            << info.gene << "\t"
            << info.function << "\n";
    }

    out << "----------------------------------------\n";
    out.close();
}

