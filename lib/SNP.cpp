#include "SNP.h"

SNP::SNP(const std::string& id, const std::string& chr, int pos, char a1, char a2)
    : m_rsID(id), m_chromosome(chr), m_position(pos), m_allele1(a1), m_allele2(a2)
{}
//Gibt Genotyp zurück
std::string SNP::getGenotype() const
{
    return std::string(1, m_allele1) + std::string(1, m_allele2);
}
//Gibt Genotype mit Enum Class zurück
GenotypeStatus SNP::getGenotypeStatus() const
{
    if (m_allele1 == m_allele2) {
        if (m_allele1 == 'A') {
            return GenotypeStatus::HomozygousDominant;
        } else {
            return GenotypeStatus::HomozygousRecessive;
        }
    } else {
        return GenotypeStatus::Heterozygous;
    }
}
// konvertiert Genotype Status von Enum Class zu String 
std::string genotypeStatusToString(GenotypeStatus status) {
    switch (status) {
        case GenotypeStatus::HomozygousDominant:
            return "Homozygous Dominant";
        case GenotypeStatus::Heterozygous:
            return "Heterozygous";
        case GenotypeStatus::HomozygousRecessive:
            return "Homozygous Recessive";
        default:
            return "Unknown";
    }
}
// gibt RSID des SNPs zurück
std::string SNP::getRSID() const {
    return m_rsID;
}

