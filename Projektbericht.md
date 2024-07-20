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

Die `main`-Methode ist wie folgt strukturiert:

1. **Einlesen der Kommandozeilenargumente**: Die Methode verwendet `getopt_long`, um die Kommandozeilenargumente zu parsen. Es werden Optionen wie Eingabedateien für die Matrizen, die Ausgabedatei, die Implementierungsversion und eine Benchmark-Option akzeptiert.

2. **Deklaration und Initialisierung von Variablen**: Es werden Variablen für die Eingabedateien, die Ausgabedatei, die Version der Implementierung und die Benchmark-Option initialisiert.

3. **Definition und Parsing der Optionen**: Mit `getopt_long` werden die Kommandozeilenargumente verarbeitet und den entsprechenden Variablen zugewiesen. Dabei wird überprüft, ob die Argumente gültig sind und ob alle notwendigen Dateien angegeben wurden.

4. **Validierung der Eingabedateien**: Die Methode stellt sicher, dass alle notwendigen Eingabedateien und die Ausgabedatei vorhanden sind. Falls eine Datei fehlt, wird eine Fehlermeldung ausgegeben und das Programm beendet.

5. **Initialisierung der Matrizen**: Es werden drei Matrizenstrukturen initialisiert: `matrixA`, `matrixB` und `resultMatrix`. Diese Strukturen speichern die Eingabematrizen und das Ergebnis der Multiplikation. Alle Felder der Strukturen werden mit `empty_matrix()` auf Null gesetzt.

6. **Einlesen der Matrizen**: Die Methode liest die Matrizen aus den angegebenen Eingabedateien ein. Hierfür wird die Funktion `readCSCMatrix` verwendet, die die Matrizen im CSC-Format aus den Dateien liest und in den entsprechenden Strukturen speichert.

7. **Benchmarking und Matrizenmultiplikation**: Falls die Benchmark-Option gesetzt ist, misst die Methode die Zeit für die Matrizenmultiplikation und führt diese mehrfach durch, um Durchschnittswerte zu berechnen. Abhängig von der angegebenen Implementierungsversion wird eine der verschiedenen Versionen der Multiplikationsfunktion aufgerufen.

8. **Schreiben der Ergebnis-Matrix**: Nach der Berechnung wird das Ergebnis in eine Ausgabedatei geschrieben. Dies erfolgt mit der Funktion `writeCSCMatrix`, die die resultierende Matrix im CSC-Format speichert.

9. **Speicherfreigabe**: Abschließend wird der für die Matrizen allokierte Speicher freigegeben, um Speicherlecks zu vermeiden. Hierzu wird eine spezielle `cleanup`-Funktion verwendet, die alle dynamisch allokierten Speicherbereiche überprüft und freigibt.

#### csc_matrix_reader.c

Die Datei `csc_matrix_reader.c` enthält Funktionen zum Einlesen von Matrizen im CSC-Format aus Textdateien.

1. **Enum DataType**: Definiert den Datentyp des Vektors (FLOAT oder INT).

2. **Funktion `readDynamicLine`**: Diese Funktion liest eine Zeile aus einer Datei und speichert die Werte in einem dynamisch allozierten Vektor. Sie ignoriert leere Zeilen sowie überflüssige Kommata und Leerzeichen. Die Funktion nimmt als Parameter einen Dateizeiger, einen Zeiger auf den Vektor, einen Zeiger auf die Größe des Vektors und den Datentyp des Vektors entgegen.

   - **Deklaration und Initialisierung von Variablen**: Zeiger auf die zu lesende Zeile, deren Größe und die Anzahl der gelesenen Zeichen werden initialisiert.
   - **Einlesen der Zeile**: Die Funktion `getline` liest eine Zeile aus der Datei und speichert sie in einem Buffer. Falls ein Fehler auftritt, wird eine Fehlermeldung ausgegeben und es wird `EXIT_FAILURE` zurückgegeben.
   - **Überprüfung auf leere Zeilen**: Leere Zeilen werden ignoriert, indem der Speicher freigegeben und `EXIT_FAILURE` zurückgegeben wird.
   - **Initialisierung des Vektors**: Speicher wird für den Vektor alloziert. Der Typ des Vektors hängt von der `DataType`-Enum ab (FLOAT oder INT). Falls die Allokation fehlschlägt, wird `EXIT_FAILURE` zurückgegeben.
   - **Tokenisierung der Zeile**: Die Zeile wird mit `strtok` in Tokens zerlegt, die durch Kommata, Leerzeichen, Tabulatoren und neue Zeilen getrennt sind.
   - **Verarbeitung der Tokens**: Jedes Token wird konvertiert (zu `float` oder `size_t`) und in den Vektor gespeichert. Falls nötig, wird der Speicher des Vektors neu alloziert. Bei ungültigen Tokens wird eine Fehlermeldung ausgegeben.
   - **Speichern der Größe des Vektors**: Die Anzahl der Elemente im Vektor wird in der übergebenen Größenvariablen gespeichert.
   - **Speicherfreigabe und Rückgabe**: Der Speicher für die gelesene Zeile wird freigegeben und `EXIT_SUCCESS` zurückgegeben, falls alle Tokens erfolgreich verarbeitet wurden.

3. **Funktion `readCSCMatrix`**: Diese Funktion liest eine CSC-Matrix aus einer Textdatei. Sie öffnet die Datei, liest die Dimensionen der Matrix, die Datenwerte, die Zeilenindizes und die Spaltenzeiger. Anschließend überprüft sie, ob die Anzahl der Zeilenindizes mit der Anzahl der Nicht-Null-Elemente übereinstimmt und ob keine zusätzlichen Daten nach den erwarteten Matrizenwerten vorhanden sind.

   - **Öffnen der Datei**: Die Funktion öffnet die Datei zum Lesen. Falls dies fehlschlägt, wird eine Fehlermeldung ausgegeben und `EXIT_FAILURE` zurückgegeben.
   - **Lesen der Dimensionen**: Die Funktion liest die erste Zeile der Datei, die die Dimensionen der Matrix enthält. Falls das Format ungültig ist, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Lesen der Datenwerte**: Die Funktion `readDynamicLine` wird aufgerufen, um die Datenwerte der Matrix zu lesen. Falls ein Fehler auftritt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Lesen der Zeilenindizes**: Die Funktion `readDynamicLine` wird aufgerufen, um die Zeilenindizes der Matrix zu lesen. Falls ein Fehler auftritt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Lesen der Spaltenzeiger**: Die Funktion `readDynamicLine` wird aufgerufen, um die Spaltenzeiger der Matrix zu lesen. Falls ein Fehler auftritt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Überprüfen der Datenkonsistenz**: Die Funktion überprüft, ob die Anzahl der Zeilenindizes mit der Anzahl der Nicht-Null-Elemente übereinstimmt. Falls nicht, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Überprüfen auf zusätzliche Daten**: Die Funktion überprüft, ob nach den erwarteten Matrizenwerten noch zusätzliche Daten in der Datei vorhanden sind. Falls ja, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Überprüfen der Spaltenzeiger**: Die Funktion überprüft, ob die Anzahl der Spaltenzeiger mit der Anzahl der Spalten übereinstimmt. Falls nicht, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.

4. **Fehlerbehandlung**

   - **Lesen der Zeile**: Falls `getline` fehlschlägt, wird eine Fehlermeldung ausgegeben und der Speicher für die Zeile wird freigegeben.
   - **Speicherallokation**: Falls die Allokation oder Realokation des Speichers für den Vektor fehlschlägt, wird eine Fehlermeldung ausgegeben und der Speicher für die Zeile wird freigegeben.
   - **Tokenkonvertierung**: Falls die Konvertierung eines Tokens zu `float` oder `size_t` fehlschlägt, wird eine Fehlermeldung ausgegeben und der Speicher für die Zeile wird freigegeben.
   - **Dateioperationen**: Falls das Öffnen der Datei fehlschlägt, wird eine Fehlermeldung ausgegeben. Falls das Lesen der Dimensionen, Datenwerte, Zeilenindizes oder Spaltenzeiger fehlschlägt, wird eine Fehlermeldung ausgegeben und die Datei wird geschlossen.
   - **Datenkonsistenz**: Falls die Anzahl der Zeilenindizes nicht mit der Anzahl der Nicht-Null-Elemente übereinstimmt, wird eine Fehlermeldung ausgegeben und die Datei wird geschlossen. Falls nach den erwarteten Matrizenwerten zusätzliche Daten vorhanden sind, wird eine Fehlermeldung ausgegeben und die Datei wird geschlossen. Falls die Anzahl der Spaltenzeiger nicht mit der Anzahl der Spalten übereinstimmt, wird eine Fehlermeldung ausgegeben und die Datei wird geschlossen.
   - **Allokation von null Zeilen in einer Null-Matrix**: Wenn die Anzahl der Nicht-Null-Elemente null ist, stellt die Funktion sicher, dass alle Einträge in den Spaltenzeigern ebenfalls null sind. Falls dies nicht der Fall ist, wird eine Fehlermeldung ausgegeben und die Datei wird geschlossen.

#### csc_matrix_writer.c

Die Datei `csc_matrix_writer.c` enthält Funktionen zum Schreiben von Matrizen im CSC-Format in Textdateien.

1. **Funktion `writeCSCMatrix`**: Diese Funktion schreibt eine CSC-Matrix in eine Textdatei. Sie nimmt als Parameter den Dateinamen und einen Zeiger auf die Matrixstruktur entgegen.

   - **Überprüfung der Eingabeparameter**: Die Funktion überprüft, ob der Dateiname und der Matrixzeiger gültig sind. Falls nicht, wird eine Fehlermeldung ausgegeben und `EXIT_FAILURE` zurückgegeben.
   - **Öffnen der Datei**: Die Datei wird zum Schreiben geöffnet. Falls dies fehlschlägt, wird eine Fehlermeldung ausgegeben, die den Grund des Fehlers beschreibt, und `EXIT_FAILURE` zurückgegeben.
   - **Schreiben der Dimensionen**: Die Anzahl der Zeilen und Spalten der Matrix wird in die Datei geschrieben. Falls dies fehlschlägt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Schreiben der Werte**: Die Nicht-Null-Werte der Matrix werden in die Datei geschrieben. Jeder Wert wird durch ein Komma getrennt. Falls dies fehlschlägt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Schreiben der Zeilenindizes**: Die Zeilenindizes der Nicht-Null-Werte werden in die Datei geschrieben. Jeder Index wird durch ein Komma getrennt. Falls dies fehlschlägt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Schreiben der Spaltenzeiger**: Die Spaltenzeiger der Matrix werden in die Datei geschrieben. Jeder Zeiger wird durch ein Komma getrennt. Falls dies fehlschlägt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Schließen der Datei**: Die Datei wird geschlossen und `EXIT_SUCCESS` wird zurückgegeben.

2. **Fehlerbehandlung**: Die Funktion enthält umfangreiche Fehlerbehandlung, um sicherzustellen, dass die Matrixdaten korrekt in die Datei geschrieben werden. Im Falle eines Fehlers gibt die Funktion entsprechende Fehlermeldungen aus und sorgt dafür, dass die Datei ordnungsgemäß geschlossen wird.

   - **Überprüfung der Eingabeparameter**: Falls der Dateiname oder der Matrixzeiger ungültig ist, wird eine Fehlermeldung ausgegeben.
   - **Fehler beim Öffnen der Datei**: Falls die Datei nicht geöffnet werden kann, wird eine Fehlermeldung ausgegeben, die den Grund des Fehlers beschreibt (z.B. fehlende Berechtigung, nicht vorhandene Datei).
   - **Fehler beim Schreiben der Dimensionen**: Falls das Schreiben der Dimensionen fehlschlägt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Fehler beim Schreiben der Werte**: Falls das Schreiben der Werte fehlschlägt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Fehler beim Schreiben der Zeilenindizes**: Falls das Schreiben der Zeilenindizes fehlschlägt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Fehler beim Schreiben der Spaltenzeiger**: Falls das Schreiben der Spaltenzeiger fehlschlägt, wird eine Fehlermeldung ausgegeben und die Datei geschlossen.
   - **Fehler beim Schließen der Datei**: Falls ein Fehler beim Schließen der Datei auftritt, wird eine Fehlermeldung ausgegeben, obwohl dies unwahrscheinlich ist, da die Standard-C-Bibliothek solche Fehler selten meldet.

#### csc_matrix_mult_V0.c

Die Datei `csc_matrix_mult_V0.c` enthält die Implementierung der Matrizenmultiplikation im CSC-Format. Die Implementierung verwendet eine naive Methode, um die Multiplikation durchzuführen, indem sie die Elemente der Matrizen einzeln multipliziert und summiert. Die Implementierung ist einfach und leicht verständlich, aber nicht besonders effizient für große Matrizen.

   - **Überprüfung der Kompatibilität der Matrizen**: Die Funktion überprüft, ob die Anzahl der Spalten der ersten Matrix mit der Anzahl der Zeilen der zweiten Matrix übereinstimmt. Falls nicht, wird eine Fehlermeldung ausgegeben und das Programm beendet.

   - **Initialisierung der Ergebnismatrix**: Die Funktion initialisiert die Dimensionen und speichert für die Ergebnismatrix. Der worst-case Speicherbedarf wird abgeschätzt und entsprechend Speicher für die Werte, Zeilenindizes und Spaltenzeiger allokiert. Falls die Allokation fehlschlägt, wird eine Fehlermeldung ausgegeben und das Programm beendet.

   - **Initialisierung der Spaltenzeiger**: Die Spaltenzeiger der Ergebnismatrix werden auf 0 gesetzt.

   - **Multiplikation der Matrizen**: Die Funktion iteriert über jede Spalte der zweiten Matrix und für jede Spalte über jeden Wert in dieser Spalte. Für jeden Wert wird die entsprechende Spalte der ersten Matrix durchsucht, um die Multiplikation durchzuführen und das Ergebnis in der Ergebnismatrix zu speichern.
    
     - **Verarbeitung der Werte**: Für jeden Wert in der aktuellen Spalte der zweiten Matrix wird die entsprechende Spalte der ersten Matrix durchsucht. Die Werte werden multipliziert und das Ergebnis wird an der richtigen Position in der Ergebnismatrix gespeichert.
        
     - **Überprüfung auf vorhandene Werte**: Vor dem Speichern eines neuen Wertes wird überprüft, ob an der entsprechenden Position bereits ein Wert vorhanden ist. Falls ja, wird der neue Wert zum vorhandenen Wert addiert. Andernfalls wird der neue Wert gespeichert.
        
     - **Einfügen neuer Werte**: Falls an der entsprechenden Position kein Wert vorhanden ist, wird der neue Wert in die Ergebnismatrix eingefügt. Die Funktion stellt sicher, dass die Zeilenindizes in jeder Spalte sortiert bleiben, indem sie den neuen Wert an der richtigen Position einfügt.

   - **Aktualisierung der Spaltenzeiger und Nicht-Null-Elemente**: Nach der Verarbeitung jeder Spalte wird der Spaltenzeiger der Ergebnismatrix aktualisiert. Nach Abschluss der Multiplikation wird die Anzahl der Nicht-Null-Elemente in der Ergebnismatrix gespeichert.

#### csc_matrix_mult_V1.c

Die Datei `csc_matrix_mult_V1.c` enthält eine Version 2 zur Implementierung der Matrizenmultiplikation im CSC-Format. Die Implementierung verwendet einen optimierten Ansatz zur Vermeidung von Backtracking durch die Verwendung eines Puffers, um die Zeilenindizes effizienter zu handhaben.

- **Überprüfung der Kompatibilität der Matrizen**: Die Funktion überprüft, ob die Anzahl der Spalten der ersten Matrix mit der Anzahl der Zeilen der zweiten Matrix übereinstimmt. Falls nicht, wird eine Fehlermeldung ausgegeben und das Programm beendet.

- **Initialisierung der Ergebnismatrix**: Die Funktion initialisiert die Dimensionen und speichert für die Ergebnismatrix. Der worst-case Speicherbedarf wird abgeschätzt und entsprechend Speicher für die Werte, Zeilenindizes und Spaltenzeiger allokiert. Falls die Allokation fehlschlägt, wird eine Fehlermeldung ausgegeben und das Programm beendet.

- **Initialisierung der Spaltenzeiger**: Die Spaltenzeiger der Ergebnismatrix werden auf 0 gesetzt.

- **Multiplikation der Matrizen**: Die Funktion iteriert über jede Spalte der zweiten Matrix und für jede Spalte über jeden Wert in dieser Spalte. Für jeden Wert wird die entsprechende Spalte der ersten Matrix durchsucht, um die Multiplikation durchzuführen und das Ergebnis in der Ergebnismatrix zu speichern.

   - **Verwendung eines Puffers**: Ein statischer Puffer wird verwendet, um die Zeilenindizes zu speichern, die bereits einen Wert in der Ergebnismatrix haben. Dies verhindert das Backtracking durch die bereits eingetragenen Werte einer Spalte.

   - **Verarbeitung der Werte**: Für jeden Wert in der aktuellen Spalte der zweiten Matrix wird die entsprechende Spalte der ersten Matrix durchsucht. Die Werte werden multipliziert und das Ergebnis wird an der richtigen Position in der Ergebnismatrix gespeichert.

   - **Überprüfung auf vorhandene Werte**: Vor dem Speichern eines neuen Wertes wird überprüft, ob an der entsprechenden Position bereits ein Wert vorhanden ist. Falls ja, wird der neue Wert zum vorhandenen Wert addiert. Andernfalls wird der neue Wert gespeichert.

   - **Einfügen neuer Werte**: Falls an der entsprechenden Position kein Wert vorhanden ist, wird der neue Wert in die Ergebnismatrix eingefügt. Die Funktion stellt sicher, dass die Zeilenindizes in jeder Spalte sortiert bleiben, indem sie den neuen Wert an der richtigen Position einfügt.

- **Aktualisierung der Spaltenzeiger und Nicht-Null-Elemente**: Nach der Verarbeitung jeder Spalte wird der Spaltenzeiger der Ergebnismatrix aktualisiert. Nach Abschluss der Multiplikation wird die Anzahl der Nicht-Null-Elemente in der Ergebnismatrix gespeichert.

#### csc_matrix_mult_V2.c

Die Datei `csc_matrix_mult_V2.c` enthält die Implementierung der Matrizenmultiplikation im CSC-Format. Diese Implementierung verwendet SIMD Intrinsics, um die Berechnungen zu beschleunigen. Der Code enthält auch eine Hilfsfunktion zur Konvertierung von CSC- zu CSR-Format (Compressed Sparse Row).

- **Überprüfung der Kompatibilität der Matrizen**: Die Funktion überprüft, ob die Anzahl der Spalten der ersten Matrix mit der Anzahl der Zeilen der zweiten Matrix übereinstimmt. Falls nicht, wird eine Fehlermeldung ausgegeben und das Programm beendet.

- **Initialisierung der Ergebnismatrix**: Die Funktion initialisiert die Dimensionen und speichert für die Ergebnismatrix. Der worst-case Speicherbedarf wird abgeschätzt und entsprechend Speicher für die Werte, Zeilenindizes und Spaltenzeiger allokiert. Falls die Allokation fehlschlägt, wird eine Fehlermeldung ausgegeben und das Programm beendet.

- **Initialisierung der Spaltenzeiger**: Die Spaltenzeiger der Ergebnismatrix werden auf 0 gesetzt.

- **Multiplikation der Matrizen**: Die Funktion iteriert über jede Spalte der zweiten Matrix und für jede Spalte über jeden Wert in dieser Spalte. Für jeden Wert wird die entsprechende Spalte der ersten Matrix durchsucht, um die Multiplikation durchzuführen und das Ergebnis in der Ergebnismatrix zu speichern.

   - **Verwendung eines Puffers**: Ein statischer Puffer wird verwendet, um die Zeilenindizes zu speichern, die bereits einen Wert in der Ergebnismatrix haben. Dies verhindert das Backtracking durch die bereits eingetragenen Werte einer Spalte.

   - **SIMD-Optimierung**: Die Funktion verwendet SIMD-Instruktionen, um die Multiplikationen zu beschleunigen. Die Werte der ersten Matrix werden in 4er-Blöcken geladen und mit einem Skalar multipliziert, der aus der zweiten Matrix stammt. Dies verbessert die Effizienz der Berechnungen erheblich.

   - **Verarbeitung der Werte**: Für jeden Wert in der aktuellen Spalte der zweiten Matrix wird die entsprechende Spalte der ersten Matrix durchsucht. Die Werte werden multipliziert und das Ergebnis wird an der richtigen Position in der Ergebnismatrix gespeichert.

   - **Überprüfung auf vorhandene Werte**: Vor dem Speichern eines neuen Wertes wird überprüft, ob an der entsprechenden Position bereits ein Wert vorhanden ist. Falls ja, wird der neue Wert zum vorhandenen Wert addiert. Andernfalls wird der neue Wert gespeichert.

   - **Einfügen neuer Werte**: Falls an der entsprechenden Position kein Wert vorhanden ist, wird der neue Wert in die Ergebnismatrix eingefügt. Die Funktion stellt sicher, dass die Zeilenindizes in jeder Spalte sortiert bleiben, indem sie den neuen Wert an der richtigen Position einfügt.

- **Aktualisierung der Spaltenzeiger und Nicht-Null-Elemente**: Nach der Verarbeitung jeder Spalte wird der Spaltenzeiger der Ergebnismatrix aktualisiert. Nach Abschluss der Multiplikation wird die Anzahl der Nicht-Null-Elemente in der Ergebnismatrix gespeichert.

- **Hilfsfunktion `csc_to_csr`**: Diese Funktion konvertiert eine Matrix vom CSC-Format in das CSR-Format. Sie zählt die Einträge in jeder Zeile, berechnet die Zeilenzeiger und füllt die Werte und Spaltenindizes entsprechend. Die Konvertierung wird verwendet, um die Matrix für andere Berechnungen vorzubereiten.

#### csc_matrix_mult_V3.c

Die Datei `csc_matrix_mult_V3.c` enthält die Implementierung der Matrizenmultiplikation im CSC-Format. Diese Implementierung verwendet eine optimierte Methode zur Handhabung der Zeilenindizes und SIMD (Single Instruction, Multiple Data) Intrinsics zur Beschleunigung der Berechnungen.

- **Überprüfung der Kompatibilität der Matrizen**: Die Funktion überprüft, ob die Anzahl der Spalten der ersten Matrix mit der Anzahl der Zeilen der zweiten Matrix übereinstimmt. Falls nicht, wird eine Fehlermeldung ausgegeben und das Programm beendet.

- **Initialisierung der Ergebnismatrix**: Die Funktion initialisiert die Dimensionen und speichert für die Ergebnismatrix. Der worst-case Speicherbedarf wird abgeschätzt und entsprechend Speicher für die Werte, Zeilenindizes und Spaltenzeiger allokiert. Falls die Allokation fehlschlägt, wird eine Fehlermeldung ausgegeben und das Programm beendet.

- **Initialisierung der Spaltenzeiger**: Die Spaltenzeiger der Ergebnismatrix werden auf 0 gesetzt.

- **Multiplikation der Matrizen**: Die Funktion iteriert über jede Spalte der zweiten Matrix und für jede Spalte über jeden Wert in dieser Spalte. Für jeden Wert wird die entsprechende Spalte der ersten Matrix durchsucht, um die Multiplikation durchzuführen und das Ergebnis in der Ergebnismatrix zu speichern.

   - **Verwendung eines Puffers**: Ein statischer Puffer wird verwendet, um die Zeilenindizes zu speichern, die bereits einen Wert in der Ergebnismatrix haben. Dies verhindert das Backtracking durch die bereits eingetragenen Werte einer Spalte.

   - **SIMD-Optimierung**: Die Funktion verwendet SIMD-Instruktionen, um die Multiplikationen zu beschleunigen. Die Werte der ersten Matrix werden in 4er-Blöcken geladen und mit einem Skalar multipliziert, der aus der zweiten Matrix stammt. Dies verbessert die Effizienz der Berechnungen erheblich.

   - **Verarbeitung der Werte**: Für jeden Wert in der aktuellen Spalte der zweiten Matrix wird die entsprechende Spalte der ersten Matrix durchsucht. Die Werte werden multipliziert und das Ergebnis wird an der richtigen Position in der Ergebnismatrix gespeichert.

   - **Überprüfung auf vorhandene Werte**: Vor dem Speichern eines neuen Wertes wird überprüft, ob an der entsprechenden Position bereits ein Wert vorhanden ist. Falls ja, wird der neue Wert zum vorhandenen Wert addiert. Andernfalls wird der neue Wert gespeichert.

   - **Einfügen neuer Werte**: Falls an der entsprechenden Position kein Wert vorhanden ist, wird der neue Wert in die Ergebnismatrix eingefügt. Die Funktion stellt sicher, dass die Zeilenindizes in jeder Spalte sortiert bleiben, indem sie den neuen Wert an der richtigen Position einfügt.

- **Aktualisierung der Spaltenzeiger und Nicht-Null-Elemente**: Nach der Verarbeitung jeder Spalte wird der Spaltenzeiger der Ergebnismatrix aktualisiert. Nach Abschluss der Multiplikation wird die Anzahl der Nicht-Null-Elemente in der Ergebnismatrix gespeichert.

- **Hilfsfunktion `csc_to_csr_V3`**: Diese Funktion konvertiert eine Matrix vom CSC-Format in das CSR-Format. Sie zählt die Einträge in jeder Zeile, berechnet die Zeilenzeiger und füllt die Werte und Spaltenindizes entsprechend. Die Konvertierung wird verwendet, um die Matrix für andere Berechnungen vorzubereiten.
- 
### 2.3 Codebeispiele
Füge einige relevante Codeausschnitte hinzu, die wichtige Aspekte deiner Implementierung veranschaulichen.

## 3. Optimierungen

### 3.1 Überblick

Um die Leistung und Effizienz der Matrizenmultiplikation im CSC-Format zu verbessern, wurden mehrere Optimierungen vorgenommen. Diese Optimierungen umfassen die Verwendung von Buffern zur Vermeidung von Backtracking, die Implementierung von SIMD (Single Instruction, Multiple Data) Intrinsics zur Beschleunigung von Berechnungen sowie die Verwendung statischer Arrays zur effizienten Handhabung von Zeilenindizes. Jede dieser Techniken zielt darauf ab, die Rechenzeit zu verkürzen und die Speichernutzung zu optimieren.

### 3.2 Detaillierte Optimierungen

#### Verwendung von Buffern

- **Technik**: In den Implementierungen `matr_mult_csc_V1`, `matr_mult_csc_V2` und `matr_mult_csc_V3` wurden statische Buffer (`rowBuffer`) verwendet, um die Zeilenindizes zu speichern, die bereits einen Wert in der Ergebnismatrix haben.
- **Grund**: Diese Technik verhindert das Backtracking durch die bereits eingetragenen Werte einer Spalte und reduziert so die Anzahl der notwendigen Vergleiche und Speicherzugriffe.
- **Implementierung**: Der Puffer speichert für jede Zeile, ob bereits ein Wert in der aktuellen Spalte der Ergebnismatrix vorhanden ist. Wenn ein neuer Wert berechnet wird, wird zuerst der Puffer überprüft. Wenn dort bereits ein Wert existiert, wird der neue Wert zum vorhandenen addiert. Andernfalls wird der neue Wert eingefügt und der Puffer entsprechend aktualisiert.

#### SIMD-Optimierung

- **Technik**: In den Implementierungen `matr_mult_csc_V2` und `matr_mult_csc_V3` wurden SIMD-Instruktionen verwendet, um die Multiplikationen zu beschleunigen.
- **Grund**: SIMD (Single Instruction, Multiple Data) ermöglicht die gleichzeitige Ausführung derselben Operation auf mehreren Datenpunkten, was die Berechnungszeit erheblich verkürzt.
- **Implementierung**: Die Werte der ersten Matrix werden in 4er-Blöcken geladen und mit einem Skalar multipliziert, der aus der zweiten Matrix stammt. Diese Vektorisierung der Berechnungen reduziert die Anzahl der notwendigen Schleifendurchläufe und nutzt die Prozessorarchitektur effizienter aus. Die SIMD-Instruktionen werden durch Intrinsics wie `_mm_load_ps` und `_mm_mul_ps` implementiert.

#### Optimierte Speicherallokation

- **Technik**: In allen Implementierungen (`matr_mult_csc`, `matr_mult_csc_V1`, `matr_mult_csc_V2`, `matr_mult_csc_V3`) wurde eine optimierte Speicherallokation vorgenommen, um den Speicherbedarf zu minimieren und die Speicherzugriffe zu optimieren.
- **Grund**: Eine effiziente Speicherallokation reduziert die Anzahl der Speicherzugriffe und verhindert Speicherfragmentierung, was die Gesamteffizienz des Programms erhöht.
- **Implementierung**: Der Speicher für die Ergebnismatrix wird basierend auf einer worst-case Schätzung allokiert. Dies stellt sicher, dass ausreichend Speicher für die Berechnungen zur Verfügung steht, ohne dass während der Laufzeit zusätzliche Allokationen erforderlich sind. Zudem werden die Speicherbereiche für die Werte, Zeilenindizes und Spaltenzeiger vorab auf null gesetzt, um initiale Fehler zu vermeiden und die Speicherzugriffe zu optimieren.

### 3.3 Ergebnisse der Optimierungen
Zeige, wie sich die Optimierungen auf die Performanz des Programms ausgewirkt haben. Dies könnte durch Vergleichstabellen oder Grafiken verdeutlicht werden.

## 4. Testergebnisse

### 4.1 Teststrategie

#### Testmethode

1. **Vergleich zwischen Python und C**:
   - **Python**: Die Matrizenmultiplikation wurde in Python unter Verwendung von `scipy.sparse` durchgeführt. Diese Ergebnisse dienen als Referenz für die Korrektheit.
   - **C**: Die gleiche Multiplikation wurde mit der C-Implementierung durchgeführt. Die Ergebnisse wurden mit den Python-Ergebnissen verglichen, um sicherzustellen, dass beide übereinstimmen.

#### Testtools

1. **Scipy**:
   - **Sparse**: `scipy.sparse` wurde verwendet, um zufällige dünnbesetzte Matrizen im CSC-Format zu generieren und die Referenzmultiplikation in Python durchzuführen.
   - **Isclose**: `numpy.isclose` wurde verwendet, um die Elemente der Ergebnismatrizen zu vergleichen und sicherzustellen, dass sie innerhalb einer akzeptablen Toleranz liegen.

2. **Subprocess**:
   - **Ausführung**: Das Python-Skript verwendet das `subprocess`-Modul, um die C-Implementierung auszuführen und die Ergebnisse zu lesen.
   - **Fehlerbehandlung**: Der Rückgabestatus des Prozesses wurde überprüft, um sicherzustellen, dass die C-Implementierung korrekt ausgeführt wurde.

#### Testfälle

1. **Standardfälle**:
   - Matrizen mit zufälligen Nicht-Null-Elementen in verschiedenen Dichten.
   - Matrizen unterschiedlicher Größen, um sicherzustellen, dass die Implementierung für verschiedene Dimensionen funktioniert.

2. **Randfälle**:
   - **Null-Matrizen**: Überprüfung, dass die Multiplikation von Null-Matrizen korrekt als Null-Matrix ausgegeben wird.
   - **Einheitsmatrizen**: Sicherstellung, dass die Multiplikation mit Einheitsmatrizen korrekt durchgeführt wird.
   - **Ungleiche Dimensionen**: Überprüfung, dass das Programm bei ungleichen Dimensionen der Matrizen eine Fehlermeldung ausgibt.

3. **Leistungstests**:
   - **Große Matrizen**: Tests mit sehr großen Matrizen, um die Effizienz und Speichernutzung der Implementierung zu bewerten.
   - **Dünnbesetzte Matrizen**: Tests mit Matrizen, die eine sehr geringe Dichte von Nicht-Null-Elementen aufweisen, um die Performance bei realistischen Anwendungsfällen zu prüfen.

#### Testprozess

1. **Generierung von Testmatrizen**:
   - Matrizen wurden mithilfe des `generator.py`-Skripts erzeugt. Das Skript ermöglicht die Spezifikation von Größe, Dichte und Seed für die Zufallsgenerierung.
   - Die generierten Matrizen wurden gespeichert, um sie in den Tests wiederzuverwenden.

2. **Durchführung der Tests**:
   - Die `tester.py`-Skripte führten die Multiplikation sowohl in Python als auch in C durch und verglichen die Ergebnisse.
   - Bei Abweichungen wurden detaillierte Ausgaben erzeugt, um die Ursache der Diskrepanz zu identifizieren.

3. **Validierung der Ergebnisse**:
   - Die Ergebnisse der C-Implementierung wurden gegen die Python-Referenzergebnisse geprüft.
   - Abweichungen wurden analysiert und behoben, um die Korrektheit der Implementierung sicherzustellen.

### 4.2 Randfälle

Das Programm wurde daraufhin getestet, wie es mit verschiedenen Randfällen umgeht, die bei der Multiplikation von Matrizen im CSC-Format auftreten können. Hier sind einige der ungewöhnlichen oder extremen Bedingungen, die getestet wurden, sowie die Reaktionen des Programms:

#### Null-Matrizen

- **Bedingung**: Beide Eingabematrizen sind vollständig mit Nullwerten gefüllt.
- **Erwartetes Verhalten**: Das Ergebnis sollte ebenfalls eine Null-Matrix sein.
- **Reaktion des Programms**: Das Programm hat korrekt eine Null-Matrix zurückgegeben, indem es alle Einträge als null berechnet und keine unnötigen Berechnungen durchgeführt hat.

#### Einheitsmatrizen

- **Bedingung**: Eine oder beide der Eingabematrizen sind Einheitsmatrizen.
- **Erwartetes Verhalten**: Das Ergebnis sollte die andere Matrix unverändert sein.
- **Reaktion des Programms**: Das Programm hat korrekt die andere Matrix als Ergebnis zurückgegeben, indem es die Eigenschaften der Einheitsmatrix genutzt hat.

#### Sehr große Matrizen

- **Bedingung**: Die Eingabematrizen sind sehr groß und enthalten Millionen von Nicht-Null-Elementen.
- **Erwartetes Verhalten**: Das Programm sollte die Matrizen effizient multiplizieren und die Speichernutzung optimieren.
- **Reaktion des Programms**: Dank der optimierten Speicherallokation und der Verwendung von SIMD-Instruktionen hat das Programm die Berechnungen effizient durchgeführt, ohne auf Speicherüberläufe oder erhebliche Performance-Einbußen zu stoßen.

#### Matrizen mit ungleichen Dimensionen

- **Bedingung**: Die Anzahl der Spalten der ersten Matrix stimmt nicht mit der Anzahl der Zeilen der zweiten Matrix überein.
- **Erwartetes Verhalten**: Das Programm sollte eine Fehlermeldung ausgeben und die Berechnung abbrechen.
- **Reaktion des Programms**: Das Programm hat korrekt eine Fehlermeldung ausgegeben und die Berechnung abgebrochen, um Inkonsistenzen zu vermeiden.

#### Dünnbesetzte Matrizen

- **Bedingung**: Die Matrizen haben eine sehr geringe Dichte von Nicht-Null-Elementen.
- **Erwartetes Verhalten**: Das Programm sollte nur die relevanten Berechnungen durchführen und keine unnötigen Operationen auf Nullwerten ausführen.
- **Reaktion des Programms**: Das Programm hat effizient nur die notwendigen Berechnungen durchgeführt und die Zeit für unnötige Operationen auf Nullwerten minimiert.

#### Zufällig verteilte Nicht-Null-Elemente

- **Bedingung**: Die Nicht-Null-Elemente sind zufällig in den Matrizen verteilt.
- **Erwartetes Verhalten**: Das Programm sollte korrekt alle relevanten Multiplikationen durchführen und die Ergebnisse richtig summieren.
- **Reaktion des Programms**: Das Programm hat die zufällig verteilten Nicht-Null-Elemente korrekt multipliziert und die Ergebnisse ordnungsgemäß summiert, ohne Fehler oder unerwartete Ergebnisse.

### 5. Performanz

### 5.1 Voraussetzungen für die Performanzmessung

Die Performanzmessungen wurden unter den folgenden Voraussetzungen durchgeführt:

- **Server:** Hetzner CX22 V-Server
- **Prozessor:** XXXGHz
- **RAM:** 4 GB
- **Betriebssystem:** Ubuntu 24.04 LTS mit 2 VCPUs
- **Architektur:** XX Bit
- **Kernel:** Linux-Kernel XXX
- **Kompilierung:** GCC XXX mit der Option XXX
- **Eingabengrößen:** Verschiedene Matrizen mit variierender Größe und Dichte
- **Wiederholungen:** XX Wiederholungen pro Ausführung

### 5.2 Performanzanalyse

Analysiere die Leistung deines Programms. Welche Metriken wurden gemessen (z.B. Laufzeit, Speicherverbrauch)?

### 5.3 Ergebnisse der Performanztests

Präsentiere die Ergebnisse der Performanztests. Verwende Diagramme und Tabellen, um die Daten übersichtlich darzustellen.

### 5.4 Diskussion der Performanz

Diskutiere die Ergebnisse der Performanztests. Was bedeuten die Ergebnisse für die praktische Anwendung des Programms? Gibt es Bereiche, in denen die Performanz verbessert werden könnte?


## 6. Fazit und Ausblick

### 6.1 Zusammenfassung

Im Rahmen dieses Projekts wurde eine effiziente Implementierung zur Multiplikation von Matrizen im CSC-Format entwickelt und getestet. Die Hauptziele umfassten die Erstellung eines Programms zur Multiplikation dünnbesetzter Matrizen, die Optimierung der Implementierung für bessere Leistung und die Validierung der Ergebnisse durch umfassende Tests. Zu den wichtigsten erreichten Punkten gehören:

- **Korrekte Implementierung**: Die Matrizenmultiplikation wurde korrekt implementiert, wobei verschiedene Versionen (V0, V1, V2, V3) entwickelt wurden, um unterschiedliche Optimierungsansätze zu erproben.
- **Effizienzsteigerung**: Durch die Verwendung von Buffern zur Vermeidung von Backtracking und den Einsatz von SIMD-Instruktionen konnte die Effizienz der Berechnungen erheblich verbessert werden.
- **Umfassende Tests**: Die Implementierung wurde gründlich getestet, indem verschiedene Szenarien und Randfälle abgedeckt wurden. Dies schloss die Überprüfung der Ergebnisse gegen eine Python-Referenzimplementierung ein.
- **Robustheit**: Das Programm reagiert korrekt auf unterschiedliche Eingabematrizen, einschließlich Null-Matrizen, Einheitsmatrizen und Matrizen mit ungleichen Dimensionen.

### 6.2 Ausblick

Für zukünftige Arbeiten und mögliche Erweiterungen des Programms gibt es mehrere interessante Ansätze:

- **Parallele Verarbeitung**: Die Implementierung könnte durch den Einsatz von parallelen Algorithmen und Multi-Threading weiter verbessert werden. Dies könnte die Berechnungszeit bei großen Matrizen erheblich verkürzen.
- **Optimierung der Speicherverwaltung**: Weitere Optimierungen der Speicherverwaltung, wie die Verwendung von Speicherpools oder optimierten Speicherallokatoren, könnten die Effizienz steigern und Speicherfragmentierung verhindern.
- **Unterstützung weiterer Formate**: Die Implementierung könnte erweitert werden, um zusätzliche dünnbesetzte Matrixformate wie COO (Coordinate Format) oder DOK (Dictionary of Keys) zu unterstützen. Dies würde die Flexibilität und Anwendbarkeit des Programms erhöhen.

