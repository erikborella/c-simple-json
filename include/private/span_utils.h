#ifndef SPAN_UTILS_H
#define SPAN_UTILS_H

#include "span.h"

void* span_loadContent(Span span, size_t contentSize);
char* span_loadContentAsString(Span span);

#endif