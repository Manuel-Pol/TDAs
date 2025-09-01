#include "pila.h"

#include <stdlib.h>

/*Constantes globales*/
#define CAPACIDAD_INICIAL 10
#define PROPORCION_PARA_ACHICAR 4
#define PROPORCION_REDIMENSION 2


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/


pila_t *pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL){
        return NULL;
    }

    pila->capacidad = CAPACIDAD_INICIAL;
    pila->cantidad = 0;
    pila->datos = malloc(pila->capacidad * sizeof(void*));
    if (pila->datos == NULL){
        free(pila);
        return NULL;
    }
    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

// Agranda el tamaño de la pila.
// Pre: la pila fue creada.
// Post: la pila duplicó su tamaño (si recibe true), o lo redujo a la mitad (si recibe false),
// y devuelve true, o la pila queda intacta y devuelve false si no se pudo cambiar el tamaño.
static bool redimensionar_pila(pila_t *pila, size_t nueva_capacidad){
    void **datos_nuevos = realloc(pila->datos, nueva_capacidad * sizeof(void*));
    if (datos_nuevos == NULL){
        return false;
    }
    pila->capacidad = nueva_capacidad;
    pila->datos = datos_nuevos;
    return true;
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
    }

void *pila_ver_tope(const pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    return pila->datos[pila->cantidad-1];
}

bool pila_apilar(pila_t *pila, void *valor){
    if (pila->cantidad == pila->capacidad){
        size_t nueva_capacidad = pila->capacidad * PROPORCION_REDIMENSION;
        bool pudo_redimensionar = redimensionar_pila(pila, nueva_capacidad);
        if (!pudo_redimensionar){
            return false;
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    pila->cantidad--;
    void *elemento = pila->datos[pila->cantidad];
    if (pila->cantidad * PROPORCION_PARA_ACHICAR <= pila->capacidad){
        size_t nueva_capacidad = pila->capacidad / PROPORCION_REDIMENSION;
        if (nueva_capacidad < CAPACIDAD_INICIAL){
            nueva_capacidad = CAPACIDAD_INICIAL;
        }
        redimensionar_pila(pila, nueva_capacidad);
    }
    return elemento;
}
