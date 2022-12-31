#include "token.h"
#include "lexer.h"

int main() {
    Lexer* l = lexer_init("123 1234.10");

    JToken token = lexer_getNext(l);
    token = lexer_getNext(l);

    lexer_free(l);

    return 0;
}