#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attivita.h"

// Versione originale per input interattivo
Nodo* aggiungiAttivita(Nodo* testa) {
    return aggiungiAttivitaFile(testa, stdin, stdout);
}

// Nuova versione con supporto file
Nodo* aggiungiAttivitaFile(Nodo* testa, FILE* input, FILE* output) {
    Nodo* nuovo = malloc(sizeof(Nodo));
    if (!nuovo) {
        fprintf(output, "Errore allocazione.\n");
        return testa;
    }

    fprintf(output, "\nHai scelto 'Aggiungi attivita di studio' \n");

    fprintf(output, "Inserisci descrizione: ");
    fgets(nuovo->attivita.descrizione, DESC_LEN, input);
    nuovo->attivita.descrizione[strcspn(nuovo->attivita.descrizione, "\n")] = 0;
    
    fprintf(output, "Inserisci corso: ");
    fgets(nuovo->attivita.corso, CORSO_LEN, input);
    nuovo->attivita.corso[strcspn(nuovo->attivita.corso, "\n")] = 0;
    
    fprintf(output, "Inserisci data scadenza (AAAA-MM-GG): ");
    fgets(nuovo->attivita.scadenza, DATA_LEN, input);
    nuovo->attivita.scadenza[strcspn(nuovo->attivita.scadenza, "\n")] = 0;
    
    fprintf(output, "Tempo stimato (ore): ");
    fscanf(input, "%d", &nuovo->attivita.tempoStimato);
    fgetc(input); // Consuma newline
    
    int prio;
    fprintf(output, "Priorita (1=Bassa, 2=Media, 3=Alta): ");
    fscanf(input, "%d", &prio);
    fgetc(input);
    
    nuovo->attivita.priorita = (Priorita)prio;
    nuovo->attivita.tempoTrascorso = 0;
    nuovo->attivita.stato = IN_CORSO;
    nuovo->next = testa;
    
    fprintf(output, "attivita aggiunta con successo.\n");
    return nuovo;
}

// Versione originale
void mostraAttivita(Nodo* testa) {
    mostraAttivitaFile(testa, stdin, stdout);
}

// Nuova versione con supporto file
void mostraAttivitaFile(Nodo* testa, FILE* input, FILE* output) {
    Nodo* curr = testa;
    int i = 1;
    char oggi[DATA_LEN];
    
    if (!curr) {
        fprintf(output, "Nessuna attivita.\n");
        return;
    }
    
    fprintf(output, "\nHai scelto 'Visualizza attivita' \n");
    fprintf(output, "Inserisci la data di oggi (AAAA-MM-GG): ");
    fgets(oggi, DATA_LEN, input);
    oggi[strcspn(oggi, "\n")] = 0;
    
    while (curr) {
        Attivita* a = &curr->attivita;
        if (a->stato != COMPLETATA && strcmp(oggi, a->scadenza) > 0) {
            a->stato = RITARDO;
        }
        
        fprintf(output, "\n %d) %s %s \n", i++, a->descrizione, a->corso);
        fprintf(output, " Priorita: %d | Tempo stimato: %d ore | Tempo trascorso: %d ore\n",
               a->priorita, a->tempoStimato, a->tempoTrascorso);
        
        const char* statoStr = (a->stato == COMPLETATA) ? "Completata" :
                              (a->stato == RITARDO) ? "In Ritardo" : "In Corso";
        fprintf(output, " Stato: %s | Scadenza: %s\n", statoStr, a->scadenza);
        curr = curr->next;
    }
}

// Versione originale
void aggiornaProgresso(Nodo* testa) {
    aggiornaProgressoFile(testa, stdin, stdout);
}

// Nuova versione con supporto file
void aggiornaProgressoFile(Nodo* testa, FILE* input, FILE* output) {
    mostraAttivitaFile(testa, input, output);

    fprintf(output, "\nHai scelto 'Aggiorna progresso' \n");

    fprintf(output, "\nSeleziona l'ID dell'attivita: ");
    int id, ore;
    fscanf(input, "%d", &id);
    
    fprintf(output, "Ore da aggiungere: ");
    fscanf(input, "%d", &ore);
    fgetc(input);
    
    Nodo* curr = testa;
    int i = 1;
    while (curr && i < id) {
        curr = curr->next;
        i++;
    }
    
    if (!curr) {
        fprintf(output, "ID non valido.\n");
        return;
    }
    
    curr->attivita.tempoTrascorso += ore;
    if (curr->attivita.tempoTrascorso >= curr->attivita.tempoStimato) {
        curr->attivita.stato = COMPLETATA;
    }
    fprintf(output, "Progresso aggiornato.\n");
}

// Versione originale
void generaReport(Nodo* testa) {
    generaReportFile(testa, stdin, stdout);
}

// Nuova versione con supporto file
void generaReportFile(Nodo* testa, FILE* input, FILE* output) {
    Nodo* curr = testa;
    char oggi[DATA_LEN];
    
    fprintf(output, "\nHai scelto 'Report settimanale' \n");
    fprintf(output, "Inserisci la data di oggi (AAAA-MM-GG): ");
    fgets(oggi, DATA_LEN, input);
    oggi[strcspn(oggi, "\n")] = 0;
    
    while (curr) {
        Attivita* a = &curr->attivita;
        if (a->stato == COMPLETATA) {
            fprintf(output, "[COMPLETATA] %s - %s\n", a->descrizione, a->corso);
        } else if (strcmp(oggi, a->scadenza) > 0) {
            a->stato = RITARDO;
            fprintf(output, "[IN RITARDO] %s - %s (Scadenza: %s)\n", 
                   a->descrizione, a->corso, a->scadenza);
        } else {
            fprintf(output, "[IN CORSO] %s - %s (Scadenza: %s)\n", 
                   a->descrizione, a->corso, a->scadenza);
        }
        curr = curr->next;
    }
}

// Versione originale
void mostraMenu() {
    mostraMenuFile(stdout);
}

// Nuova versione con supporto file
void mostraMenuFile(FILE* output) {
    fprintf(output, "\nGESTIONE STUDIO\n");
    fprintf(output, "1. Aggiungi attivita di studio\n");
    fprintf(output, "2. Visualizza attivita\n");
    fprintf(output, "3. Aggiorna progresso\n");
    fprintf(output, "4. Genera report settimanale\n");
    fprintf(output, "0. Esci\n");
    fprintf(output, "Seleziona un'opzione: ");
}

// Libera la memoria della lista collegata
void liberaLista(Nodo* testa) {
    Nodo* temp;
    while (testa) {
        temp = testa;
        testa = testa->next;
        free(temp); // Dealloca ogni nodo
    }
}
