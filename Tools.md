# Nützliche Tools

## Beispiele-Ordner

Der Beispiele-Ordner enthält Matrizen zum Testen des Programmes. Die Matrizen
sind nach dem Schema `<m>-<n>-<density>-<seed>.txt` benannt.

## Flamegraphs

Im Ordner `Flammengraphen` finden sich Flamegraphs zu den verschiedenen
Versionen des Programms. Diese sind interaktiv und können im Browser geöffnet
werden.

## Python-Skripte

Nützliche Tools zum Testen der MM_CSC Implementierung. Die Skripte befinden sich
im `Skripte`-Ordner.

### Systemvoraussetzungen

Eine Python 3.12 Installation. Dependencies über die `requirements.txt` mit
`pip` installieren:

```sh
pip install -r Skripte/requirements.txt
```

### Verfügbare Tools

#### generator.py

Das `generator.py` Skript kann CSC-Matrizen in verschiedenen Größen und Dichten
generieren. Hierbei kann angegeben werden, dass die Matrix im Beispielordner
abgelegt werden soll (mittels der Option `--save`), andernfalls wird sie auf
`stdout` ausgegeben. Zudem kann man mit `--seed <seed> ` einen Seed angeben, um
die Generation reproduzierbar zu machen.

Verwendungsweise:

`<m>` ist die Anzahl an Spalten \
`<n>` ist die Anzahl an Zeilen

```sh
python3 ./Skripte/generator.py -m <m> -n <n> --seed=<seed>
```

#### Tester.py

Das `tester.py` Skript testet die Matrix-Implementierung auf Korrektheit. Zuvor
muss diese allerdings kompiliert werden.

Verwendungsweise:

`<a>` und `<b>` sind verschiedene Matrizen.

```sh
python3 ./Skripte/tester.py <a> <b>
```
