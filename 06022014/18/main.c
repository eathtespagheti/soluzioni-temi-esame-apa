#include "SymbleTable.h/SymbleTableLib.h"

uint MAX_LENGHT        = 10 + 1;
uint NUMBER_OF_STRINGS = 3;
uint MAX_CHARS         = 26;
SymbleTableLib ST;

void parseSymbles(char **strings, SymbleTable s) {
    uint len = 0;
    for (size_t i = 0; i < NUMBER_OF_STRINGS; i++) {
        len = strlen(strings[i]);
        for (size_t j = 0; j < len; j++) {
            ST.addSymbleFromChar(s, strings[i][j]);
        }
    }
}

void readSymbles(char **strings, SymbleTable s) {
    uint symblesNumber = ST.getNumberOfSymbles(s);
    uint value         = 0;
    for (size_t i = 0; i < symblesNumber; i++) {
        printf("Insert value for the following character %s : ",
               ST.getSymbleByIndex(s, i)->Name);
        scanf("%d", &value);
        ST.changeIDByIndex(s, i, value);
    }
}

void readStrings(char **strings) {
    for (size_t i = 0; i < NUMBER_OF_STRINGS; i++) {
        printf("Insert string number %d: ", i);
        scanf("%s", strings[i]);
    }
}

bool verificaID(int a, int b, int c, int *resto) {
    int somma = a + b + *resto;
    *resto    = somma / 10;
    somma %= 10;
    return somma == c;
}

bool verificaTabella(SymbleTable s, char **strings) {
    int lenA    = strlen(strings[0]);
    int lenB    = strlen(strings[1]);
    int lenC    = strlen(strings[2]);
    int longest = lenA >= lenB ? lenA : lenB;
    int valA, valB, valC, resto = 0;

    for (size_t i = 0; i < longest; i++) {
        valA = i >= lenA
                   ? 0
                   : ST.getSymbleFromChar(s, strings[0][lenA - 1 - i])->ID;
        valB = i >= lenB
                   ? 0
                   : ST.getSymbleFromChar(s, strings[1][lenB - 1 - i])->ID;
        valC = i >= lenC
                   ? 0
                   : ST.getSymbleFromChar(s, strings[2][lenC - 1 - i])->ID;
        if (!verificaID(valA, valB, valC, &resto)) { return false; }
    }
    return true;
}

bool verificaSoluzione(char **strings) {
    SymbleTable s = ST.new(MAX_CHARS);
    parseSymbles(strings, s);
    readSymbles(strings, s);
    bool result = verificaTabella(s, strings);
    ST.free(s);
    return result;
}

void setIDsFromArray(SymbleTable s, int *array) {
    uint numberOfSymbles = ST.getNumberOfSymbles(s);
    for (size_t i = 0; i < numberOfSymbles; i++) {
        ST.changeIDByIndex(s, i, array[i]);
    }
}

void printSoluzione(SymbleTable s) {
    Symble tmp;
    uint nS = ST.getNumberOfSymbles(s);
    printf("La soluzione è:\n");
    for (size_t i = 0; i < nS; i++) {
        tmp = ST.getSymbleByIndex(s, i);
        printf("%s -> %d\n", tmp->Name, tmp->ID);
    }
}

bool disposizioniSempliciR(SymbleTable s, int *a, bool *freeValues, uint pos,
                           char **strings) {
    int nSymbles = ST.getNumberOfSymbles(s);
    int nValues  = ST.getSize(s);

    if (pos >= nSymbles) {
        setIDsFromArray(s, a);
        return verificaTabella(s, strings);
    }

    for (size_t i = 0; i < nValues; i++) {
        if (freeValues[i]) {
            freeValues[i] = false;
            a[pos]        = i;
            bool result =
                disposizioniSempliciR(s, a, freeValues, pos + 1, strings);
            if (result) { return result; }
            freeValues[i] = true;
        }
    }

    return false;
}

void disposizioniSemplici(char **strings) {
    SymbleTable s = ST.new(10);
    parseSymbles(strings, s);

    uint maxSymbles      = ST.getSize(s);
    uint numberOfSymbles = ST.getNumberOfSymbles(s);
    bool *freeValues     = calloc(sizeof(*freeValues), maxSymbles);
    int *a               = calloc(sizeof(*a), maxSymbles);

    for (size_t i = 0; i < maxSymbles; i++) {
        freeValues[i] = true;
        a[i]          = i;
    }

    if (disposizioniSempliciR(s, a, freeValues, 0, strings)) {
        printSoluzione(s);
    } else {
        printf("Nessuna soluzione valida trovata\n");
    }

    ST.free(s);
    free(freeValues);
    free(a);
}

int main(int argc, char const *argv[]) {
    ST = newSymbleTableLib();
    char **strings;

    strings = calloc(sizeof(*strings), NUMBER_OF_STRINGS);
    for (size_t i = 0; i < NUMBER_OF_STRINGS; i++) {
        strings[i] = calloc(sizeof(*strings[i]), MAX_LENGHT);
    }

    readStrings(strings);
    disposizioniSemplici(strings);

    for (size_t i = 0; i < NUMBER_OF_STRINGS; i++) {
        free(strings[i]);
    }
    free(strings);
    return 0;
}
