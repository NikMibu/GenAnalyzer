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