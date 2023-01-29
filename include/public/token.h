#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdbool.h>

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
    EOS_T,
};

typedef struct {
    size_t line;
    size_t column;
} TokenPosition;

typedef struct {
    TokenPosition start;
    TokenPosition end;
} TokenLocation;

typedef struct jToken {
    enum JTokenType type;
    TokenLocation location;
    union {
        char* string_content;
        int int_content;
        double float_content;
        bool boolean_content;
    } content;
} JToken;

#endif