#ifndef TOKEN_H
#define TOKEN_H

#include "span.h"

enum JTokenType {
    NUMBER,
    STRING,
    TRUE_T,
    FALSE_T,
    OPEN_SQUARE_BRACKETS,
    CLOSE_SQUARE_BRACKETS,
    OPEN_CURLY_BRACKETS,
    CLOSE_CURLY_BRACKETS,
    COLON,
    COMMA,
    NULL_T,
    EOF_T,
};

typedef struct jToken {
    enum JTokenType type;
    Span content;
} JToken;

#endif