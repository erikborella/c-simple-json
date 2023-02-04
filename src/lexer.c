#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#include "lexer.h"
#include "utils/stringReader.h"

struct lexer {
    StringReader* stringReader;
};

static void throwError(Lexer* l, const char* msg, ...) {
    stringReader_ignoreSelected(l->stringReader);
    TokenLocation errorLocation = stringReader_getLocation(l->stringReader);

    fprintf(stderr, "Lexer Error -> L:%ld C:%ld ",
        errorLocation.start.line, errorLocation.start.column);

    va_list arg_ptr;

    va_start(arg_ptr, msg);
    vfprintf(stderr, msg, arg_ptr);
    va_end(arg_ptr);

    exit(1);
}

static TokenLocation getLocationAndMoveToNext(Lexer* l) {
    stringReader_moveNext(l->stringReader);
    TokenLocation location = stringReader_getLocation(l->stringReader);

    stringReader_ignoreSelected(l->stringReader);

    return location;
}

static void moveWhileIsNumber(Lexer* l) {
    while (!stringReader_isEOS(l->stringReader) && isdigit(stringReader_getCurrent(l->stringReader))) {
        stringReader_moveNext(l->stringReader);        
    }

    const char current = stringReader_getCurrent(l->stringReader);

    if (isalpha(current))
        throwError(l, "Expected a number or '.', but found a letter '%c'\n", current);
}

static JToken getFloatNumber(Lexer* l) {
    stringReader_moveNext(l->stringReader);

    const char current = stringReader_getCurrent(l->stringReader);

    if (!isdigit(current))
        throwError(l, "Expected a number after '.', but found '%c'\n", current);

    moveWhileIsNumber(l);

    TokenLocation location = stringReader_getLocation(l->stringReader);
    char *str = stringReader_getSelected(l->stringReader);

    JToken t = {
        .type = FLOAT,
        .content.float_content = strtod(str, NULL),
        .location = location,
    };

    free(str);
    return t;
}

static JToken getIntNumber(Lexer* l) {
    TokenLocation location = stringReader_getLocation(l->stringReader);
    char *str = stringReader_getSelected(l->stringReader);

    JToken t = {
        .type = INT,
        .content.int_content = strtoll(str, NULL, 10),
        .location = location
    };

    free(str);
    return t;
}

static JToken getNumber(Lexer* l) {
    stringReader_moveNext(l->stringReader);

    moveWhileIsNumber(l);

    if (stringReader_getCurrent(l->stringReader) == '.')
        return getFloatNumber(l);
    else
        return getIntNumber(l);
}

static JToken getLiteral(Lexer* l) {
    // Checks for '' or "" in strings
    char stringGuard = stringReader_getCurrent(l->stringReader);

    if (stringGuard == '\'' || stringGuard == '\"') {
        stringReader_moveNext(l->stringReader);
        stringReader_ignoreSelected(l->stringReader);
    } else {
        stringGuard = 0;
    }
    
    while (!stringReader_isEOS(l->stringReader) && 
        ((stringGuard == 0 && !isspace(stringReader_getCurrent(l->stringReader)) && isalnum(stringReader_getCurrent(l->stringReader))) || 
        (stringGuard != 0 && stringReader_getCurrent(l->stringReader) != stringGuard))) {
        stringReader_moveNext(l->stringReader);
    }

    TokenLocation location = stringReader_getLocation(l->stringReader);
    char* stringContent = stringReader_getSelected(l->stringReader);

    enum JTokenType type = STRING;
    union u_jTokenContent content = { .string_content = stringContent };

    // Check for literal constant values (true, false, null)
    if (stringGuard == 0) {
        if (strcmp(stringContent, "true") == 0) {
            type = BOOLEAN;
            content.boolean_content = true;
        }
        else if (strcmp(stringContent, "false") == 0) {
            type = BOOLEAN;
            content.boolean_content = false;
        }
        else if (strcmp(stringContent, "null") == 0) {
            type = NULL_T;
        }
    }

    // As we are no long need the string content of the lexeme for literal constants
    // we can already free then
    if (type != STRING) {
        free(stringContent);
    }

    JToken token = {
        .type = type,
        .content = content,
        .location = location
    };

    if (stringGuard != 0) {
        stringReader_moveNext(l->stringReader);
        stringReader_ignoreSelected(l->stringReader);
    }

    return token;
}

Lexer* lexer_init(const char* stringRef) {
    Lexer* l = (Lexer*) malloc(sizeof(Lexer));

    if (l != NULL) {
        l->stringReader = stringReader_init(stringRef);
    }

    return l;
}

void lexer_free(Lexer* l) {
    stringReader_free(l->stringReader);
    free(l);
}

JToken lexer_getNext(Lexer* l) {
    JToken t;
    bool tokenFound;

    do {
        tokenFound = true;
        const char currentChar = stringReader_getCurrent(l->stringReader);

        switch (currentChar) {
            case '{':
                t.type = OPEN_CURLY_BRACKETS;
                t.location = getLocationAndMoveToNext(l);
                break;
            
            case '}':
                t.type = CLOSE_CURLY_BRACKETS;
                t.location = getLocationAndMoveToNext(l);
                break;

            case '[':
                t.type = OPEN_SQUARE_BRACKETS;
                t.location = getLocationAndMoveToNext(l);
                break;
            
            case ']':
                t.type = CLOSE_SQUARE_BRACKETS;
                t.location = getLocationAndMoveToNext(l);
                break;

            case ':':
                t.type = COLON;
                t.location = getLocationAndMoveToNext(l);
                break;
                
            case ',':
                t.type = COMMA;
                t.location = getLocationAndMoveToNext(l);
                break;

            case EOS:
                t.type = EOS_T;
                t.location = stringReader_getLocation(l->stringReader);
                break;

            default:
                if (isdigit(currentChar))
                    t = getNumber(l);
                else if (isalpha(currentChar) || currentChar == '\'' || currentChar == '\"')
                    t = getLiteral(l);
                else {
                    stringReader_moveNext(l->stringReader);
                    stringReader_ignoreSelected(l->stringReader);

                    tokenFound = false;
                }
                break;
        }

    } while (!tokenFound);

    return t;
}