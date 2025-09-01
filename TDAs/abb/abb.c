#include "abb.h"
#include "../pila/pila.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef int (*abb_comparar_clave_t)(const char *, const char *);
typedef void (*abb_destruir_dato_t)(void *);

char* _strdup(const char* org){
    char* copy = malloc(sizeof(char) * (strlen(org) + 1));

    if (!copy) return NULL;

    strcpy(copy, org);
    return copy;
}

/* *******************
   *       ABB       *
   ********************/

typedef struct abb_nodo {
    struct abb_nodo* der;
    struct abb_nodo* izq;
    char* clave;
    void* dato;
}abb_nodo_t;

typedef struct abb {
    abb_nodo_t* raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cant;
}abb_t;

abb_nodo_t* nodo_crear(const char *clave, void *dato){
    abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));

    if (!nodo) return NULL;

    char* nueva_clave = _strdup(clave);
    nodo->clave = nueva_clave;
    nodo->dato = dato;
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}

void nodo_destruir(abb_nodo_t* nodo){
    free(nodo->clave);
    free(nodo);
}

void* _buscar_nodo(abb_nodo_t* actual, abb_nodo_t* padre, const char* clave, abb_nodo_t** extra, abb_comparar_clave_t cmp){

    if (extra) *extra = padre;
    
    if (!actual) return NULL;

    int comparacion = cmp(clave, actual->clave);

    if (comparacion == 0) return actual;

    abb_nodo_t* siguiente = comparacion > 0 ? actual->der : actual->izq;
    return _buscar_nodo(siguiente, actual, clave, extra, cmp);
}

void* buscar_nodo(abb_t* arbol,const char* clave, abb_nodo_t** extra){
    return _buscar_nodo(arbol->raiz, NULL, clave, extra, arbol->cmp);
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));

    if (!abb) return NULL;

    abb->cmp = cmp;
    abb->raiz = NULL;
    abb->destruir_dato = destruir_dato;
    abb->cant = 0;
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    abb_nodo_t* nuevo_nodo = nodo_crear(clave, dato);

    if (!nuevo_nodo) return false;

    arbol->cant++;

    if (!arbol->raiz){
        arbol->raiz = nuevo_nodo;
        return true;
    }

    void** puntero_a_padre;
    void* padre = NULL;
    puntero_a_padre = &padre;
    abb_nodo_t* actual = buscar_nodo(arbol, clave, (abb_nodo_t**)puntero_a_padre);

    if (actual){

        if(arbol->destruir_dato) arbol->destruir_dato(actual->dato);

        actual->dato = dato;
        nodo_destruir(nuevo_nodo);
        arbol->cant--;
    } else{
        int comparacion =  arbol->cmp(clave, ((abb_nodo_t*)padre)->clave);
        
        if (comparacion > 0) {
            ((abb_nodo_t*)padre)->der = nuevo_nodo;
        } else{
            ((abb_nodo_t*)padre)->izq = nuevo_nodo;
        }
    }

    return true;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    abb_nodo_t* nodo_encontrado = buscar_nodo((abb_t*)arbol, clave, NULL);
    return nodo_encontrado ? nodo_encontrado->dato : NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    return buscar_nodo((abb_t*)arbol, clave, NULL);
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cant;
}

void _abb_destruir(abb_nodo_t* actual, abb_destruir_dato_t destruir_dato){

    if (!actual) return;

    _abb_destruir(actual->izq, destruir_dato);
    _abb_destruir(actual->der, destruir_dato);

    if (destruir_dato) destruir_dato(actual->dato);

    nodo_destruir(actual);
}

void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}

bool _abb_in_order(abb_nodo_t* actual, bool visitar(const char *, void *, void *), void* extra){

    if (!actual) return true;

    if (!_abb_in_order(actual->izq, visitar, extra)) return false;

    if (!visitar(actual->clave, actual->dato, extra)) return false;

    if (!_abb_in_order(actual->der, visitar, extra)) return false;

    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    _abb_in_order(arbol->raiz, visitar, extra);
}

bool es_hijo_derecho(const char* clave_padre, const char* clave_hijo, abb_comparar_clave_t cmp){
    return cmp(clave_padre, clave_hijo) < 0;
}

abb_nodo_t* buscar_reemplazante(abb_nodo_t* actual){
    actual = actual->izq;

    while(actual->der) actual = actual->der;

    return actual;
}

void* _abb_borrar(abb_t* arbol, abb_nodo_t* hijo, abb_nodo_t* padre){    

    void* dato_a_devolver = hijo->dato;

    if (hijo->der && hijo->izq){
        abb_nodo_t* reemplazante = buscar_reemplazante(hijo);
        char* copia_clave = _strdup(reemplazante->clave);
        void* nuevo_dato = abb_borrar(arbol, copia_clave);
        arbol->cant++;
        free(hijo->clave);
        hijo->clave = copia_clave;
        hijo->dato = nuevo_dato;
    }else {
        abb_nodo_t* nieto = hijo->der ? hijo->der : hijo->izq;

        if (padre){

            if (es_hijo_derecho(padre->clave, hijo->clave, arbol->cmp)) padre->der = nieto; else padre->izq = nieto;
            
        } else{
            arbol->raiz = nieto;
        }

        nodo_destruir(hijo);
    }

    return dato_a_devolver;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    arbol->cant--;

    void** puntero_a_padre;
    void* padre = NULL;
    puntero_a_padre = &padre;
    abb_nodo_t* hijo = buscar_nodo(arbol, clave, (abb_nodo_t**)puntero_a_padre);

    if (!hijo){
        arbol->cant++;
        return NULL;
    }
    
    return _abb_borrar(arbol, hijo, (abb_nodo_t*)*puntero_a_padre);
}

/* *******************
   *     ABB ITER    *
   ********************/

typedef struct abb_iter{
    pila_t* pila;
} abb_iter_t;

void apilar_izquierdos(pila_t* pila, abb_nodo_t* actual){

    while (actual){
        pila_apilar(pila, actual);
        actual = actual->izq;
    }

}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));

    if (!iter) return NULL;

    iter->pila = pila_crear();

    if (!iter->pila){
        free(iter);
        return NULL;
    }

    apilar_izquierdos(iter->pila, arbol->raiz);
    return iter;
}
bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

bool abb_iter_in_avanzar(abb_iter_t *iter){

    if (abb_iter_in_al_final(iter)) return false;
    
    apilar_izquierdos(iter->pila, ((abb_nodo_t*)pila_desapilar(iter->pila))->der);
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila) ? NULL : ((abb_nodo_t*)pila_ver_tope(iter->pila))->clave;
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}
