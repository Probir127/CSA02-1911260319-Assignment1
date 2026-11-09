#include <stdio.h>
#include <limits.h>
#include <time.h>
#include "../common/billing.h"
int main(void) {
    const unsigned sizes[]={4000,60000,800000};
    const money_t one=bill_for_units(65535).total_paise;
    puts("n,integer_paise,float_rupees,float_error_paise,double_rupees,double_error_paise,int_safe,long_safe,ll_safe,float_ms,integer_ms,double_ms");
    for (unsigned k=0;k<3;k++) {
        unsigned n=sizes[k];volatile float ft=0;volatile double dt=0;volatile money_t it=0;
        clock_t start=clock();
        for (unsigned i=0;i<n;i++) ft+=(float)one/100.0f;
        double ft_ms=1000.0*(double)(clock()-start)/CLOCKS_PER_SEC;
        start=clock();for (unsigned i=0;i<n;i++) it+=one;
        double it_ms=1000.0*(double)(clock()-start)/CLOCKS_PER_SEC;
        start=clock();for (unsigned i=0;i<n;i++) dt+=(double)one/100.0;
        double dt_ms=1000.0*(double)(clock()-start)/CLOCKS_PER_SEC;
        printf("%u,%lld,%.2f,%.0f,%.2f,%.0f,%d,%d,%d,%.6f,%.6f,%.6f\n",n,(money_t)it,(double)ft,(double)ft*100.0-(double)it,(double)dt,(double)dt*100.0-(double)it,
               one<=INT_MAX/n,one<=LONG_MAX/n,one<=LLONG_MAX/n,ft_ms,it_ms,dt_ms);
    }
    printf("First overflowing int addition at maximum bill: %lld\n",(money_t)INT_MAX/one+1);
    return 0;
}
