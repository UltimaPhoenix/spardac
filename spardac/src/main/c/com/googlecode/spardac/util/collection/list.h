#ifndef LIST_H_
#define LIST_H_

/*
 * Represent a list element.
 */
typedef struct element {
	void  *obj;
	struct element *next;
	struct element *prev;
} list_element;

/*
 * Represent a list.
 */
typedef struct list {
	list_element *head;
	list_element *tail;
	int number_of_element;
} *list_t;


/*
 * Create new list.
 */
list_t new_list();

/*
 * Free the list.
 */
void free_list(const list_t list);

/*
 * Free the list calling free on all list element.
 */
void free_list_and_object(const list_t list);

/*
 * Return the number of element in list.
 */
int size_list(const list_t list);

/*
 * Add an element to the list. The element is added at the end of this list.
 */
void add_element(const list_t list, void *elementPtr);

/*
 * Add an element to the list only if absent. The element is added at the end of this list.
 */
void add_element_if_absent(const list_t list, void *elementPtr, int (*compare_function) (void* a, void* b));

/*
 * Remove an element from the list.
 */
int remove_element(const list_t list, void *element_to_remove);

/*
 * Remove an element from the list with the respect compare function.
 */
int remove_element_function(const list_t list, void *element_to_remove, int (*compare_function) (void* a, void* b));

/*
 * Iterator of a list.
 */
typedef struct iterator {
	list_element *element;
} *iterator_t;

/*
 * Create new list iterator.
 */
iterator_t new_iterator(const list_t list);

/*
 * Free the iterator.
 */
void free_iterator(const iterator_t it);

/*
 * Return 1 if there are element to iterate, 0 otherwise.
 */
int has_next(const iterator_t it);

/*
 * Return the next element and move iterator to next element.
 * Return NULL if the end of the list is reached ended.
 */
void* next(const iterator_t it);

#endif /*LIST_H_*/
