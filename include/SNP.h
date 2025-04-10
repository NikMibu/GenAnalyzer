#pragma once

#include <string>
#include <vector>

/**
 * Representiert einen einzelnen SNP (aus der Datei), z.B.:
 * 
 * Attribute:
 * - std::string rsID
 * - std::string chromosome
 * - int position
 * - char allele1
 * - char allele2
 * 
 * Funktionen: 
 * - Konstruktor 
 * - Getter/Setter
 * - std::string getGenotype -> z.b. "AG"
 */

enum class GenotypeStatus {
    HomozygousDominant,
    Heterozygous,
    HomozygousRecessive
};


 class SNP
 {
    //Konstruktor & Destructor
    public:
        SNP(const std::string& id, const std::string& chr, int pos, char a1, char a2);
        ~SNP() = default;  

    // no unknown SNPs and no cloning of SNP
    public:
        SNP() = delete;                                   // default constructor
        SNP(const SNP &i_rhs) = delete;                   // copy constructor
        SNP &operator=(const SNP &i_rhs) = delete;        // assignment operator

    //Getter & Setter
    public:
        std::string getGenotype() const;
        GenotypeStatus getGenotypeStatus() const;

    private:
        std::string m_rsID;
        std::string m_chromosome;
        int m_position;
        char m_allele1;
        char m_allele2;
 };

 std::string genotypeStatusToString(GenotypeStatus status);
