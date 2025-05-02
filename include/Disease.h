#pragma once
#include <string>
#include <vector>

/**
 * Enthält die krankheitsrelevant SNPs + Risiko-Genotypen
 * 
 * Attribute: 
 * - std::string name
 * - std::map<std::string,std::string> riskSNPs (z. B. `{ "rs123": "AA", "rs456": "GG" }`) - "rsID", "Genotype"
 * 
 * Funktionen: 
 * - bool isAtRisk(const Genome& genome) const
 * 
 * Mögliche Erweiterung:
 * Unterklassen für jede Krankheit -> in eigene Dateien z.b. CancerDisease.h / CancerDisease.cpp
 * - class CancerDisease : public Disease {}
 * - class MCASDisease : public Disease {}
 * 
 */


// Repräsentiert eine einzelne krankheitsassoziierte SNP
class DiseaseSNP {
    public:
        std::string rsID;      // SNP-Bezeichner (z. B. rs1801133)
        std::string gene;      // Zugehöriges Gen (z. B. MTHFR)
        std::string function;  // Beschreibung der Funktion oder Auswirkung
    
        // Konstruktor zur Initialisierung eines DiseaseSNP-Eintrags
        DiseaseSNP(const std::string& rs, const std::string& g, const std::string& f)
            : rsID(rs), gene(g), function(f) {}
    };
    
// Repräsentiert eine genetisch mit SNPs assoziierte Krankheit
class Disease {
    public:
        // Konstruktor mit Krankheitsnamen (z. B. "MCAS", "Cancer")
        Disease(const std::string& name);
        
        // Gibt den Namen der Krankheit zurück
        const std::string& getName() const;
        
        // Liefert die Liste aller mit dieser Krankheit assoziierten SNPs
        const std::vector<DiseaseSNP>& getRiskSNPs() const;
        
        // Lädt Risiko-SNPs aus einer TSV-Datei (z. B. MCAS_snps.tsv)
        void loadRiskSNPsFromFile(const std::string& filename);
        
        // Gibt alle geladenen SNPs auf der Konsole aus (Debug- oder Info-Zweck)
        void printRiskSNPs() const;
    
    private:
        std::string name;                        // Name der Krankheit
        std::vector<DiseaseSNP> riskSNPs;        // Liste der relevanten SNPs für diese Krankheit
    };
    

