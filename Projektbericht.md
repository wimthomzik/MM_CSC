<!--# Überschrift 1
## Überschrift 2
### Überschrift 3
#### Überschrift 4

**Fett** oder __Fett__
*Kursiv* oder _Kursiv_
~~Durchgestrichen~~

- Punkt 1
- Punkt 2
    - Unterpunkt 2.1
    - Unterpunkt 2.2
- Punkt 3

1. Punkt 1
2. Punkt 2
    1. Unterpunkt 2.1
    2. Unterpunkt 2.2
3. Punkt 3
   
[Linktext](http://example.com)

![Alt-Text](http://example.com/bild.jpg)

Dies ist ein `Inline-Code` Beispiel.


| Spalte 1 | Spalte 2 |
|----------|----------|
| Inhalt 1 | Inhalt 2 |
| Inhalt 3 | Inhalt 4 |


---
-->

# Projektbericht

## 1. Einführung

## 1.1 Motivation
Das Projekt zur Multiplikation dünnbesetzter Matrizen im CSC-Format wurde uns zugeteilt, um ein tiefgehendes Verständnis für effiziente Algorithmen 
und Datenstrukturen im Bereich der linearen Algebra zu erlangen. Traditionelle Methoden zur Matrizenmultiplikation sind bei dünnbesetzten Matrizen, 
die eine große Anzahl von Null-Einträgen aufweisen, ineffizient und ressourcenintensiv. Diese Ineffizienz entsteht durch unnötige Berechnungen und Speicherplatzverschwendung. 
Durch die Implementierung des CSC-Formats werden diese Probleme adressiert, indem nur die relevanten Einträge gespeichert und verarbeitet werden. 
Dieses Projekt hat Relevanz im größeren Kontext, da dünnbesetzte Matrizen in vielen Bereichen wie wissenschaftlichen Berechnungen, maschinellem Lernen und Computergraphik weit verbreitet sind. 
Eine effiziente Verarbeitung solcher Matrizen kann somit die Performance und Ressourcenoptimierung in zahlreichen Anwendungen erheblich verbessern.

## 1.2 Zielsetzung
Das Hauptziel dieses Projekts ist die Entwicklung eines Programms, das die Multiplikation dünnbesetzter Matrizen im CSC-Format effizient durchführt. Die erwarteten Ergebnisse umfassen:

- Eine Implementierung der Matrizenmultiplikation im CSC-Format in C nach dem C17-Standard.
- Ein Rahmenprogramm, das Eingabe- und Ausgabedateien im spezifischen Textformat verarbeitet.
- Eine wissenschaftliche Bewertung der Performance der Implementierung anhand von Matrizen unterschiedlicher Dichte.
- Die Bereitstellung von Ergebnissen und Performanzdaten in grafischer Form zur besseren Veranschaulichung.
- Die Sicherstellung der Korrektheit, Genauigkeit und Effizienz der Implementierung auch bei Randfällen.

## 2. Implementierung

### 2.1 Überblick

Das Programm zur Multiplikation dünnbesetzter Matrizen im CSC-Format ist in verschiedene Hauptkomponenten unterteilt, die jeweils spezifische Aufgaben erfüllen. Die Struktur und der Aufbau des Programms sind darauf ausgelegt, die Modularität und Wartbarkeit zu maximieren. Hier ist ein allgemeiner Überblick über die Hauptkomponenten und deren Interaktionen:

### Hauptkomponenten:

1. **Implementierungsdateien**
    - **csc_matrix.h**: Enthält die Definitionen der grundlegenden Datenstrukturen für die Arbeit mit CSC-Matrizen.
    - **csc_matrix_mult_V0.c, csc_matrix_mult_V0.h**: Die Hauptimplementierung der Multiplikation von CSC-Matrizen. Enthält die Kernlogik für die Berechnung.
    - **csc_matrix_mult_V1.c, csc_matrix_mult_V1.h**: Eine alternative Implementierung der Multiplikation, die verschiedene Optimierungen oder Ansätze beinhaltet (siehe unten).
    - **csc_matrix_mult_V2.c, csc_matrix_mult_V2.h**: Eine alternative Implementierung der Multiplikation, die verschiedene Optimierungen oder Ansätze beinhaltet (siehe unten).
    - **csc_matrix_mult_V3.c, csc_matrix_mult_V3.h**: Eine alternative Implementierung der Multiplikation, die verschiedene Optimierungen oder Ansätze beinhaltet (siehe unten).
    - **csc_matrix_printer.c, csc_matrix_printer.h**: Funktionen zum Ausdrucken von Matrizen im CSC-Format.
    - **csc_matrix_reader.c, csc_matrix_reader.h**: Funktionen zum Einlesen von Matrizen aus Textdateien im vorgegebenen Format.
    - **csc_matrix_writer.c, csc_matrix_writer.h**: Funktionen zum Schreiben von Matrizen in das vorgegebenen Format.
    - **main.c**: Das Hauptprogramm, das die Nutzereingaben von dem Command Line Interface einliest und die verschiedenen Komponenten zusammenführt und die Programmlogik steuert.

2. **Test- und Hilfsskripte**
    - **exampleMachine.py**: Ein Skript was beliebig viele Beispiele testet.
    - **generator.py**: Skript zur Generierung von Testmatrizen, die als Eingabedateien für das Programm dienen.
    - **tester.py**: Skript zur automatisierten Testung der Implementierung und Validierung der Ergebnisse.

3. **Konfigurations- und Hilfsdateien**
    - **Makefile**: Automatisiert den Kompilierungsprozess der C-Implementierungen.
    - **justfile**: Automatisiert Build-Prozess der verschiedenen Versionen mit beliebigen Parametern und ermöglicht zentrale Steuerung der Python Umgebung.
    - **requirements.txt**: Listet die Python-Abhängigkeiten für die Skripte und die virtuelle Umgebung auf.
    - **.gitignore**: Definiert, welche Dateien und Verzeichnisse von der Versionskontrolle ausgeschlossen werden sollen.
    - 
### Interaktion der Komponenten:

- **Hauptprogramm (main.c)**: Dies ist das zentrale Steuerelement des Programms. Es liest die Nutzereingaben, lädt die Matrizen aus den Eingabedateien, ruft die Implementierungen der Matrizenmultiplikation auf und speichert die Ergebnisse in Ausgabedateien. Es verwendet die I/O-Funktionen, um die Matrizen zu laden und zu speichern, und ruft die Implementierungsdateien auf, um die Multiplikation durchzuführen. Die Implementierungsdatein können mit beliebiger Wiederholungszahl gebenchmarked werden.
- **Implementierungsdateien**: Die verschiedenen Implementierungen der Matrizenmultiplikation (`csc_matrix_mult_V0.c`, `csc_matrix_mult_V1.c`, etc.) enthalten die Logik zur Durchführung der Multiplikation. Diese Implementierungen können durch das Hauptprogramm je nach Bedarf aufgerufen werden.
- **I/O-Funktionen**: Die `csc_matrix_reader` und `csc_matrix_writer` Dateien enthalten Funktionen zum Einlesen und Schreiben der Matrizen, welche vom Hauptprogramm genutzt werden, um die Matrizen zu laden und die Ergebnisse zu speichern.
- **Hilfsskripte**: Die Skripte `generator.py` und `tester.py` unterstützen die Entwicklung und Testung, indem sie Testmatrizen generieren und die Ergebnisse der Implementierungen validieren.

### 2.2 Detaillierte Beschreibung der Implementierung

#### CSC-Matrix

Die CSC-Matrixstruktur besteht aus drei Arrays: `values`, `row_indices` und `col_ptrs`. `values` enthält die Werte der Matrix, `row_indices` speichert die Zeilenindizes der Werte und `col_ptrs` zeigt auf den Anfang jeder Spalte. Diese Struktur ermöglicht eine effiziente Verarbeitung von dünnbesetzten Matrizen, da nur die relevanten Werte gespeichert und verarbeitet werden. Die Implementierung der CSC-Matrixstruktur ist in `csc_matrix.h` definiert. Zusätzlich wurden noch col_ptr_length und nnz (number of non-zero elements) hinzugefügt, um die Länge der col_ptrs und die Anzahl der Nicht-Null-Elemente zu speichern, um Schleifendurchläufe zu minimieren.

#### main.c


### 2.3 Codebeispiele
Füge einige relevante Codeausschnitte hinzu, die wichtige Aspekte deiner Implementierung veranschaulichen.

## 3. Optimierungen

### 3.1 Überblick
Erkläre, welche Optimierungen vorgenommen wurden, um die Leistung und Effizienz des Programms zu verbessern.

### 3.2 Detaillierte Optimierungen
Beschreibe die spezifischen Optimierungstechniken, die angewendet wurden. Warum wurden diese gewählt und wie wurden sie implementiert?

### 3.3 Ergebnisse der Optimierungen
Zeige, wie sich die Optimierungen auf die Performanz des Programms ausgewirkt haben. Dies könnte durch Vergleichstabellen oder Grafiken verdeutlicht werden.

## 4. Testergebnisse

### 4.1 Teststrategie
Erkläre, wie die Tests durchgeführt wurden. Welche Testmethoden und -tools wurden verwendet? Welche Testfälle wurden abgedeckt?

### 4.2 Ergebnisse
Präsentiere die Testergebnisse. Dies könnte in Form von Tabellen, Diagrammen oder Text erfolgen. Stelle sicher, dass du sowohl erfolgreiche Tests als auch eventuelle Fehler und deren Lösungen dokumentierst.

### 4.3 Randfälle
Beschreibe, wie das Programm mit Randfällen umgeht. Welche ungewöhnlichen oder extremen Bedingungen wurden getestet und wie hat das Programm darauf reagiert?

## 5. Performanz

### 5.1 Performanzanalyse
Analysiere die Leistung deines Programms. Welche Metriken wurden gemessen (z.B. Laufzeit, Speicherverbrauch)?

### 5.2 Ergebnisse der Performanztests
Präsentiere die Ergebnisse der Performanztests. Verwende Diagramme und Tabellen, um die Daten übersichtlich darzustellen.

### 5.3 Diskussion der Performanz
Diskutiere die Ergebnisse der Performanztests. Was bedeuten die Ergebnisse für die praktische Anwendung des Programms? Gibt es Bereiche, in denen die Performanz verbessert werden könnte?

## 6. Fazit und Ausblick

### 6.1 Zusammenfassung
Fasse die wichtigsten Punkte deines Berichts zusammen. Was wurde erreicht, welche Probleme wurden gelöst?

### 6.2 Ausblick
Gib einen Ausblick auf mögliche zukünftige Arbeiten. Welche Verbesserungen oder Erweiterungen könnten in zukünftigen Versionen des Programms vorgenommen werden?

