#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include "span.h"

enum JTokenType {
    INT,
    FLOAT,
    BOOLEAN,
    STRING,
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
    union {
        Span raw_content;
        int int_content;
        double float_content;
        bool boolean_content;
    } content;
} JToken;

#endif