#pragma once

#include <vector>
#include <map>
#include "SNP.h"

/**
 * Enthällt alle SNPs einer Person (aus Datei gelesen)
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
        Genome() = default;
        Genome(const std::string& sampleID);
    
        void loadFromFile(const std::string& filename, size_t maxLines = 0);


        size_t getSNPCount() const;
        const SNP* getSNPByID(const std::string& rsID) const;
        bool hasSNP(const std::string& rsID) const;
        void addSNP(const SNP& snp);

        void setSampleID(const std::string& id);
        std::string getSampleID() const;

        void printSummary() const;
    
    private:
        std::vector<SNP> m_snps;
        std::map<std::string, size_t> m_indexByID;
        std::string m_sampleID;
        std::string m_sourceFile;
    };