#include <iostream>
#include <vector>
#include <filesystem>


#include "SNP.h"
#include "Genome.h"
#include "Disease.h"
#include "Analyzer.h"

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
 * 
 * Dieses Tool ist kein diagnostisches Instrument, 
 * sondern dient der explorativen Identifikation von potenziell relevanten SNPs in einer genetischen Datei, 
 * basierend auf frei definierbaren Risiko-SNP-Listen, 
 * dient also eher zur Anschauung und zu Gunsten der Umsetzbarkeit in Bezug auf dieses Projekt
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
        /*  - Simple Tests von SNP und Genome Klasse
        SNP snp("rs123456", "1", 123456, 'T', 'A');
        std::cout << "Genotype: " << snp.getGenotype() << ", Status: " << genotypeStatusToString(snp.getGenotypeStatus()) << std::endl;

        Genome genome("TestSample");
        SNP snp1("rs123", "1", 123456, 'A', 'G');
        genome.addSNP(snp1);
        genome.addSNP(snp);

        const SNP* found = genome.getSNPByID("rs123");
        if (found) {
            std::cout << "✅ SNP gefunden: " << found->getRSID() << ", Genotyp: " << found->getGenotype() << std::endl;
        }
        std::cout << genome.getSNPCount() << genome.getSampleID() << genome.hasSNP("rs1235")<<std::endl;
        genome.printSummary();
        */

        // Schritt 1: Genome einlesen
        Genome genome("DemoSample");
        genome.loadFromFile("data/seqs/AncestryDNA.txt");
        std::cout << "Geladene SNPs: " << genome.getSNPCount() << std::endl;

        // Nach SNP mir rsID suchen 
        const SNP* found = genome.getSNPByID("rs7015180");
        if (found) {
            std::cout << "Gefunden: " << found->getRSID() << ", Genotyp: " << found->getGenotype() << ", Genotype Status: " << genotypeStatusToString(found->getGenotypeStatus())<<std::endl;
        }
        genome.printSummary();

        // Schritt 2: Disease-Liste einlesen
        Disease cancer("Cancer");
        cancer.loadRiskSNPsFromFile("data/disease/CancerGenes_snps.tsv");

        /*for (const auto& snp : cancer.getRiskSNPs()) {
            std::cout << snp.rsID << " → " << snp.function << std::endl;
        }*/
        cancer.printRiskSNPs();

        Disease mcas("MCAS");
        mcas.loadRiskSNPsFromFile("data/disease/MCAS_snps.tsv");
        

        // Schritt 3: Analyse starten
        Analyzer try_1;
        try_1.runAnalysis(genome,cancer);
        try_1.printSummary();

        Analyzer try_2;
        try_2.runAnalysis(genome,mcas);
        try_2.printSummary();
        try_2.saveResults("data/output/results_demo.txt");


        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}