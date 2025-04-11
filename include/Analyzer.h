#pragma once
#include "Genome.h"
#include "Disease.h"

/**
 * Macht die eigentliche Analyse
 * Klassifizierung ist eine vereinfachte Heuristik, keine medizinisch validierte Bewertung
 * Es fehlen populationsbezogene Risikoquoten, Validierungsstudien, statistische Wahrscheinlichkeiten.
 * 
 * Attribute:
 * - Genome genome
 * - std::vector<Disease> disease
 * - std::vector<std::string> detectedRisks
 * 
 * Funktionen:
 * - void runAnalysis()
 * - void saveResults(const std::string& filename) // writeToFile()
 * - void printSummary()
 * maybe:
 * - calculateRiskScore() Heterozygot = 1 Punkt, Homozygot = 2 Punkte, Summe aller Treffer = Score
 * - analyzeMultipleDiseases()
 * - groupResultsByGene()
 * - highlightCriticalGenotypes()
 * 
 * 
 * Persistierung:
 * - void Analyzer::writeResultsToFile(const std::string& filename, const Genome& genome, const std::vector<DiseaseSNP>& results);
 * - void load 
 */
enum class RiskLevel {
    Low,
    Medium,
    High
};


 class Analyzer {
    public:
        void runAnalysis(const Genome& genome, const Disease& disease);         // macht alles
        void saveResults(const std::string& filename) const;                    // speichert Treffer
        void printSummary() const;                                              // zeigt Treffer
        int calculateRiskScore() const;                                         // gibt Score zurück
        RiskLevel getRiskLevel(int score) const;
        static std::string riskLevelToString(RiskLevel level);
    
    private:
        const Genome* genome = nullptr;  
        const Disease* disease = nullptr;  
        std::vector<const SNP*> matchedSNPs;        // Zeiger auf gefundene SNPs im Genome
        std::vector<DiseaseSNP> matchedDiseaseSNPs; // zugehörige Infos aus Disease
};
