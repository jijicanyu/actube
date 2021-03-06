#include <stdlib.h>
#include <string.h>

#include "mlist.h"

mlist_t mlist_create(int (*cmp) (const void *v1, const void *v2))
{
	struct mlist * l = malloc(sizeof(struct mlist));
	if (!l)
		return NULL;
	memset(l, 0, sizeof(struct mlist));
	l->cmp = cmp;
	return l;
}

struct mlist_elem * mlist_append(mlist_t l, void *data)
{
	
	struct mlist_elem **n = &l->first;
	while (*n != NULL)
		n = &(*n)->next;
	*n = malloc(sizeof(struct mlist_elem));
	if (!*n)
		return NULL;

	(*n)->data = data;
	(*n)->next = NULL;

	return *n;
}

struct mlist_elem * mlist_prepend(mlist_t l, void *data){
	
}




struct mlist_elem *mlist_find(mlist_t l, struct mlist_elem *start, void *data)
{
	struct mlist_elem *e;
	if (start)
		e = start;
	else
		e = l->first;

	while (e) {
		if (l->cmp(e->data, data) == 0)
			return e;
		e = e->next;
	}

	return NULL;
}


struct mlist_elem * mlist_replace(mlist_t l, struct mlist_elem *start, void *data)
{
	struct mlist_elem *e;
	if (start)
		e = start;
	else
		e = l->first;

	struct mlist_elem * f = mlist_find(l,e,data);
	if (!f)
		return NULL;

	f->data = data;
	return f;
}

