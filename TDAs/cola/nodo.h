#ifndef NODO_H
#define NODO_H


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct nodo;
typedef struct nodo nodo_t;

/* ******************************************************************
 *                    PRIMITIVA DEL NODO
 * *****************************************************************/

// Crea una nodo.
// Post: devuelve un nuevo nodo vacío.
nodo_t* nodo_crear(void *valor);

// Obtiene el valor del dato del nodo.
// Pre: el nodo fue creado
void *nodo_ver_dato(nodo_t *nodo);

// Determina el proximo del nodo.
// Pre: ambos nodos, tanto el actual como el ingresado, fueron creados.
// Post: el proximo del nodo actual es el ingresado.
void nodo_proximo(nodo_t *actual, nodo_t *proximo);

// Obtiene el proximo nodo al actual.
// Pre: el nodo actual fue creado.
// Post: se devolvió el proximo del actual, si existe, en caso contrario
// se devolvió NULL.
nodo_t *nodo_ver_proximo(nodo_t *nodo);

// Destruye el nodo.
// Pre: el nodo fue creado.
// Post: el nodo fue destruido.
void nodo_destruir(nodo_t *nodo);

#endif
