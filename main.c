#include "token.h"
#include "lexer.h"

int main() {
    Lexer* l = lexer_init("'TESTE'");

    JToken token;

    do {
        token = lexer_getNext(l);
    } while (token.type != EOS_T);

    lexer_free(l);

    return 0;
}