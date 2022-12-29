#include <stdbool.h>
#include <ctype.h>

#include "span.h"
#include "lexer.h"

struct lexer {
    const char* stringRef;
};

Lexer* lexer_init(const char* stringRef) {
    Lexer* l = (Lexer*) malloc(sizeof(Lexer));

    if (l != NULL) {
        l->stringRef = stringRef;
    }

    return l;
}

void lexer_free(Lexer* l) {
    free(l);
}

JToken lexer_getNext(Lexer* l) {
    JToken t;
    bool tokenFound;

    do {
        tokenFound = false;
        const char currentChar = *l->stringRef;

        switch (currentChar) {
            case '{':
                t.type = OPEN_CURLY_BRACKETS;
                l->stringRef++;
                break;

            case '}':
                t.type = CLOSE_CURLY_BRACKETS;
                l->stringRef++;
                break;

            case '[':
                t.type = OPEN_SQUARE_BRACKETS;
                l->stringRef++;
                break;

            case ']':
                t.type = CLOSE_SQUARE_BRACKETS;
                l->stringRef++;
                break;

            case ':':
                t.type = COLON;
                l->stringRef++;
                break;

            case ',':
                t.type = COMMA;
                l->stringRef++;
                break;

            case 0:
                t.type = EOF_T;
                l->stringRef++;
                break;
        }

    } while (!tokenFound);

    return t;
}