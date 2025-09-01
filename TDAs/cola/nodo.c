#include "nodo.h"

#include <stdlib.h>

/*Definición del struct nodo*/
struct nodo{
    void *dato;
    struct nodo *prox;
};

/* *****************************************************************
 *                    PRIMITIVAS DEL NODO
 * *****************************************************************/

nodo_t* nodo_crear(void *valor){
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL){
        return NULL;
    }
    nodo->dato = valor;
    nodo->prox = NULL;
    return nodo;
}

void *nodo_ver_dato(nodo_t *nodo){
    return nodo->dato;
}

void nodo_proximo(nodo_t *actual, nodo_t *proximo){
    actual->prox = proximo;
}

nodo_t *nodo_ver_proximo(nodo_t *nodo){
    return nodo->prox;
}

void nodo_destruir(nodo_t *nodo){
    free(nodo);
}