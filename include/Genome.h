#pragma once

#include <vector>
#include <map>
#include "SNP.h"

/**
 * Die Klasse "Genome" repräsentiert das genetische Profil einer Person.
 * Sie enthält eine Sammlung von SNPs einer Person (aus Datei gelesen) sowie Metainformationen zur Probe.
 * 
 * Attribute: std::vector<SNP> snps
 * 
 * Funktionen:
 * - void loadFromFile string& filename
 * - const SNP* getSNPByID string& rsID const 
 * 
 */

class Genome {
    public:
        Genome() = default; // Default-Konstruktor
        Genome(const std::string& sampleID); // Konstruktor mit Sample-ID
    
        // Lädt SNP-Daten aus einer Datei (z. B. AncestryDNA.txt)
        // Optional: maxLines begrenzt die Anzahl der gelesenen SNPs
        void loadFromFile(const std::string& filename, size_t maxLines = 0);
    
        // Gibt die Anzahl gespeicherter SNPs zurück
        size_t getSNPCount() const;
    
        // Liefert einen Zeiger auf einen SNP anhand der rsID (oder nullptr)
        const SNP* getSNPByID(const std::string& rsID) const;
    
        // Prüft, ob ein SNP mit dieser rsID existiert
        bool hasSNP(const std::string& rsID) const;
    
        // Fügt einen neuen SNP zum Genom hinzu
        void addSNP(const SNP& snp);
    
        // Setzt bzw. gibt die Sample-ID
        void setSampleID(const std::string& id);
        std::string getSampleID() const;
    
        // Gibt eine Zusammenfassung im Terminal aus
        void printSummary() const;
    
    private:
        std::vector<SNP> m_snps;                      // Liste aller SNPs
        std::map<std::string, size_t> m_indexByID;    // Optional: Index zur schnellen rsID-Suche
        std::string m_sampleID;                       // Probenbezeichnung (z. B. "DemoSample")
        std::string m_sourceFile;                     // Ursprungsdatei (optional verwendbar)
    };
    