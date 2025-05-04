# GenAnalyzer – C++ SNP-Risikoanalyse

GenAnalyzer ist ein objektorientiertes C++-Projekt zur Analyse genetischer Rohdaten – beispielsweise aus AncestryDNA-Textdateien.

Bei privaten Gentests wie AncestryDNA werden sogenannte SNPs (Single Nucleotide Polymorphisms) – also Einzelbasenvarianten im Genom – analysiert und als Rohdaten an die getestete Person zurückgegeben. Diese SNPs können theoretisch genutzt werden, um genetische Prädispositionen für Krankheiten zu erkennen oder Lebensstilentscheidungen zu unterstützen (z. B. Ernährung, Mikronährstoffe, Entgiftungswege).

GenAnalyzer automatisiert diesen Vergleich:
Es liest ein persönliches SNP-Profil ein und vergleicht es mit einer vordefinierten Liste risikobehafteter Varianten (z. B. für MCAS, Methylierungsstörungen, Krebs). Auffällige Genotypen werden samt zugehöriger Funktion und einer einfachen Risikoeinschätzung ausgegeben.

---

## Aufbau & Funktion

- Dateien einlesen: AncestryDNA-Rohdaten (`.txt`) und Risiko-SNP-Tabellen (`.tsv`)
- Analyse: Vergleich jedes SNPs im Genom mit bekannten Risikovarianten
- Risikobewertung: Einfache Punktwertung (1 Punkt = heterozygot, 2 Punkte = homozygot)
- Ausgabe: Terminal-Zusammenfassung & Text-Export

### Beispielausgabe:

```
Risiko-Score: 5 → Mäßig erhöht
rs1801133   AG   Heterozygot   MTHFR   Methylierung (C677T)
rs4680      AA   Homozygot     COMT    Dopaminabbau (Val/Met)
```

---

## Projektstruktur

```
GenAnalyzer/
├── src/                # main.cpp
├── lib/                # Implementierungen (SNP, Genome, Analyzer, Disease)
├── include/            # Header-Dateien
├── data/               # Beispiel-SNP-Daten (MCAS_snps.tsv etc.)
├── build/              # (von CMake generiert)
├── CMakeLists.txt
└── README.md
```

---

## Hinweis zur Risikobewertung

Die im Projekt verwendete Bewertung ist eine vereinfachte Heuristik und dient nur zu Demonstrationszwecken – keine medizinische Risikoeinschätzung.

Sie basiert lose auf:
>[Study: "Population-standardized genetic risk score"](https://pmc.ncbi.nlm.nih.gov/articles/PMC4955173/)  
>GRS-RAC-Modell
>(Genetic Risk Score – Risk Allele Count)

>RR = 2 Punkte → homozygot risikobehaftet (zwei Risikoallele)
>RN = 1 Punkt → heterozygot (ein Risiko-, ein Normalallel)
>NN = 0 Punkte → homozygot normal (keine Risikoallele)

Die Summe aller Punkte ergibt den individuellen Risikoscore, der in diesem Projekt in drei Klassen eingeteilt wird: Gering, Mäßig erhöht, Hoch.
Die Verteilung der Risikoallele, ihre Populationshäufigkeit sowie Interaktionen mit anderen Genen werden nicht berücksichtigt.

---

## Kompilieren & Ausführen

```bash
mkdir build
cd build
cmake ..
make
cd ..
.\build\GenAnalyzer
```
Hier gibt es leider Probleme wenn man im Build Ordner startet und es kann nicht richtig auf data zugegriffen werden.
Deshalb in das Überverzeichnis wechseln und mit .\build\GenAnalyzer starten
---

## Abhängigkeiten

- C++17
- CMake ≥ 3.10
- MSYS2 / GCC oder Visual Studio Code mit CMake Tools

---

## Funktionen
Die Funktionen werden in der jewiligen Klasse beschrieben und im main eine Beispiel Analyse ausgeführt und ein Results Demo Datei erstellt.

---

## Erweiterungsmöglichkeiten

- Analyse mehrerer Krankheiten gleichzeitig
- CSV/HTML-Ausgabe
- Erweiterung des Genome-Modells mit Personendaten (Alter, BMI, Lebensstil)
- RiskAnalyzer v2: gewichtete Risikoallele, bessere Visualisierung
- Terminal-UI mit Menüführung

---

**Autor**: Niklas Mitterbuchner
**Projekt für:** C++ Abgabeprojekt SS  
