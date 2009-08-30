#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include "list.h"

/*
 * Free a list_element.
 */
void free_element(list_element *element) {
	free(element);
}

/*
 * free a list_element with calling free on element referred.
 */
void free_element_and_object(list_element *element) {
	void* to_destroy;
	to_destroy = element->obj;
	free(element);
}

/*
 * Create new list iterator.
 */
iterator_t new_iterator(const list_t listPtr) {
	iterator_t it = (iterator_t) malloc(sizeof(struct iterator));
	it->element = listPtr->head;
	return it;
}

/*
 * Free the iterator.
 */
void free_iterator(const iterator_t it) {
	free(it);
}

/*
 * Return 1 if there are element to iterate, 0 otherwise.
 */
int has_next(const iterator_t it) {
	return it->element != NULL;
}

/*
 * Return the next element and move iterator to next element.
 * Return NULL if the end of the list is reached ended.
 */
void* next(const iterator_t it) {
	void *obj;
	obj = it->element->obj;
	it->element = it->element->next;
	return obj;
}

/*
 * Create new list.
 */
list_t new_list() {
	list_t list = malloc(sizeof(struct list));
	list->head = NULL;
	list->tail = NULL;
	list->number_of_element = 0;
	return list;
}

/*
 * Free the list.
 */
void free_list(const list_t list) {
	iterator_t it = create_iterator(list);
	while(has_next(it)) {
		list_element *element;
		element = it->element;
		next(it);
		free(element);
	}
	free_iterator(it);
	free(list);
}

/*
 * Free the list calling free on all list element.
 */
void free_list_and_object(const list_t list) {
	iterator_t it = create_iterator(list);
	while(has_next(it)) {
		list_element *element;
		element = it->element;
		free_element_and_object(element);
		next(it);
	}
	free_iterator(it);
	free(list);
}

/*
 * Return the number of element in list.
 */
int size_list(const list_t list) {
	return list->number_of_element;
}

/*
 * Create a new element in list.
 */
list_element* new_element() {
	list_element *element;
	element = malloc(sizeof(list_element));
	element->obj = NULL;
	element->next = NULL;
	element->prev = NULL;
	return element;
}

/*
 * Add an element to the list. The element is added at the end of this list.
 */
void add_element(const list_t list, void *elementPtr) {
	list_element *new_tail;
	new_tail = new_element();
	new_tail->obj = elementPtr;
	if(list->head == NULL) { //if list is empty, so set head and tail
		list->head = new_tail;
		list->tail = new_tail;
	} else {
		new_tail->prev = list->tail;
		list->tail->next = new_tail;
		list->tail = new_tail;
	}
	list->number_of_element++; // update element counter
}

/*
 * Add an element to the list only if absent. The element is added at the end of this list.
 */
void add_element_if_absent(const list_t list, void *elementPtr, int (*compare_function) (void* a, void* b)) {
	iterator_t it = new_iterator(list);
	while(has_next(it)) {
		if(compare_function(next(it), elementPtr)) {
			free_iterator(it);
			return;
		}
	}
	free_iterator(it);
	add_element(list, elementPtr);
}


/*
 * Remove an element from the list.
 */
int remove_element(const list_t list, void *element_to_remove) {
	iterator_t it = new_iterator(list);
	int i;
	for(i=1; has_next(it); i++) {
		list_element *current_element;
		current_element = it->element;		// get the element from current list
		void *current_object;
		current_object = next(it);				// get current object and go on
		if(current_object == object_to_remove) {	// found object searched
			if(i==1) {								// if element is head of the list
				list->head = current_element->next;		// change the head
				current_element->next->prev = NULL;		// next element become the new head
			} else if(i==list->number_of_element) {	// if the element is tail
				list->tail = current_element->prev;		// change the tail
				current_element->prev->next = NULL;		// the preceding element become the tail
			} else {								// if the element is in the middle
				current_element->next->prev = current_element->prev;	// update the reference of preceding with the next
				current_element->prev->next = current_element->next;	// update the reference of the next with the preceding
			}
			free_element(current_element);
			free_iterator(it);
			list->number_of_element--;
			return 1;
		}
	}
	free_iterator(it);
	return 0;
}

/*
 * Remove an element from the list with the respect compare function.
 */
int remove_element_function(const list_t list, void *element_to_remove, int (*compare_function) (void* a, void* b)) {
	iterator_t it = new_iterator(list);
	int i;
	for(i=1; has_next(it); i++) {
		list_element *current_element;
		current_element = it->element;		// get the element from current list
		void *current_object;
		current_object = next(it);				// get current object and go on
		if((*compare_function) (current_object, object_to_remove) == 0) {	// found object searched
			if(i==1) {								// if element is head of the list
				list->head = current_element->next;		// change the head
				current_element->next->prev = NULL;		// next element become the new head
			} else if(i==list->number_of_element) {	// if the element is tail
				list->tail = current_element->prev;		// change the tail
				current_element->prev->next = NULL;		// the preceding element become the tail
			} else {								// if the element is in the middle
				current_element->next->prev = current_element->prev;	// update the reference of preceding with the next
				current_element->prev->next = current_element->next;	// update the reference of the next with the preceding
			}
			free_element(current_element);
			list->number_of_element--;
			free_iterator(it);
			return 1;
		}
	}
	return 0;
}

