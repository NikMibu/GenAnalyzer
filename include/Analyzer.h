#pragma once

/**
 * Macht die eigentliche Analyse
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
 */

 class Analyzer
 {

 };
