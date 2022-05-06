#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef size_t T;

int compare0(T *el1, T *el2)
{
    return *el2 - *el1;
};

int compare1(T *el1, T *el2)
{
    return *el1 - *el2;
};

struct heap
{
    T *body;

    size_t allocated;
    size_t nodes;

    int(* comp)(T *, T *);
};

typedef struct heap heap;

heap *heap_create(size_t count, int type)
{
    heap *new_heap = (heap *)calloc(1, sizeof(heap));

    new_heap->body      = (T *)calloc(count, sizeof(T));
    new_heap->allocated = count;
    new_heap->nodes     = 0; 
    new_heap->comp      = type == 0 ? compare0 : compare1;

    return new_heap;
}

void swap(T *el1, T *el2)
{
   //assert(el1 != nullptr);
   //assert(el2 != nullptr);

    T el = *el1;

    *el1 = *el2;
    *el2 = el;
}

void sift_up(heap *cur_heap, size_t index)
{
   //assert(cur_heap != nullptr);
    
    while (index > 0)
	{ 
		if(cur_heap->comp(cur_heap->body + ((index - 1) >> 1), cur_heap->body + index) < 0)
    	{        
        	swap(&cur_heap->body[index], &cur_heap->body[(index - 1) >> 1]);
        
        	index = (index - 1) >> 1;
    	}

		else
		{
			break;
		}
	}
    return;
}

void sift_down(heap *cur_heap, size_t index)
{
   //assert(cur_heap != nullptr);

    while ((index + index + 1) + 1 < cur_heap->nodes)
    {
        if (cur_heap->comp(cur_heap->body + (index + index + 1), cur_heap->body + index) > 0 || cur_heap->comp(cur_heap->body + (index + index + 1) + 1, cur_heap->body + index) > 0)
        {
            if (cur_heap->comp(cur_heap->body + (index + index + 1), cur_heap->body + (index + index + 1) + 1) > 0)
            {
                swap(&cur_heap->body[index], &cur_heap->body[index + index + 1]);
                
                index += index + 1;
            }

            else 
            {
                swap(&cur_heap->body[index], &cur_heap->body[(index + index +1) + 1]);
                
                index += index + 1 + 1;
            }
        }

        else
        {
            break;
        }
    }

    if ((index + index + 1) < cur_heap->nodes)
    {
        if (cur_heap->comp(cur_heap->body + (index + index + 1), cur_heap->body + index) > 0)
        {
            swap(&cur_heap->body[index], &cur_heap->body[index + index + 1]);
        }
    }
}

void heap_add(heap *cur_heap, T *elem)
{
   //assert(cur_heap != nullptr);
   //assert(elem     != nullptr);

    memcpy(cur_heap->body + cur_heap->nodes, elem, sizeof(T)); 
    cur_heap->nodes ++;

    sift_up(cur_heap, cur_heap->nodes - 1);
}

void heap_head_delete(heap *cur_heap)
{
   //assert(cur_heap != nullptr);

    cur_heap->body[0] = cur_heap->body[cur_heap->nodes - 1];
    
    cur_heap->nodes--;

    sift_down(cur_heap, 0);
}

heap* merge(heap *cur_heap1, heap *cur_heap2)
{
   //assert(cur_heap1 != nullptr);
   //assert(cur_heap2 != nullptr);
    
    cur_heap1->body = (T *)realloc(cur_heap1->body, (cur_heap1->allocated + cur_heap2->allocated) * sizeof(T));

    memcpy(cur_heap1->body + cur_heap1->nodes, cur_heap2->body, cur_heap2->nodes * sizeof(T));
    
    cur_heap1->allocated = cur_heap1->allocated + cur_heap2->allocated;
    cur_heap1->nodes     = cur_heap1->nodes     + cur_heap2->nodes;
    
    int half = cur_heap1->nodes >> 1;

    for (int i = half - 1; i > -1; i--)
    {
        sift_down(cur_heap1, i);
    }
    
    cur_heap2->nodes = 0;
    
    return cur_heap1;
}

void heap_delete(heap *cur_heap)
{
    free(cur_heap->body);
    free(cur_heap);
}
