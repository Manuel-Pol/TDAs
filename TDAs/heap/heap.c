#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>

#define CAP_INICIAL 5
#define PROPORCION_AGRANDAR 2
#define PROPORCION_ACHICAR 4

typedef int (*cmp_func_t)(const void *a, const void *b);

typedef struct heap{
    void** arreglo;
    cmp_func_t cmp;
    size_t cap;
    size_t cant;
}heap_t;

bool _heap_redimensionar(heap_t* heap, size_t nueva_capacidad) {

    void** auxiliar = realloc(heap->arreglo, nueva_capacidad * sizeof(void*));

    if (!auxiliar) return false;

    heap->arreglo = auxiliar;
    heap->cap = nueva_capacidad;
    return true;
}

void swap(void** a, void** b){
    void* aux = *a;
    *a = *b;
    *b = aux;
}

void upheap(void** arreglo, int hijo, cmp_func_t cmp){  

    if (hijo == 0) return;

    int padre = (hijo - 1) / 2;
    int comparacion = cmp(arreglo[padre], arreglo[hijo]);
    
    if (comparacion < 0){
        swap(&arreglo[padre], &arreglo[hijo]);
        upheap(arreglo, padre, cmp);
    }
}

int maximo(void** arreglo, int cant, int padre, cmp_func_t cmp){
    int hijo_der = 2*padre + 2;
    int hijo_izq = 2*padre + 1;
    bool es_menor_que_su_izq = (hijo_izq < cant && cmp(arreglo[padre], arreglo[hijo_izq]) < 0);
    bool es_menor_que_su_der = (hijo_der < cant && cmp(arreglo[padre], arreglo[hijo_der]) < 0);
    
    if (!es_menor_que_su_der && !es_menor_que_su_izq) return padre;

    if (es_menor_que_su_der && es_menor_que_su_izq) return cmp(arreglo[hijo_der], arreglo[hijo_izq]) > 0 ? hijo_der : hijo_izq;
    
    return es_menor_que_su_izq ? hijo_izq : hijo_der;
}

void downheap(void** arreglo, int cant, int padre, cmp_func_t cmp){
    int max = maximo(arreglo, cant, padre, cmp);

    if (max != padre){
        swap(&arreglo[padre], &arreglo[max]);
        downheap(arreglo, cant, max, cmp);
    }
}

void heapify (void** arreglo, int cantidad, cmp_func_t cmp){

    for (int i = cantidad/2 + 1; i >= 0; i--) downheap(arreglo, cantidad, i, cmp);

}

heap_t* heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));

    if (!heap) return NULL;

    heap->arreglo = malloc(sizeof(void*)*CAP_INICIAL);

    if (!heap->arreglo){
        free(heap);
        return NULL;
    }

    heap->cap = CAP_INICIAL;
    heap->cant = 0;
    heap->cmp = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));

    if (!heap) return NULL;

    size_t capacidad = n > CAP_INICIAL ? 2*n : CAP_INICIAL;
    heap->arreglo = malloc(sizeof(void*) * capacidad);

    if (!heap->arreglo){
        free(heap);
        return NULL;
    }

    heap->cap = capacidad;
    heap->cant = n;
    heap->cmp = cmp;

    for (size_t i = 0; i < n; i++) heap->arreglo[i] = arreglo[i];

    heapify(heap->arreglo, (int)n, cmp);
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){

    if (destruir_elemento) for (size_t i = 0; i < heap->cant; i++) destruir_elemento(heap->arreglo[i]);

    free(heap->arreglo);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap->cant == 0;
}

void *heap_ver_max(const heap_t *heap){
    return heap_esta_vacio(heap) ? NULL : heap->arreglo[0];
}

bool heap_encolar(heap_t *heap, void *elem){

    if (heap_cantidad(heap) == heap->cap){
        if (!_heap_redimensionar(heap, heap->cap * PROPORCION_AGRANDAR)) return false;
    }

    heap->arreglo[heap->cant] = elem;
    upheap(heap->arreglo, (int)heap->cant, heap->cmp);
    heap->cant++;
    return true;
}

void *heap_desencolar(heap_t *heap){

    if (heap_esta_vacio(heap)) return NULL;

    void* dato_a_devolver = heap->arreglo[0];
    swap(&heap->arreglo[0], &heap->arreglo[heap->cant - 1]);
    heap->cant--;

    downheap(heap->arreglo, (int)heap->cant, 0, heap->cmp);

    if (heap_cantidad(heap) * PROPORCION_ACHICAR < heap->cap && (size_t) (heap->cap / PROPORCION_ACHICAR) > CAP_INICIAL){
        if (!_heap_redimensionar(heap, (size_t)(heap->cap / PROPORCION_ACHICAR))) return NULL;
    }

    return dato_a_devolver;
}

void _heap_sort(void* elementos[], int ultimo, cmp_func_t cmp){

    if (ultimo == 0) return;

    swap(&elementos[0], &elementos[ultimo]);
    ultimo--;
    downheap(elementos, ultimo + 1, 0, cmp);
    _heap_sort(elementos, ultimo, cmp);
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos, (int)cant, cmp);
    _heap_sort(elementos, (int)cant -1, cmp);
}

