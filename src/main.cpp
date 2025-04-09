#include <iostream>

int main() {
    try
    {
        std::cout << "🧬 Willkommen bei GenAnalyzer!" << std::endl;

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