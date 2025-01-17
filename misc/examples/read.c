#include <stc/cstr.h>
#define i_val_str
#include <stc/cvec.h>
#include <errno.h>

cvec_str read_file(const char* name)
{
    cvec_str vec = cvec_str_init();
    c_WITH (FILE* f = fopen(name, "r"), fclose(f))
        c_WITH (cstr line = cstr_NULL, cstr_drop(&line))
            while (cstr_getline(&line, f))
                cvec_str_push(&vec, cstr_clone(line));
    return vec;
}

int main()
{
    int n = 0;
    c_WITH (cvec_str vec = read_file(__FILE__), cvec_str_drop(&vec))
        c_FOREACH (i, cvec_str, vec)
            printf("%5d: %s\n", ++n, cstr_str(i.ref));

    if (errno)
        printf("error: read_file(" __FILE__ "). errno: %d\n", errno);
}
