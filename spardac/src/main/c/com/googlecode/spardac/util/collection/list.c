#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include "list.h"

/*
 * Dealloca l'elemento.
 */
void free_element(list_element *element) {
	free(element);
}

/*
 * Dealloca l'elemento ed dealloca anche l'oggetto puntato.
 */
void free_element_and_object(list_element *elemento) {
	void* to_destroy;
	to_destroy = elemento->obj;
	free(elemento);
}

/*
 * Consente di creare un iteratore per una lista.
 */
iterator_t create_iterator(const list_t listPtr) {
	iterator_t it = (iterator_t) malloc(sizeof(struct iterator));
	it->element = listPtr->head;
	return it;
}

/*
 * Dealloca l'iteratore.
 */
void free_iterator(const iterator_t iterator) {
	free(iterator);
}

/*
 * Ritorna 1 se ci sono elementi da iterare e 0 altrimenti.
 */
int has_next(const iterator_t it) {
	return it->element != NULL;
}

/*
 * Ritorna l'elemento corrente della lista e sposta l'iteratore sul prossimo elemento. 
 */
void* next(const iterator_t it) {
	void *obj;
	obj = it->element->obj;
	it->element = it->element->next;
	return obj;
}

/*
 * Crea una lista.
 */
list_t alloc_list() {
	list_t lista = malloc(sizeof(struct list));
	lista->head = NULL;
	lista->tail = NULL;
	lista->number_of_element = 0;
	return lista;
}

/*
 * Distrugge la lista ma rimangono appesi i riferimenti degli oggetti.
 */
void free_list(const list_t lista) {
	iterator_t it = create_iterator(lista);
	while(has_next(it)) {
		list_element *element;
		element = it->element;
		next(it);
		free(element);
	}
	free_iterator(it);
	free(lista);
}

/*
 * Distrugge la lista e tutti gli oggetti riferimento invocando una free anche su di essi.
 */
void free_list_and_object(const list_t lista) {
	iterator_t it = create_iterator(lista);
	while(has_next(it)) {
		list_element *element;
		element = it->element;
		free_element_and_object(element);
		next(it);
	}
	free_iterator(it);
	free(lista);
}

/*
 * Ritorna il numero di elementi nella lista.
 */
int size_list(const list_t lista) {
	return lista->number_of_element;
}

/*
 * Crea un elemento per la lista.
 */
list_element* create_element() {
	list_element *elemento;
	elemento = malloc(sizeof(list_element));
	elemento->obj = NULL;
	elemento->next = NULL;
	elemento->prev = NULL;
	return elemento;
}

/*
 * Aggiunge l'elemento alla lista
 */
void add_element(const list_t lista, void *elementPtr) {
	list_element *nuova_coda;
	nuova_coda = create_element();
	nuova_coda->obj = elementPtr;
	if(lista->head == NULL) { //se la lista era vuota imposto testa e coda
		lista->head = nuova_coda;
		lista->tail = nuova_coda;
	} else {
		nuova_coda->prev = lista->tail;
		lista->tail->next = nuova_coda;
		lista->tail = nuova_coda;
	}
	lista->number_of_element++; // aggiorno il contatore di elementi
}

/*
 * Aggiunge l'elemento alla lista solo se assente secondo la rispettiva funzione di comparazione.
 */
void add_element_if_absent(const list_t lista, void *elementPtr, int (*compare_function) (void* a, void* b)) {
	iterator_t iterator = create_iterator(lista);
	while(has_next(iterator))
		if(compare_function(next(iterator), elementPtr)) {
			free_iterator(iterator);
			return;
		}
	free_iterator(iterator);
	add_element(lista, elementPtr);
}


/*
 * Rimuove l'elemento dalla lista.
 */
int remove_element(const list_t lista, void *object_to_remove) {
	iterator_t it = create_iterator(lista);
	int i;
	for(i=1; has_next(it); i++) {
		list_element *elemento_corrente;
		elemento_corrente = it->element;		// prendo l'elemento della lista corrente
		void *current_object;
		current_object = next(it);				// prendo l'oggetto corrente e vado avanti
		if(current_object == object_to_remove) {	// ho trovato l'oggetto che cercavo
			if(i==1) {									// se l'elemento è in testa
				lista->head = elemento_corrente->next;		// cambio la testa
				elemento_corrente->next->prev = NULL;		// il successivo diventa la nuova testa
			}
			else if(i==lista->number_of_element) {		// se l'elemento è in coda
				lista->tail = elemento_corrente->prev;		// cambio la coda
				elemento_corrente->prev->next = NULL;		// il precedente diventa la nuova coda
			}
			else {										// se l'elemento è in mezzo
				elemento_corrente->next->prev = elemento_corrente->prev;	// aggiorno riferimento indietro del prossimo
				elemento_corrente->prev->next = elemento_corrente->next;	// aggiorno riferimento avanti del precedente
			}
			free_element(elemento_corrente);
			free_iterator(it);
			lista->number_of_element--;
			return 1;
		}
	}
	free_iterator(it);							// rimuovo iteratore
	return 0;
}

/*
 * Rimuove l'elemento dalla lista applicando la funziopne di verifica di uguaglianza.
 */
int remove_element_function(const list_t lista, void *object_to_remove, int (*compare_function) (void* a, void* b)) {
	iterator_t it = create_iterator(lista);
	int i;
	for(i=1; has_next(it); i++) {
		list_element *elemento_corrente;
		elemento_corrente = it->element;		// prendo l'elemento della lista corrente
		void *current_object;
		current_object = next(it);				// prendo l'oggetto corrente e vado avanti
		if((*compare_function) (current_object, object_to_remove) == 0) {	// ho trovato l'oggetto che cercavo
			if(i==1) {									// se l'elemento è in testa
				lista->head = elemento_corrente->next;		// cambio la testa
				elemento_corrente->next->prev = NULL;		// il successivo diventa la nuova testa
			}
			else if(i==lista->number_of_element) {		// se l'elemento è in coda
				lista->tail = elemento_corrente->prev;		// cambio la coda
				elemento_corrente->prev->next = NULL;		// il precedente diventa la nuova coda
			}
			else {										// se l'elemento è in mezzo
				elemento_corrente->next->prev = elemento_corrente->prev;	// aggiorno riferimento indietro del prossimo
				elemento_corrente->prev->next = elemento_corrente->next;	// aggiorno riferimento avanti del precedente
			}
			free_element(elemento_corrente);
			lista->number_of_element--;
			free_iterator(it);							// rimuovo iteratore
			return 1;
		}
	}
	return 0;
}

