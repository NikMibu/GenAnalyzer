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
 * Unterklassen für jede Krankheit -> in eigene Dateien z.b. CancerDisease.h / CancerDisease.cpp
 * Baer erst später
 * - class CancerDisease : public Disease {}
 * - class MCASDisease : public Disease {}
 * 
 */


class DiseaseSNP {
    public:
        std::string rsID;
        std::string gene;
        std::string function;
    
        DiseaseSNP(const std::string& rs, const std::string& g, const std::string& f)
            : rsID(rs), gene(g), function(f) {}
};


class Disease {
    
    public:
        Disease(const std::string& name);
        void loadRiskSNPsFromFile(const std::string& filename);
        const std::string& getName() const;
        const std::vector<DiseaseSNP>& getRiskSNPs() const;
    
        void printRiskSNPs() const;

    private:
        std::string name;
        std::vector<DiseaseSNP> riskSNPs;
};

