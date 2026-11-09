/* Defined run-time failure rather than undefined behaviour. */
#include <stdio.h>
int main(void) {
    FILE *f=fopen("__nonexistent_assignment_input__.txt","r");
    if (!f) { perror("Cannot open input");return 2; }
    fclose(f);return 0;
}
