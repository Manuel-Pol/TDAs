#ifndef LISTA_H
#define LISTA_H


#include <stdlib.h>
#include <stdbool.h>

/* ******************************************************************
 *                DEFINICION DEL TIPO DE DATO
 * *****************************************************************/

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Inserta el elemento en la primera posición de la lista. Devuelve true si el elemento
// pudo ser insertado, false en caso contrario.
// Pre: la lista fue creada.
// Post: la primera posición de la lista contiene al elemento insertado.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta el elemento en la ultima posición de la lista. Devuelve true si el elemento
// pudo ser insertado, false en caso contrario.
// Pre: la lista fue creada.
// Post: la ultima posición de la lista contiene al elemento insertado.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del borrado, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: si la lista tenía al menos un elemento devuelve el borrado, si estaba vacía
// devuelve NULL.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Obtiene el largo de la lista.
// Pre: lista fue creada.
// Post: se devolvió el largo.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Se itera la lista de manera automática, hasta que llegue al final, o hasta que la
// visitar devuelva false. La función visitar recibe los elementos de la lista y un
// valor extra, y devulve true en caso de seguir iterando la lista, false en caso
// de cortar la iteración.
// Pre: la lista fue creada.
// Post: se iteró la lista por completo, o hasta donde visitar devolvió false.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador para una lista en particular.
// Pre: la lista fue creada. La lista no puede ser modificada por fuera del iterador
// mientras haya un iterador creado.
// Post: devuelve un iterador.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posición en la lista. Devuelve false en caso de haber llegado al final
// ,true en caso contrario.
// Pre: el iterador fue creado.
// Post: se avanzó una posición en el iterador de la lista
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor del elemento de la lista en donde se encuetra el iterador.
// Si existe un elemento en la posición del iterador, se devuelve el valor del
// primero, si no hay un elemento devuelve NULL.
// Pre: el iterador fue creado.
// Post: se devolvió el elemento de la lista, cuando este existe en la posición actual.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true en caso de que el iterado esté al final de la lista,
// devuelve false en caso contrario.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: el iterador se destruyó.
void lista_iter_destruir(lista_iter_t *iter);

void lista_invertir(lista_t* lista);

// Inserta el elemento pasado por parametro en la posición actual del iterador
// en la lista. Si el iterador está al final, el elemento insertado será el ultimo.
// Devuelve true si el iterador pudo insertar, false en caso contrario.
// Pre: el iterador fue creado.
// Post: se insertó el elemento ingresado en la posición actual del iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Se borra el elemento que se encuentra en la posición actual del iterador
// en la lista. Se devuelve el valor del borrado si este existe en la posición
// actual, en caso contrario devuelve NULL.
// Pre: el iterador fue creado.
// Post: devuelve el elemento borrado.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_lista.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_estudiante(void);

#endif  // LISTA_H