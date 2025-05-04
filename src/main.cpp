#include <iostream>
#include <vector>
#include <filesystem>

#include "SNP.h"
#include "Genome.h"
#include "Disease.h"
#include "Analyzer.h"

/**
 * GenAnalyzer – Hauptprogramm
 * 
 * Dieses Tool dient der explorativen Analyse genetischer Rohdaten (z. B. aus AncestryDNA),
 * um auffällige SNPs in Bezug auf definierte Krankheitsprofile zu identifizieren.
 * Die Auswertung basiert auf Risiko-SNP-Tabellen und ist nicht medizinisch validiert.
 *
 * ──────────────────────────────────────────────────────────────
 * Beispielstruktur einer AncestryDNA-Rohdatei:
 * (Tabulatorgetrennte Datei mit sehr vielen Zeilen)
 * 
 * rsid          chromosome   position   allele1   allele2
 * rs3131972     1            752721     A         G
 * rs11240777    1            798959     A         G
 * rs1801133     1            11856378   A         G
 * ...
 *
 * → Die Datei enthält typischerweise mehrere hunderttausend SNPs.
 * → GenAnalyzer extrahiert und prüft davon nur diejenigen, die als risikorelevant definiert wurden.
 * ──────────────────────────────────────────────────────────────
 */

 /**
 * To-Do:
 * ✔ Fehlerbehandlung
 * ✔ Objektstruktur mit Klassen
 * ✔ Genome-Import
 * ✔ Disease-Liste importieren
 * ✔ Analysefunktion (v1)
 *
 * Geplante Erweiterungen:
 * ⭕ Erweiterung des Genome-Modells mit Personendaten (Alter, BMI, Lebensstil)
 * ⭕ RiskAnalyzer v2: gewichtete Risikoallele, bessere Visualisierung
 * ⭕ Terminal-UI mit Menüführung
 */

/** Erklärung zum UI-Teil:
 * Dieses Menü stellt eine einfache, terminalbasierte Benutzeroberfläche dar,
 * mit der der Nutzer die Analyse-Schritte manuell ausführen kann.
 * Der Workflow folgt einer typischen Pipeline:
 * [1] SNP-Daten einlesen → [2] Krankheits-SNPs laden → [3] Analyse durchführen
 * Anschließend kann der Nutzer die Ergebnisse anzeigen oder speichern.
 * 
 * In dieser Demo-Version sind Genome-Datei und Krankheit vordefiniert, bzw. kann ausgewählt werden, MCAS - default 
 * -> Momentan kann man noch nicht mehrere Krankheiten auf ein mal Analysieren oder die Analyse von vorne starten 
 * 
 **/


int main() {
    try {
        std::cout << "🧬 Willkommen bei GenAnalyzer!" << std::endl;

        Genome genome("DemoSample");
        Disease disease("MCAS"); 
        Analyzer analyzer;

        bool running = true;
        while (running) {
            std::cout << "\n===== Menü =====\n";
            std::cout << "\nBitte führen Sie diese Analyse in der richtigen Reihenfolge durch\n";
            std::cout << "[1] Genome-Datei laden\n";
            std::cout << "[2] Krankheit laden \n";
            std::cout << "[3] Analyse starten\n";
            std::cout << "[4] Ergebnisse anzeigen\n";
            std::cout << "[5] Ergebnisse speichern\n";
            std::cout << "[6] Beenden\n";
            std::cout << "Eingabe: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1:
                    genome.loadFromFile("data/seqs/AncestryDNA.txt");
                    std::cout << "✅ SNPs geladen: " << genome.getSNPCount() << std::endl;
                    break;
                case 2: {
                    std::cout << "\nWelche Krankheit möchtest du laden?\n";
                    std::cout << "[1] MCAS\n";
                    std::cout << "[2] Cancer\n";
                    std::cout << "[3] Gilbert-Syndrom\n";
                    std::cout << "Eingabe: ";
                        
                    int diseaseChoice;
                    std::cin >> diseaseChoice;
                    
                    switch (diseaseChoice) {
                        case 1:
                            disease = Disease("MCAS");
                            disease.loadRiskSNPsFromFile("data/disease/MCAS_snps.tsv");
                            break;
                        case 2:
                            disease = Disease("Cancer");
                            disease.loadRiskSNPsFromFile("data/disease/CancerGenes_snps.tsv");
                            break;
                        case 3:
                            disease = Disease("Gilbert-Syndrom");
                            disease.loadRiskSNPsFromFile("data/disease/Gilbert_Syndrom_snps.tsv");
                            break;
                        default:
                            std::cout << "⚠ Ungültige Auswahl – bitte 1–3 eingeben." << std::endl;
                            break;
                    }
                    
                    std::cout << "✅ " << disease.getName() << "-Risiko-SNPs geladen: "
                                << disease.getRiskSNPs().size() << std::endl;
                    break;
                }
                case 3:
                    analyzer.runAnalysis(genome, disease);
                    std::cout << "🔬 Analyse abgeschlossen." << std::endl;
                    break;
                case 4:{
                    if (!analyzer.isInitialized()) {
                        std::cout << "⚠ Bitte zuerst eine Analyse durchführen (Option 3)." << std::endl;
                        break;
                    }
                    
                    analyzer.printSummary();
                    break;
                }
                case 5:{
                    if (!analyzer.isInitialized()) {
                        std::cout << "⚠ Bitte zuerst eine Analyse durchführen (Option 3)." << std::endl;
                        break;
                    }                    
                    std::string filename = "data/output/" + genome.getSampleID() + "_" + disease.getName() + "_results.txt";
                    analyzer.saveResults(filename);
                    std::cout << "💾 Ergebnisse gespeichert unter: " << filename << std::endl;
                    break;
                }
                case 6:
                    running = false;
                    std::cout << "Programm beendet. Vielen Dank fürs Nutzen von GenAnalyzer." << std::endl;
                    break;
                default:
                    std::cout << "⚠ Ungültige Eingabe. Bitte 1–6 eingeben." << std::endl;
            }
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fehler: " << e.what() << std::endl;
        return -1;
    }
}
