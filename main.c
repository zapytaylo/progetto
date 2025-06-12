#include <stdio.h>
#include "attivita.h"

int main(int argc, char *argv[]) {
    Nodo* lista = NULL;
    int scelta;
    FILE *input = stdin;
    FILE *output = stdout;

    // Se viene passato un argomento, usalo come file di input
    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Errore nell'apertura del file di input\n");
            return 1;
        }
    }

    if (argc > 2) {
        output = fopen(argv[2], "w");
        if (!output) {
            fprintf(stderr, "Errore nell'apertura del file di output\n");
            fclose(input);
            return 1;
        }
    }

    do {
        mostraMenu(output);
        fscanf(input, "%d", &scelta);
        fgetc(input); // consuma il newline

        switch (scelta) {
            case 1:
                lista = aggiungiAttivitaFile(lista, input, output);
                break;
            case 2:
                mostraAttivitaFile(lista, input, output);
                break;
            case 3:
                aggiornaProgressoFile(lista, input, output);
                break;
            case 4:
                generaReportFile(lista, input, output);
                break;
            case 0:
                fprintf(output, "Uscita dal programma.\n");
                break;
            default:
                fprintf(output, "Scelta non valida.\n");
        }
    } while (scelta != 0);

    liberaLista(lista);
    
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);
    
    return 0;
}
