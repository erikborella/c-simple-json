#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct lexer Lexer;

Lexer* lexer_init(const char* stringRef);
void lexer_free(Lexer* l);

JToken lexer_getNext(Lexer* l);

#endif