#include "span.h"
#include "token.h"
#include "lexer.h"

int main() {
    Lexer* l = lexer_init("{{}}");

    JToken token = lexer_getNext(l);

    lexer_free(l);
    return 0;
}