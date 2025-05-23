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

//Enum class for assignment of Genotype Status
enum class GenotypeStatus {
    HomozygousDominant,
    Heterozygous,
    HomozygousRecessive
};


 class SNP
 {
    //Konstruktor & Destruktor
    public:
        SNP(const std::string& id, const std::string& chr, int pos, char a1, char a2);
        ~SNP() = default;  

    // no unknown SNPs and no cloning of SNP
    public:
        SNP() = delete;                                   // default constructor
        SNP(const SNP &i_rhs) = default;                   // copy constructor
        SNP &operator=(const SNP &i_rhs) = delete;        // assignment operator

    //Getter & Setter
    public:
        std::string getGenotype() const;
        GenotypeStatus getGenotypeStatus() const;
        std::string getRSID() const;

    //Member Variablen
    private:
        std::string m_rsID;
        std::string m_chromosome;
        int m_position;
        char m_allele1;
        char m_allele2;
 };
    // Prototyp einer otside Class Function
 std::string genotypeStatusToString(GenotypeStatus status);
