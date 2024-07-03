##########
# Config #
##########

# Einstiegspunkt für das Programm und Zielname
SRC := main.c csc_matrix_reader/csc_matrix_reader.c

# Zusätzliche Flags für den Compiler
CFLAGS := -Wall -Wextra -I./csc_matrix_reader -I./csc_matrix_reader/csc_matrix_datastructure

###########
# /Config #
###########

# gcc or clang?
CC := $(shell command -v gcc || command -v clang)
ifeq ($(strip $(CC)),)
    $(error Weder clang noch gcc ist verfügbar. Beenden.)
endif


# Standardmäßig auf Release-Build für App und Bibliothek
all: debug

# Debug-Build
debug: CFLAGS += -g # Hinzufügen des Debugging-Flags, ermöglicht das Debuggen des Programms
debug: main

# Release-Build
release: CFLAGS += -O2 # Hinzufügen der Optimierungsflags für eine schnellere Ausführung des Programms
release: main

# Rezept zum Bauen des Programms
main: $(SRC)  # Definiert 'main' als Ziel, das von den Dateien in $(SRC) abhängt
	$(CC) $(CFLAGS) -o main $(SRC) # Führt den Compiler $(CC) mit den Flags $(CFLAGS) aus, um die Quelldatei $(SRC) zu kompilieren und das Ergebnis in der Datei 'main' zu speichern.

# Bereinigung
clean:
	rm -f main # Entfernt die erzeugte Ausführungsdatei 'main', um das Projektverzeichnis aufzuräumen.

.PHONY: all debug release clean
