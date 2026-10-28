/*
    PREFACE
        I needed a change of style, so now I use 4 spaces for tab instead of 2,
        And also I put a newline before the function declaration and the bracket.
        These two changes make the code have more space to breathe

    COMPILE
        That said, build with:
            cc -O2 -std=c99 -Wall -Wextra -Wpedantic -Werror da.c

    made with ♡ by heliumhydride
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


typedef struct {
    int *items;      // points to the data of the array itself
    size_t count;    // size of the array
    size_t capacity; // basically how much memory we give to it currently, can grow and be superior to count
} Numbers;

void da_append(Numbers* da, int x)
{
    if(da->count++ >= da->capacity) {
        da->capacity *= 2;
        if(da->capacity == 0) da->capacity = 1;
        da->items = realloc(da->items, da->capacity * sizeof(*da->items));
    }

    da->items[da->count] = x; // note: da->count was already post-incremented
}

void da_pop(Numbers* da)
{
    // TODO maybe we could save memory by dividing capacity by 2 if possible
    if(da->count > 0) da->count--;
}

void da_insert(Numbers* da, int x, size_t pos)
{
    if(pos > da->count) return;

    if(da->count++ >= da->capacity) {
        da->capacity *= 2;
        if(da->capacity == 0) da->capacity = 1;
        da->items = realloc(da->items, da->capacity * sizeof(*da->items));
    }

    for(size_t i = da->count - 1; i > pos; --i) {
        da->items[i] = da->items[i-1];
    }

    da->items[pos] = x;
}

void da_delete(Numbers* da, size_t pos)
{
    if(pos > da->count) return;

    for(size_t i = da->count - 1; i > pos; --i) {
        da->items[i-1] = da->items[i];
    }

    --da->count;
}

int main(void)
{
    Numbers da = {0};
    da_append(&da, 1);
    da_append(&da, 2);
    da_append(&da, 3);
    da_insert(&da, 69, 2);
    da_insert(&da, 420, 0);

    printf("da.count = %zu\n", da.count);
    printf("da.items = { ");
    for(size_t i = 0; i < da.count; i++)
        printf("%d, ", da.items[i]);
    printf("}\n");
    printf("da.capacity = %zu\n", da.capacity);

    free(da.items);
}
