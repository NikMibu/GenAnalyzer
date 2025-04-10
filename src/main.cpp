#include <iostream>
#include <vector>

#include "../include/SNP.h"

/** 
 * Beispiel Datei, Ancestry Sequencing:
 * 
 * rsid chromosome	position allele1 allele2
 * rs3131972	1	752721	A	G
 * rs114525117	1	759036	G	G
 * rs4040617	1	779322	A	G
 * rs141175086	1	780397	C	C
 * rs115093905	1	787173	T	G
 * rs11240777	1	798959	A	G
 * rs6681049	1	800007	C	C
 * ......
 * mehrere 100.000 Einträge pro Sequencing einer Person -> wird in Genome gespeichert 
*/

/**
 * To-Do:
 * - kleines Text / Terminal basiertes UI
 * - verschiedene Error Handlings
 * - Genome einlesen 
 * - Disease einlesen
 * - Analyzer
 * - Output : Histogramm of Risk associated to that disease bacause of that SNPs and Genotypes 
 */


int main() {
    try
    {
        std::cout << "🧬 Willkommen bei GenAnalyzer!" << std::endl;

        // Tests:
        SNP snp("rs123456", "1", 123456, 'T', 'A');
        std::cout << "Genotype: " << snp.getGenotype() << ", Status: " << genotypeStatusToString(snp.getGenotypeStatus()) << std::endl;


        // Schritt 1: Genome einlesen
        // Schritt 2: Disease-Liste einlesen
        // Schritt 3: Analyse starten
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}