#include "stdlib.h"
#include "stdbool.h"

typedef struct abb abb_t;
typedef struct abb_nodo abb_nodo_t;
typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

// Crea el abb, junto con su función de comparación y de destrucción de dato.
// Post: se devolvió el abb.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Guarda un elemento en el abb.
// Pre: el abb fue creado.
// Post: si se guardó el elemento en el arbol devuelve true, caso contrario, devuelve false.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Borra del arbol el elemento con la clave ingresada por parametro, y lo devuelve.
// Pre: el abb fue creado.
// Post: si se encontró el elemento, lo borra y lo devuleve, si no se encontró, devuelve NULL.
void *abb_borrar(abb_t *arbol, const char *clave);

// Obtiene el elemento con la clave del arbol, y lo devuelve.
// Pre: el arbol fue creado.
// Post: si se encontró el elemento lo devuleve, si no se encontró, devuelve NULL.
void *abb_obtener(const abb_t *arbol, const char *clave);

// Devuelve true si la clave ingresada pertenece al abb, false en caso contrario.
// Pre: el abb fue creado.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de elementos en el abb.
// Pre: el abb fue creado.
size_t abb_cantidad(const abb_t *arbol);

// Destruye el abb junto con sus elementos.
// Pre: el abb fue creado.
// Post: el abb fue destruido.
void abb_destruir(abb_t *arbol);

// Itera el abb realizando un recorrido in-order.
// Pre: el abb fue creado.
// Post: se iteró por completo el abb. se aplicó de manera in-order la función visitar a cada elemento.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

typedef struct abb_iter abb_iter_t;

// Crea el iterador in-order del abb.
// Pre: el abb due creado.
// Post: se devolvió el iterador in-order.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza de manera in-order un elemento en el abb. Devuelve booleano.
// Pre: el iterador del abb fue creado.
// Post: devuelve true si se pudo avanzar, false en caso de que no o en caso de que esté al final.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve la clave del actual en el iterador.
// Pre: el iterador del abb fue creado.
// Post: devuelve la clave de actual, si la encontró, sino devuelve NULL.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Indica si el iterador está al final del recorrido in-order del arbol.
// Pre: el iterador del abb fue creado.
// Post si esta al final devuelve true, caso contrario false.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador del arbol.
// Pre: el iterador del abb fue creado.
// Post: el iterador in-order fue destruido.
void abb_iter_in_destruir(abb_iter_t* iter);