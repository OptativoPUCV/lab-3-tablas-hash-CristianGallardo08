#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap
{
    Pair **buckets;
    long size;     // cantidad de datos/pairs en la tabla
    long capacity; // capacidad de la tabla
    long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value)
{
    Pair *new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash(char *key, long capacity)
{
    unsigned long hash = 0;
    char *ptr;
    for (ptr = key; *ptr != '\0'; ptr++)
    {
        hash += hash * 32 + tolower(*ptr);
    }
    return hash % capacity;
}

int is_equal(void *key1, void *key2)
{
    if (key1 == NULL || key2 == NULL)
        return 0;
    if (strcmp((char *)key1, (char *)key2) == 0)
        return 1;
    return 0;
}

void insertMap(HashMap *map, char *key, void *value)
{
    int pos = hash(key, map->capacity);
    // int ogPos = pos;
    while (map->buckets[pos] != NULL && map->buckets[pos]->key != key)
    {
        if (is_equal(map->buckets[pos]->key, key))
            return;
        pos = (pos + 1) % map->capacity;
    }
    map->buckets[pos] = createPair(key, value);
    map->size++;
    map->current = pos;
}

void enlarge(HashMap *map)
{
    enlarge_called = 1; // no borrar (testing purposes)
}

HashMap *createMap(long capacity)
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));

    map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));

    map->capacity = capacity;
    map->size = 0;
    map->current = -1;
    return map;
}

void eraseMap(HashMap *map, char *key)
{
}

Pair *searchMap(HashMap *map, char *key)
{
    int pos = hash(key, map->capacity);
    int ogPos = pos;
    while (map->buckets != NULL)
    {
        if (map->buckets[pos]->key != NULL && is_equal(map->buckets[pos]->key, key))
        {
            map->current = pos;
            return map->buckets[pos];
        }
        pos = (pos + 1) % map->capacity;
        if (pos == ogPos)
            break;
    }

    return NULL;
}

Pair *firstMap(HashMap *map)
{
    for (long i = 0; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair *nextMap(HashMap *map)
{
    for (long i = map->current + 1; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}
