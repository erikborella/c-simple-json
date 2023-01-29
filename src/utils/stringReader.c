#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "utils/stringReader.h"

struct stringReader {
    const char* stringRef;
    bool isNewLine;
    size_t startPtr;
    size_t endPtr;
    TokenPosition startPosition;
    TokenPosition endPosition;
};

static void updateEndPosition(StringReader* sr, char ch) {
    if (sr->isNewLine) {
        sr->endPosition.line++;
        sr->endPosition.column = 1;

        if (ch != '\n')
            sr->isNewLine = false;

        return;
    }

    if (ch == '\n')
        sr->isNewLine = true;

    sr->endPosition.column++;
}

static void finishSelection(StringReader* sr) {
    sr->startPtr = sr->endPtr;
    sr->startPosition = sr->endPosition;
}

StringReader* stringReader_init(const char* stringRef) {
    StringReader* sr = (StringReader*) malloc(sizeof(StringReader));

    if (sr != NULL) {
        sr->stringRef = stringRef;

        sr->isNewLine = true;

        sr->startPtr = 0;
        sr->endPtr = 0;

        TokenPosition startPosition = {.line = 1, .column = 1};
        TokenPosition endPosition = {.line = 0, .column = 0};
        sr->startPosition = startPosition;
        sr->endPosition = endPosition;

        updateEndPosition(sr, sr->stringRef[0]);
    }

    return sr;
}

void stringReader_free(StringReader* sr) {
    free(sr);
}

bool stringReader_isEOS(StringReader *sr) {
    return sr->stringRef[sr->endPtr] == EOS;
}

void stringReader_moveNext(StringReader* sr) {
    sr->endPtr++;

    char current = stringReader_getCurrent(sr);
    updateEndPosition(sr, current);
}

char stringReader_getCurrent(StringReader* sr) {
    return sr->stringRef[sr->endPtr];
}

char* stringReader_getSelected(StringReader* sr) {
    size_t selectedLen = sr->endPtr - sr->startPtr;

    const size_t selectedAllocSize = sizeof(char) * selectedLen;
    // selectedAllocSize + 1 char (EOS)
    char* selected = malloc(selectedAllocSize + (sizeof(char) * 1));

    memcpy(selected, sr->stringRef + sr->startPtr, selectedAllocSize);
    selected[selectedAllocSize] = EOS;

    finishSelection(sr);

    return selected;
}

void stringReader_ignoreSelected(StringReader* sr) {
    finishSelection(sr);
}

TokenLocation stringReader_getLocation(StringReader* sr) {
    TokenLocation location = {
        .start = sr->startPosition,
        .end = sr->endPosition,
    };

    return location;
}