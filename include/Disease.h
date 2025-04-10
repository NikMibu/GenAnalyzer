#pragma once

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
 * - class CancerDisease : public Disease {}
 * - class MCASDisease : public Disease {}
 * 
 */

enum class RiskLevel {
    Low,
    Medium,
    High
};


 class Disease
 {

 };

