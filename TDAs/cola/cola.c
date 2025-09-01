#include "cola.h"
#include "nodo.h"

#include <stdlib.h>

/*Definición del struct cola*/
struct cola{
    nodo_t *prim;
    nodo_t *ult;
};

struct nodo{
    void *dato;
    struct nodo *prox;
};


/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t *cola_crear(void){
    cola_t *cola = malloc(sizeof(cola_t));
    if (cola == NULL){
        return NULL;
    }
    cola->prim = NULL;
    cola->ult = NULL;
    return cola;
}

bool cola_esta_vacia(const cola_t *cola){
    return cola->prim == NULL;
}

bool cola_encolar(cola_t *cola, void *valor){
    nodo_t *nodo = nodo_crear(valor);
    if (nodo == NULL){
        return false;
    }
    if (cola->prim == NULL){
        cola->prim = nodo;
    }
    else{
        nodo_proximo(cola->ult, nodo);
    }
    cola->ult = nodo;
    return true;
}

void *cola_ver_primero(const cola_t *cola){
    if (cola_esta_vacia(cola)){
        return NULL;
    }
    return nodo_ver_dato(cola->prim);
}

void *cola_desencolar(cola_t *cola){
    if (cola_esta_vacia(cola)){
        return NULL;
    }
    void *elemento = nodo_ver_dato(cola->prim);
    void *proximo = nodo_ver_proximo(cola->prim);
    nodo_destruir(cola->prim);
    if (proximo == NULL){
        cola->ult = NULL;
    }
    cola->prim = proximo;
    return elemento;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
    nodo_t *actual = cola->prim;
    while (actual != NULL){
        if (destruir_dato != NULL) destruir_dato(nodo_ver_dato(actual));
        cola_desencolar(cola);
        actual = cola->prim;
    }
    free(cola);
}
