#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "../lista/lista.h"
#include <stdio.h>

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#define CAPACIDAD_INICIAL 23
#define MAX_FACTOR_CARGA 3
#define MIN_FACTOR_CARGA 0.3
#define REDIMENSION 3

//LINK DE LA FUNCION DE HASH
//http://codereview.stackexchange.com/questions/85556/simple-string-hashing-algorithm-implementation


size_t funcion_hash(const char *clave, size_t largo_primo){
	size_t hash = 0x811c9dc5;

	while (*clave){
		hash ^= (unsigned char) *clave++;
		hash *= 0x01000193;
	}

	return (hash % largo_primo);
}

char* _strdup(const char* org) {
    char* copy = malloc(sizeof(char) * (strlen(org) + 1));

    if (!copy) return NULL;

    strcpy(copy, org);
    return copy;
}


/* ******************************************************************
 *                  IMPLEMENTACIONES DE LOS STRUCTS
 * *****************************************************************/

typedef struct hash{
    lista_t** tabla;
    size_t cant;
    size_t cap;
    hash_destruir_dato_t hash_destruir;
} hash_t;

typedef struct hash_iter{
    const hash_t* hash;
    size_t indice_actual;
    lista_iter_t* iter_actual; 
} hash_iter_t;

typedef struct campo{
    char* clave;
    void* valor;
} campo_t;

// Crea un struct campo
campo_t* _crear_campo(char* clave, void* valor){
    campo_t* campo = malloc(sizeof(campo_t));
    
    if (!campo) return false;
    
    campo->clave = clave;
    campo->valor = valor;
    return campo;
}

/* ******************************************************************
 *              IMPLEMENTACIONES DE LAS PRIMITIVAS HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));

    if (!hash) return NULL;

    lista_t** tabla = calloc(CAPACIDAD_INICIAL, sizeof(lista_t*));
    
    if (!tabla){
        free(hash);
        return NULL;
    }

    hash->tabla = tabla;
    hash->cap = CAPACIDAD_INICIAL;
    hash->cant = 0;
    hash->hash_destruir = destruir_dato;
    return hash;
}

lista_iter_t* buscar_nodo(const hash_t* hash, size_t indice, const char* clave){
    lista_t* lista_actual = hash->tabla[indice];
    lista_iter_t* iter = lista_iter_crear(lista_actual);

    while(!lista_iter_al_final(iter)){
        char* clave_actual = ((campo_t*)lista_iter_ver_actual(iter))->clave;

        if (strcmp(clave_actual, clave) == 0) break;

        lista_iter_avanzar(iter);
    }
    return iter;
}

bool hash_redimensonar(hash_t* hash, size_t nueva_cap){
    lista_t** nueva_tabla = calloc(nueva_cap, sizeof(lista_t*));
    
    if (nueva_tabla == NULL) return false;
    
    for (int i = 0; i < hash->cap; i++){
        
        if (hash->tabla[i] != NULL){
            lista_t* lista_actual = hash->tabla[i];
            lista_iter_t* iter = lista_iter_crear(lista_actual);
            
            while(!lista_iter_al_final(iter)){
                campo_t* campo_actual = lista_iter_ver_actual(iter);
                size_t nuevo_indice = funcion_hash(campo_actual->clave, nueva_cap);
                
                if (nueva_tabla[nuevo_indice] == NULL){
                    lista_t* nueva_lista = lista_crear();
                    nueva_tabla[nuevo_indice] = nueva_lista;
                }
                
                lista_insertar_ultimo(nueva_tabla[nuevo_indice], campo_actual);
                lista_iter_avanzar(iter);
            }
            
            lista_iter_destruir(iter);
            lista_destruir(lista_actual, NULL);
        }
    }

    free(hash->tabla);
    hash->tabla = nueva_tabla;
    hash->cap = nueva_cap;
    return true;
}

bool manejo_redimension(hash_t* hash){
    double factor_de_carga =  ((double)hash->cant) / (double)(hash->cap);
    size_t nueva_cap = 0;
    bool todo_ok = true;

    if (factor_de_carga > MAX_FACTOR_CARGA) nueva_cap = hash->cap * REDIMENSION;
    
    if (factor_de_carga < MIN_FACTOR_CARGA && hash->cap / REDIMENSION > CAPACIDAD_INICIAL) nueva_cap = hash->cap / REDIMENSION;

    if (nueva_cap != 0) todo_ok = hash_redimensonar(hash, nueva_cap);
    
    return todo_ok;
}

void* hash_obtener(const hash_t *hash, const char *clave){
    size_t indice = funcion_hash(clave, hash->cap);

    if (hash->tabla[indice] == NULL) return NULL;

    void* valor_a_devolver = NULL;
    lista_iter_t* iter = buscar_nodo(hash, indice, clave);

    if (!lista_iter_al_final(iter)) valor_a_devolver = ((campo_t*)lista_iter_ver_actual(iter))->valor;

    lista_iter_destruir(iter);
    return valor_a_devolver;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t indice = funcion_hash(clave, hash->cap);

    if (hash->tabla[indice] == NULL) return false;

    lista_iter_t* iter = buscar_nodo(hash, indice, clave);
    bool pertenecia = false;
    
    if (!lista_iter_al_final(iter)) pertenecia = true;

    lista_iter_destruir(iter);
    return pertenecia;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    
    char* copia_clave = _strdup(clave);
    size_t indice = funcion_hash(copia_clave, hash->cap);
    campo_t* nuevo_campo = _crear_campo(copia_clave, dato);

    if (hash->tabla[indice] == NULL){
        lista_t* nueva_lista = lista_crear();

        if (nueva_lista == NULL){
            free(copia_clave);
            free(nuevo_campo);
            return false;
        }
        
        hash->tabla[indice] = nueva_lista;
    }
    
    lista_iter_t* iter = buscar_nodo(hash, indice, clave);

    if (!lista_iter_al_final(iter)){
        campo_t* campo_actual = lista_iter_ver_actual(iter);

        if (hash->hash_destruir != NULL) hash->hash_destruir(campo_actual->valor);
            
        campo_actual->valor = dato;
        free(copia_clave);
        free(nuevo_campo);
    } else{
        lista_iter_insertar(iter, nuevo_campo);
        hash->cant++;
    }
    
    lista_iter_destruir(iter);
    return manejo_redimension(hash);
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cant;
}

void* hash_borrar(hash_t *hash, const char *clave){
    
    size_t indice = funcion_hash(clave, hash->cap);
    if (hash->tabla[indice] == NULL) return NULL;

    lista_iter_t* iter = buscar_nodo(hash, indice, clave);

    if (!lista_iter_al_final(iter)){
        campo_t* campo_devuelto = lista_iter_borrar(iter);
        void* dato_a_devolver = campo_devuelto->valor;
        free(campo_devuelto->clave);
        free(campo_devuelto);
        lista_iter_destruir(iter);

        lista_t* lista_actual = hash->tabla[indice];
        if (lista_esta_vacia(lista_actual)){
            lista_destruir(lista_actual, NULL);
            hash->tabla[indice] = NULL;
        }

        hash->cant--;
        if (!manejo_redimension(hash)) return NULL;
        return dato_a_devolver;
    }

    lista_iter_destruir(iter);
    return NULL;
}

void hash_destruir(hash_t *hash){
    
    for (int i = 0; i < hash->cap; i++){

        if (hash->tabla[i] != NULL){
            lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);

            while(!lista_iter_al_final(iter)){
                campo_t* campo_actual = (campo_t*)lista_iter_ver_actual(iter);
                free(campo_actual->clave);

                if (hash->hash_destruir != NULL) hash->hash_destruir(campo_actual->valor);

                free(campo_actual);
                lista_iter_borrar(iter);
            }

            lista_iter_destruir(iter);
            lista_destruir(hash->tabla[i], NULL);
        }

    }
    free(hash->tabla);
    free(hash);
}

/* ******************************************************************
 *               IMPLEMENTACION DE PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

bool proximo_iterador_lista(hash_iter_t* hash_iter){

    for (size_t i = (hash_iter->indice_actual)+1; i < hash_iter->hash->cap; i++){

        if (hash_iter->hash->tabla[i] != NULL){
            lista_iter_t* lista_iter = lista_iter_crear(hash_iter->hash->tabla[i]);
            
            hash_iter->indice_actual = i;
            hash_iter->iter_actual = lista_iter;
            
            return lista_iter != NULL;
        }
    }
    hash_iter->indice_actual = 0;
    hash_iter->iter_actual = NULL;
    return true;
}


hash_iter_t* hash_iter_crear(const hash_t *hash){
    hash_iter_t* hash_iter = malloc(sizeof(hash_iter_t));

    if (hash_iter == NULL) return NULL;
    
    hash_iter->hash = hash;
    hash_iter->indice_actual = -1;

    if (!proximo_iterador_lista(hash_iter)){
        free(hash_iter);
        return NULL;
    }

    return hash_iter;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return iter->iter_actual == NULL;
}

bool hash_iter_avanzar(hash_iter_t* iter){
    
    if (iter->iter_actual == NULL) return false;

    lista_iter_avanzar(iter->iter_actual);

    if (!lista_iter_al_final(iter->iter_actual)) return true;

    lista_iter_destruir(iter->iter_actual);
    iter->iter_actual = NULL;

    return proximo_iterador_lista(iter);
}

const char* hash_iter_ver_actual(const hash_iter_t* iter){
    return iter->iter_actual == NULL ? NULL : ((campo_t*)lista_iter_ver_actual(iter->iter_actual))->clave;
}

void hash_iter_destruir(hash_iter_t* iter){

    if (!hash_iter_al_final(iter)) lista_iter_destruir(iter->iter_actual);
    
    free(iter);
}

