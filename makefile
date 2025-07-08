# Makefile pour le projet de gestion de stock
# Ce Makefile compile le programme en C, gère les dépendances et crée un exécutable.
# Il est conçu pour être utilisé sur Windows, mais peut être adapté pour d'autres systèmes.

#Définit le compilateur utilisé (ici GCC).
# Check if the OS is Windows and set the compiler accordingly.
ifeq ($(OS),Windows_NT)
#local	build settings for Windows
 RM = del /S /Q
 RMDIR = rmdir /S /Q
 MKDIR = if not exist build mkdir build
 EXEC_EXT = .exe
 CC := gcc
else
#cross-platform settings
#used for github actions
 RM = rm -f
 RMDIR = rm -rf
 MKDIR = mkdir -p build
 EXEC_EXT =
 CC ?= gcc
endif

# Test sources and executable
TEST_SRC = test/test_database.c
TEST_EXEC = build/test_database$(EXEC_EXT)
# Définit la version du programme.
BIN = build/gestion_stock_v$(VERSION)$(EXEC_EXT)

#Options de compilation :
#-Wall et -Wextra activent de nombreux avertissements utiles pour repérer les erreurs potentielles.
#-g ajoute les symboles de débogage (utile avec GDB, par exemple).
#-I ajoute le dossier "Inc" au chemin de recherche des fichiers d'en-tête.
#fix No Dependency Tracking
# -MMD in CFLAGS to generate .d files

CFLAGS=-Wall -Wextra -g -IInc -MMD

#Liste des fichiers source principaux et supplémentaires.
SRC=$(SRC_MAIN) $(SRC_SRC) $(SRC_SQLITE)
#SRC_MAIN est le fichier principal (main.c).
SRC_MAIN=main.c
#SRC_SRC contient tous les fichiers .c du dossier src.
SRC_SRC=$(wildcard Src/*.c)
#Ajoute le fichier SQLite3 source.
SRC_SQLITE = sqlite-lib/sqlite3.c

#Convertit la liste de fichiers .c en fichiers .o (les objets intermédiaires).
OBJ=$(patsubst %.c,build/%.o,$(SRC))

# Nom de l’exécutable final.
# Si le dossier build n'existe pas, il sera créé automatiquement.
EXEC=build/gestion_stock$(EXEC_EXT)

#	Fichiers de test d'intégration.
TEST_INTEGRATION_SRC = test/test_integration.c
TEST_INTEGRATION_EXEC = build/test_integration$(EXEC_EXT)
#Cible par défaut : si tu tapes make, cela va construire l’exécutable.
all: $(EXEC)

#Crée l’exécutable à partir des fichiers .o.
#MKDIR crée le dossier build s'il n'existe pas.
#$@ = le nom de la cible (gestion_stock.exe)
#$^ = tous les fichiers .o (dépendances)
$(EXEC): $(OBJ)
	@$(MKDIR)
	@echo "Compilation de $(EXEC)..."
	$(CC) $(CFLAGS) -o $@ $^

#Compile chaque fichier .c individuellement en .o.
#Cette règle utilise un modèle pour compiler tous les fichiers .c dans le dossier courant.
#Le modèle build/%.o signifie que tous les fichiers .c seront compilés en fichiers
# .o dans le dossier build.
#Le symbole % est un joker qui correspond à n'importe quel nom de fichier.
#Le fichier build/%.o est créé à partir du fichier %.c.
#$(dir $@) est le répertoire du fichier cible, et $(dir $<) est le répertoire du fichier source.
#$< = le fichier source .c
#$@ = le fichier .o généré
# Compilation individuelle des fichiers .c vers build/*.o

# windows part :
# $(dir $@) is evaluated by Make, not PowerShell.
# $(subst /,\,$(dir $@)) converts forward slashes to backslashes for Windows compatibility.
# if not exist works in cmd.exe and PowerShell when paths are properly escaped.

build/%.o: %.c
ifeq ($(OS),Windows_NT)
	@if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
else
	@mkdir -p $(dir $@)
endif
	$(CC) $(CFLAGS) -c $< -o $@


#Cible pour nettoyer les fichiers intermédiaires et l'exécutable.
#support for Windows and Unix-like systems.
clean:
	@echo "Nettoyage..."
	-@$(RM) *.o
	-@$(RM) build/*$(EXEC_EXT)
	-@$(RMDIR) build
	-@$(RM) build/*.d

#Compile et lance ton programme en une seule commande : make run
run: all

	./$(EXEC)

# CI build with embedded version macro and versioned binary for linux
ci-build:
	@mkdir -p build
	$(CC) $(CFLAGS) -DVERSION=\"$(VERSION)\" -o build/gestion_stock_v$(VERSION) $(SRC)
	
# CI build for Windows with embedded version macro and versioned binary
ci-build-windows:
	@mkdir -p build
	x86_64-w64-mingw32-gcc $(CFLAGS) -DVERSION=\"$(VERSION)\" -o build/gestion_stock_v$(VERSION).exe $(SRC)

# Test target
test: $(TEST_EXEC)
	@echo "Exécution des tests unitaires..."
	./$(TEST_EXEC)

# Build test executable
$(TEST_EXEC): $(TEST_SRC) $(SRC_SQLITE) Src/database.c
	@$(MKDIR)
	$(CC) $(CFLAGS) -o $@ $^

#	Cible pour exécuter les tests d'intégration.
test-integration: $(TEST_INTEGRATION_EXEC)
	@echo "Exécution du test d'intégration..."
	./$(TEST_INTEGRATION_EXEC)

$(TEST_INTEGRATION_EXEC): $(TEST_INTEGRATION_SRC) $(SRC_SQLITE) $(SRC_SRC)
	@$(MKDIR)
	$(CC) $(CFLAGS) -o $@ $^

#	Cible pour exécuter les tests valgrind.
ifeq ($(OS),Windows_NT)
valgrind-test:
	@echo "Valgrind n'est pas disponible sur Windows. Utilisez WSL ou CI."
valgrind-integration:
	@echo "Valgrind n'est pas disponible sur Windows. Utilisez WSL ou CI."
else
valgrind-test: $(TEST_EXEC)
	valgrind --leak-check=full --error-exitcode=1 ./$(TEST_EXEC)

valgrind-integration: $(TEST_INTEGRATION_EXEC)
	valgrind --leak-check=full --error-exitcode=1 ./$(TEST_INTEGRATION_EXEC)
endif

#Code coverage tools track which lines of code are executed during tests. This helps you:
#Spot dead or unused code
#Identify untested branches or conditions
#Improve test completeness
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage -g -O0

coverage: CFLAGS += $(COVERAGE_FLAGS)
coverage: LDFLAGS += -lgcov
coverage: clean $(TEST_EXEC)
	@echo "Exécution des tests avec couverture..."
	./$(TEST_EXEC)
	@echo "Génération du rapport de couverture..."
	gcovr -r . --html --html-details -o build/coverage.html

#	Cible pour générer un rapport de bugs hebdomadaire.
#	Le rapport inclut les résultats des tests, l'analyse statique et la couverture de code.
#	Le rapport est généré dans le dossier reports avec un nom basé sur la date actuelle
WEEKLY_DATE := $(shell date +%F)
WEEKLY_REPORT = reports/weekly_report_$(WEEKLY_DATE).md
BUG_REPORT_XML = reports/bug_report_raw.xml
#	Assure that the reports directory exists
.PHONY: weekly-report
#	weekly-report: $(WEEKLY_REPORT)
weekly-report:
	@echo "Génération du rapport hebdomadaire..."
	@mkdir -p reports
	@echo "#  Weekly Report - $(WEEKLY_DATE)" > $(WEEKLY_REPORT)
	@echo "" >> $(WEEKLY_REPORT)
	@echo "## Test Results" >> $(WEEKLY_REPORT)
	@make test >> $(WEEKLY_REPORT) 2>&1 || echo " Unit tests failed." >> $(WEEKLY_REPORT)
	@make test-integration >> $(WEEKLY_REPORT) 2>&1 || echo " Integration tests failed." >> $(WEEKLY_REPORT)
	@echo "" >> $(WEEKLY_REPORT)
	@echo "##  Valgrind" >> $(WEEKLY_REPORT)
	@make valgrind-test >> $(WEEKLY_REPORT) 2>&1 || echo " Valgrind (unit) failed." >> $(WEEKLY_REPORT)
	@make valgrind-integration >> $(WEEKLY_REPORT) 2>&1 || echo " Valgrind (integration) failed." >> $(WEEKLY_REPORT)
	@echo "" >> $(WEEKLY_REPORT)
	@echo "##  Code Coverage" >> $(WEEKLY_REPORT)
	@make coverage >> $(WEEKLY_REPORT) 2>&1 || echo " Coverage generation failed." >> $(WEEKLY_REPORT)
	@echo "" >> $(WEEKLY_REPORT)
	@echo "##  Static Analysis (cppcheck)" >> $(WEEKLY_REPORT)
	@cppcheck --enable=all --inconclusive --quiet --std=c99 --language=c --xml --xml-version=2 . 2> $(BUG_REPORT_XML) || true
	@python3 scripts/format_bug_report.py $(BUG_REPORT_XML) >> $(WEEKLY_REPORT)
	@echo " Rapport hebdomadaire généré : $(WEEKLY_REPORT)"

#	Génération des dépendances pour la compilation
-include $(OBJ:.o=.d)

# Déclaration des cibles phony
.PHONY: all clean run test test-integration valgrind-test valgrind-integration coverage ci-build ci-build-windows weekly-report

