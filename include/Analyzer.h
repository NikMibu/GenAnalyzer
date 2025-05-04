#pragma once
#include "Genome.h"
#include "Disease.h"

#pragma once

#include "Genome.h"
#include "Disease.h"

/**
 * Die Klasse Analyzer führt die Analyse eines genetischen Profils (Genome)
 * im Vergleich zu einer Krankheitsbeschreibung (Disease) durch.
 *
 * Hinweis:
 * Die Klassifizierung basiert auf einer einfachen Heuristik und stellt keine medizinische Bewertung dar.
 * Es fehlen populationsbezogene Referenzdaten, statistische Validierung und klinische Aussagekraft.
 *
 * Hauptfunktionen:
 * - Analyse eines Genoms in Bezug auf eine Krankheit
 * - Ausgabe und Speicherung der Treffer
 * - Berechnung eines einfachen Risiko-Scores
 * - Risiko-Einstufung (Low / Medium / High)
 *
 * Mögliche Erweiterungen:
 * - Analyse mehrerer Krankheiten in einer Session
 * - Gruppierung nach Genen
 * - Hervorhebung besonders kritischer SNPs
 */

// Einfache Einteilung des genetischen Risikos basierend auf dem Score
enum class RiskLevel {
    Low,
    Medium,
    High
};

class Analyzer {
public:
    // Startet die Analyse eines Genoms im Vergleich zur Disease-SNP-Liste
    void runAnalysis(const Genome& genome, const Disease& disease);

    // Speichert die Analyseergebnisse in eine Textdatei
    void saveResults(const std::string& filename) const;

    // Gibt eine Zusammenfassung aller Treffer im Terminal aus
    void printSummary() const;

    // Berechnet einen einfachen Score (z. B. 1 Punkt für heterozygot, 2 für homozygot)
    int calculateRiskScore() const;

    // Wandelt einen numerischen Score in eine Risiko-Einstufung um
    RiskLevel getRiskLevel(int score) const;

    // Gibt den Risiko-Level als String zurück
    static std::string riskLevelToString(RiskLevel level);

    bool isInitialized() const;

private:
    const Genome* genome = nullptr;                // Referenz auf das untersuchte Genom
    const Disease* disease = nullptr;              // Referenz auf die betrachtete Krankheit

    std::vector<const SNP*> matchedSNPs;           // Liste gefundener SNPs im Genom
    std::vector<DiseaseSNP> matchedDiseaseSNPs;    // Zuordnung zu Risiko-SNPs aus Disease
};

