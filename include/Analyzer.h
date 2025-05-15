#pragma once

#include <string>
#include <vector>
#include <utility>

#include "Genome.h"
#include "Disease.h"
#include "SNP.h"

// Einfache Einteilung des genetischen Risikos basierend auf dem Score
enum class RiskLevel {
    Low,
    Medium,
    High
};

// Ergebnis einer einzelnen Krankheitsanalyse
class AnalysisResult {
    public:
        Disease disease;
        std::vector<const SNP*> matchedSNPs;
        std::vector<DiseaseSNP> matchedDiseaseSNPs;
        int score = 0;
        int total_score = 0;
    
        AnalysisResult(const Disease& d) : disease(d) {}
};
    


class Analyzer {
public:
    // Startet eine neue Analyse und fügt sie zur Ergebnisliste hinzu
    void runAnalysis(const Genome& genome, const Disease& disease);

    // Speichert alle Analyseergebnisse in eine Textdatei
    void saveResults(const std::string& filename) const;

    // Gibt alle Analysen im Terminal aus
    void printSummary() const;

    // Bewertet den Genotyp eines SNPs mit Punkten
    static int scoreGenotype(GenotypeStatus status);

    // Berechnet Risiko-Level anhand des Scores
    RiskLevel getRiskLevel(int score) const;

    // Gibt den Risiko-Level als lesbaren String zurück
    static std::string riskLevelToString(RiskLevel level);

    // Prüft, ob ein Genom geladen wurde und mindestens eine Analyse existiert
    bool isInitialized() const;

private:
    const Genome* genome = nullptr;                 // Referenz auf das Genom
    std::vector<AnalysisResult> analyses;           // Liste der durchgeführten Analysen
};
