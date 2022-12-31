#include <string.h>

#include "span_utils.h"
#include "span.h"

void* span_loadContent(Span span, size_t contentSize) {
    const size_t contentAllocSize = span.size * contentSize;

    void* content = malloc(contentAllocSize);

    memcpy(content, span.ptr, contentAllocSize);

    return content;
}

char* span_loadContentAsString(Span span) {
    const size_t contentAllocSize = span.size * sizeof(char);

    // +1 to include end of string (0)
    char* content = malloc(contentAllocSize + 1);

    memcpy(content, span.ptr, contentAllocSize);

    // ensures that the end of string will be present
    content[span.size] = 0;

    return content;
}