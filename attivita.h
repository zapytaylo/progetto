#define LUNGHEZZA_DC 50
#define L_DATA 10

typedef struct {
    char descr[LUNGHEZZA_DC];
    char corso[LUNGHEZZA_DC];
    int data[L_DATA];
    int tempo;
    int priorita;
    char completato;
} attivita;

