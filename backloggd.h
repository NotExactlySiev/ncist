#include <stddef.h>

#define BL_URL  "https://backloggd.com"

typedef struct {
    char buf[0x10000];
    size_t size;
} resp_t;

int bl_journal();
int bl_find(char*);