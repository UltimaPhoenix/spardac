#ifndef LIST_H_
#define LIST_H_

/*
 * Rappresente un elemento della lista.
 */
typedef struct element {
	void  *obj;
	struct element *next;
	struct element *prev;
} list_element;

/*
 * Rappresenta il tipo lista.
 */
typedef struct list {
	list_element *head;
	list_element *tail;
	int number_of_element;
} *list_t;


/*
 * Crea una lista.
 */
list_t alloc_list();                       //alloca una lista

/*
 * Libera la lista.
 */
void free_list(const list_t lista);            //dealloca una lista

/*
 * Libera la lista e tutti i suoi elementi.
 */
void free_list_and_object(const list_t lista);

/*
 * Ritorna il numero di elementi nella lista.
 */
int size_list(const list_t lista);

/*
 * Aggiunge l'elemento alla lista
 */
void add_element(const list_t lista, void *elementPtr);   //aggiunge in fondo un elemento

/*
 * Aggiunge l'elemento alla lista solo se assente secondo la rispettiva funzione di comparazione.
 */
void add_element_if_absent(const list_t lista, void *elementPtr, int (*comapare_function) (void* a, void* b));

/*
 * Rimuove l'elemento dalla lista.
 */
int remove_element(const list_t lista, void *element_to_remove); //rimuove un elemento

/*
 * Rimuove l'elemento dalla lista applicando la funziopne di verifica di uguaglianza.
 */
int remove_element_function(const list_t lista, void *element_to_remove, int (*compare_function) (void* a, void* b)); //rimuove un elemento

/*
 * Tipo iteratore per iterare sugli elementi di una lista
 */
typedef struct iterator {
	list_element *element;
} *iterator_t;

/*
 * Consente di creare un iteratore per una lista.
 */
iterator_t create_iterator(const list_t lista); //crea un iteratore sulla lista

/*
 * Dealloca l'iteratore
 */
void free_iterator(const iterator_t it);      //dealloca un iteratore

/*
 * Ritorna 1 se ci sono elementi da iterare e 0 altrimenti.
 */
int has_next(const iterator_t it);            //iterazione finita?

/*
 * Ritorna l'elemento corrente della lista e sposta l'iteratore sul prossimo elemento. 
 */
void* next(const iterator_t it);             //prossimo elemento, NULL se finiti

#endif /*LIST_H_*/
