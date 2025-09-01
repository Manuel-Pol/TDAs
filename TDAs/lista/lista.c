#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"

typedef struct nodo{
    void* dato;
    struct nodo* prox;
} nodo_t;

nodo_t* _crear_nodo(void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));

    if (nodo == NULL) return NULL;

    nodo->dato = dato;
    nodo->prox = NULL;
    return nodo;
}

void _destruir_nodo(nodo_t* nodo) {
    free(nodo);
}

typedef struct lista {
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
} lista_t;

lista_t* lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));

    if (!lista) return NULL;

    lista->largo = 0;
    lista->ultimo = NULL;
    lista->primero = NULL;
    return lista;
}

bool lista_esta_vacia(const lista_t* lista){
    return lista->largo == 0;
}

bool lista_insertar_primero(lista_t* lista, void* dato){
    nodo_t* nodo_a_insertar = _crear_nodo(dato);

    if (!nodo_a_insertar) return false;

    if (!lista_esta_vacia(lista)) nodo_a_insertar->prox = lista->primero; else lista->ultimo = nodo_a_insertar;
    
    lista->primero = nodo_a_insertar;
    lista->largo++;
    return true;
}

void lista_invertir(lista_t* lista) {
    if (lista_esta_vacia(lista) || lista_largo(lista) == 1) return;
    nodo_t* actual = lista->primero;
    nodo_t* anterior = NULL;
    nodo_t* siguiente = lista->primero->prox;
    while (siguiente != NULL){
        actual->prox = anterior;
        anterior = actual;
        actual = siguiente;
        siguiente = siguiente->prox;
    }
    actual->prox = anterior;
    nodo_t* aux = lista->primero;
    lista->primero = lista->ultimo;
    lista->ultimo = aux;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo_a_insertar = _crear_nodo(dato);

    if (!nodo_a_insertar) return false;

    if (!lista_esta_vacia(lista)) lista->ultimo->prox = nodo_a_insertar; else lista->primero = nodo_a_insertar;
    
    lista->ultimo = nodo_a_insertar;
    lista->largo++;
    return true;
}

size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

void *lista_borrar_primero(lista_t *lista){
    nodo_t* nuevo_primero = NULL;

    if (lista_esta_vacia(lista)) return NULL;

    void* dato_a_devolver = lista->primero->dato;

    if (lista_largo(lista) == 1) lista->ultimo = NULL; else nuevo_primero = lista->primero->prox;

    _destruir_nodo(lista->primero);
    lista->primero = nuevo_primero;
    lista->largo--;
    return dato_a_devolver;
}

void *lista_ver_primero(const lista_t *lista){
    return lista->primero ? lista->primero->dato : NULL;
}

void *lista_ver_ultimo(const lista_t* lista){
    return lista->ultimo ? lista->ultimo->dato : NULL;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
    nodo_t* actual = lista->primero;
    nodo_t* prox;

    while(actual) {
         prox = actual->prox;

         if (destruir_dato != NULL) destruir_dato(actual->dato);

         _destruir_nodo(actual);
         actual = prox;
    }

    free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    nodo_t* actual = lista->primero;

    while(actual){

         if (!visitar(actual->dato, extra)) break;

         actual = actual->prox;
    }

}

typedef struct lista_iter{
    nodo_t* actual;
    nodo_t* anterior;
    lista_t* lista;
} lista_iter_t;

lista_iter_t* lista_iter_crear(lista_t *lista){
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));

    if (!iter) return NULL;

    iter->lista = lista;
    iter->actual = lista->primero;
    iter->anterior = NULL;
    return iter;
}

void* lista_iter_ver_actual(const lista_iter_t *iter){
    return iter->actual != NULL ? iter->actual->dato : NULL;
}

bool lista_iter_al_final(const lista_iter_t* iter){
    return !lista_iter_ver_actual(iter);
}

bool lista_iter_avanzar(lista_iter_t *iter){

    if (lista_iter_al_final(iter)) return false;

    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void* dato){
    nodo_t* nodo_a_insertar = _crear_nodo(dato);

    if (!nodo_a_insertar) return false;
    
    if (lista_iter_al_final(iter)){

        if (lista_esta_vacia(iter->lista)) iter->lista->primero = nodo_a_insertar; else iter->lista->ultimo->prox = nodo_a_insertar;
        
        iter->lista->ultimo = nodo_a_insertar;
    }
    else{
        nodo_a_insertar->prox = iter->actual;

        if (!iter->anterior) iter->lista->primero = nodo_a_insertar; else iter->anterior->prox = nodo_a_insertar;    
    
    }
    
    iter->actual = nodo_a_insertar;
    iter->lista->largo++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
    
    if (lista_iter_al_final(iter)) return NULL;

    void* dato_a_devolver = iter->actual->dato;
    nodo_t* aux_prox = iter->actual->prox;

    if (!aux_prox) iter->lista->ultimo = iter->anterior;
    
    if (!iter->anterior) iter->lista->primero = aux_prox; else iter->anterior->prox = aux_prox;
    
    _destruir_nodo(iter->actual);
    iter->actual = aux_prox;
    iter->lista->largo--;
    return dato_a_devolver;
}
