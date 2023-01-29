#ifndef STRING_READER_H
#define STRING_READER_H

#include <stdbool.h>

// End Of String
#define EOS 0

typedef struct stringReader StringReader;

StringReader* stringReader_init(const char* stringRef);
void stringReader_free(StringReader* sr);

bool stringReader_isEOS(StringReader* sr);
void stringReader_moveNext(StringReader* sr);
char stringReader_getCurrent(StringReader* sr);
char* stringReader_getSelected(StringReader* sr);
void stringReader_ignoreSelected(StringReader* sr);
TokenLocation stringReader_getLocation(StringReader* sr);

#endif