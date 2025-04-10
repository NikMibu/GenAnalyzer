#pragma once

/**
 * Enthält die krankheitsrelevant SNPs + Risiko-Genotypen
 * 
 * Attribute: 
 * - std::string name
 * - std::map<std::string
 * - std::string> riskSNPs (z. B. `{ "rs123": "AA", "rs456": "GG" }`)
 * 
 * Funktionen: 
 * - bool isAtRisk(const Genome& genome) const
 * 
 */