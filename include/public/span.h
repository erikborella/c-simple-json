#ifndef SPAN_H
#define SPAN_H

#include <stdlib.h>

typedef struct span {
    const void* ptr;
    size_t size;
} Span;

#endif