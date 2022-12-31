#include <stdbool.h>
#include <ctype.h>

#include "span.h"
#include "lexer.h"
#include "span_utils.h"

// End Of String
#define EOS 0

struct lexer {
    const char* stringRef;
};

static size_t moveWhileIsNumber(Lexer* l) {
    size_t moveCounter = 0;

    while (*l->stringRef != EOS && isdigit(*l->stringRef)) {
        l->stringRef++;
        moveCounter++;
    }

    const char current = *l->stringRef;

    if (isalpha(current))
        exit(1);

    return moveCounter;
}

static JToken getIntNumber(Lexer* l, Span span) {
    char* str = span_loadContentAsString(span);
    int value = strtoll(str, NULL, 10);

    JToken t = {
        .type = INT,
        .content.int_content = value
    };

    free(str);

    return t;
}

static JToken getFloatNumber(Lexer* l, Span span) {
    l->stringRef++;
    span.size++;

    if (!isdigit(*l->stringRef))
        exit(1);

    int floatMovedCounter = moveWhileIsNumber(l);
    span.size += floatMovedCounter;

    char* str = span_loadContentAsString(span);
    float value = strtod(str, NULL);

    JToken t = {
        .type = FLOAT,
        .content.float_content = value
    };

    free(str);

    return t;
}

static JToken getNumber(Lexer* l) {
    Span currentNumber = {
        .ptr = l->stringRef,
        .size = 0
    };

    size_t intMovedCounter = moveWhileIsNumber(l);
    currentNumber.size += intMovedCounter;

    if (*l->stringRef == '.')
        return getFloatNumber(l, currentNumber);
    else
        return getIntNumber(l, currentNumber);
}

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
        tokenFound = true;
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

            default:
                if (isdigit(currentChar))
                    t = getNumber(l);
                else {
                    l->stringRef++;
                    tokenFound = false;
                }

                break;
                
        }

    } while (!tokenFound);

    return t;
}