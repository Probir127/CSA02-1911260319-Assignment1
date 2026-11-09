#include <stdio.h>
/* Teaching example: numeric token must be within int's range.
   Task09 uses bounded text plus checked conversion for arbitrary input. */
int main(void) {
    int previous=0,current=0;
    printf("Enter previous and current readings: ");
    int matched=scanf("%d %d",&previous,&current);
    printf("scanf assignments=%d\n",matched);
    if (matched!=2) { puts("Invalid numeric input");return 2; }
    if (previous<0 || current<0 || previous>65535 || current>65535) {
        puts("Reading outside 0..65535");return 2;
    }
    printf("Accepted previous=%d current=%d\n",previous,current);
    long p=16250L;
    printf("Example formatted bill: Rs %ld.%02ld\n",p/100,p%100);
    return 0;
}
