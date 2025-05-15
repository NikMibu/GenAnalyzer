#include <iostream>
#include <vector>
#include <filesystem>
#include <ctime>

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
 * ⭕ Terminal-UI mit guter Menüführung
 */

/** Erklärung zum UI-Teil:
 * Dieses Menü stellt eine einfache, terminalbasierte Benutzeroberfläche dar,
 * mit der der Nutzer die Analyse-Schritte manuell ausführen kann.
 * Der Workflow folgt einer typischen Pipeline:
 * [1] SNP-Daten einlesen → [2] Krankheits-SNPs laden → [3] Analyse durchführen
 * Anschließend kann der Nutzer die Ergebnisse anzeigen, speichern oder auch mehrere Krankheiten laden 
 * und als gesammelten Report speichern.
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
            std::cout << "\nBitte fuehren Sie diese Analyse in der richtigen Reihenfolge durch\n";
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
                    // Genom wird von File geladen 
                    genome.loadFromFile("data/seqs/AncestryDNA.txt");
                    std::cout << "✅ SNPs geladen: " << genome.getSNPCount() << std::endl;
                    break;
                case 2: {
                    namespace fs = std::filesystem;

                    // Liste zur Speicherung gefundener Krankheitsdateien (.tsv)
                    std::vector<fs::directory_entry> diseaseFiles;

                    // Verzeichnis, in dem Krankheitsdaten gesucht werden
                    std::string diseaseDir = "data/disease";

                    // Durchsuche das Verzeichnis nach Dateien mit der Endung ".tsv"
                    for (const auto& entry : fs::directory_iterator(diseaseDir)) {
                        if (entry.path().extension() == ".tsv") {
                            diseaseFiles.push_back(entry);  // Speichere gültige Datei
                        }
                    }

                    // Keine passenden Dateien gefunden?
                    if (diseaseFiles.empty()) {
                        std::cout << "Keine Krankheitsdateien gefunden!" << std::endl;
                        break;
                    }

                    // Zeige dem Benutzer eine Liste aller gefundenen Krankheiten
                    std::cout << "\nWelche Krankheit moechtest du laden?\n";
                    for (size_t i = 0; i < diseaseFiles.size(); ++i) {
                        // .stem() entfernt die Dateiendung, z. B. "MCAS.tsv" → "MCAS_snps"
                        std::cout << "[" << (i + 1) << "] " << diseaseFiles[i].path().stem().string() << std::endl;
                    }

                    std::cout << "Eingabe: ";
                    size_t choice;
                    std::cin >> choice;

                    // Ungültige Eingabe?
                    if (choice < 1 || choice > diseaseFiles.size()) {
                        std::cout << "Ungueltige Auswahl – bitte eine gültige Zahl eingeben." << std::endl;
                        break;
                    }

                    // Gewählte Datei und Krankheitsname extrahieren
                    fs::path filepath = diseaseFiles[choice - 1].path();
                    std::string diseaseName = filepath.stem().string();  // z. B. "MCAS_snps"

                    // Krankheit laden
                    disease = Disease(diseaseName);
                    disease.loadRiskSNPsFromFile(filepath.string());

                    // Ausgabe: wie viele SNPs wurden geladen?
                    std::cout << "✅ " << disease.getName() << "-Risiko-SNPs geladen: "
                            << disease.getRiskSNPs().size() << std::endl;
                                    
                }
                case 3:
                    // Analyse wird gestartet
                    analyzer.runAnalysis(genome, disease);
                    std::cout << "Analyse abgeschlossen." << std::endl;
                    break;
                case 4:{
                    if (!analyzer.isInitialized()) {
                        std::cout << "Bitte zuerst eine Analyse durchfuehren (Option 3)." << std::endl;
                        break;
                    }
                    // Analyse kann vor dem speichern schonmal ausgegeben werden - so auf die Art Preview     
                    analyzer.printSummary();
                    break;
                }
                case 5:{
                    if (!analyzer.isInitialized()) {
                        std::cout << "Bitte zuerst eine Analyse durchfuehren (Option 3)." << std::endl;
                        break;
                    }
                    time_t now = time(0);
                    tm* localTime = localtime(&now);  // Lokale Zeitstruktur

                    // Erzeuge ein Format wie "16_04" (Tag_Monat)
                    char dateStr[20];
                    strftime(dateStr, sizeof(dateStr), "%d_%m_%H%M%S", localTime);

                    std::string filename = "data/output/" + genome.getSampleID() + "_results_" + dateStr + ".txt";

                    // Speichern der Results
                    analyzer.saveResults(filename);
                    std::cout << "Ergebnisse gespeichert unter: " << filename << std::endl;
                    break;
                }
                case 6:
                    running = false;
                    std::cout << "Programm beendet. Vielen Dank fuers Nutzen von GenAnalyzer." << std::endl;
                    break;
                default:
                    std::cout << "Ungueltige Eingabe. Bitte 1–6 eingeben." << std::endl;
            }
        }

        return 0;
    }
    catch (const std::exception& e) { // Fehler Handling
        std::cerr << "Fehler: " << e.what() << std::endl;
        return -1;
    }
}
