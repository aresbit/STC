#include <stdio.h>
#define i_extern // cstr + utf8 functions
#include <stc/cregex.h>
#include <stc/csview.h>

int main()
{
    puts("Split with c_FORTOKEN (csview):");

    c_FORTOKEN (i, "Hello World C99!", " ")
        printf("'%.*s'\n", c_SVARG(i.token));


    puts("\nSplit with c_FORMATCH (regex):");

    c_WITH (cregex re = cregex_from("[^ ]+"), cregex_drop(&re))
        c_FORMATCH (i, &re, "  Hello   World      C99! ")
            printf("'%.*s'\n", c_SVARG(i.match[0]));
}
