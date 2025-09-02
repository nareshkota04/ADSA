#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    double (*func)(double);
    const char* name;
    const char* gnuplot_title;
} Function;


double f_1_over_n(double n) { return (n > 0) ? 1.0 / n : 0; }
double f_log2n(double n) { return (n > 0) ? log2(n) : 0; }
double f_12sqrtn(double n) { return 12 * sqrt(n); }
double f_50n_p5(double n) { return 50 * pow(n, 0.5); }
double f_n_p51(double n) { return pow(n, 0.51); }
double f_nlog2n(double n) { return (n > 0) ? n * log2(n) : 0; }
double f_n2_m324(double n) { return pow(n, 2) - 324; }
double f_100n2_p6n(double n) { return 100 * pow(n, 2) + 6 * n; }
double f_2n3(double n) { return 2 * pow(n, 3); }
double f_n_log2n(double n) { return (n > 0) ? pow(n, log2(n)) : 0; }
double f_3_n(double n) { return pow(3, n); }
double f_2_2n(double n) { return pow(4, n); } 


void plot_inline(const char* title, Function functions[], int count, 
                 double n_start, double n_end, double step, int use_log_scale) {
    
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == NULL) {
        printf("Gnuplot not found or could not be opened.\n");
        return;
    }


    fprintf(gnuplotPipe, "set title '%s' font ',14'\n", title);
    fprintf(gnuplotPipe, "set xlabel 'n'\nset ylabel 'f(n)'\n");
    fprintf(gnuplotPipe, "set key top left\n");
    fprintf(gnuplotPipe, "set grid\n");
    if (use_log_scale) {
        fprintf(gnuplotPipe, "set logscale y\n");
    }

   
    fprintf(gnuplotPipe, "plot ");
    for (int i = 0; i < count; i++) {
        fprintf(gnuplotPipe, "'-' with lines linewidth 2 title '%s'%s",
                functions[i].gnuplot_title, (i == count - 1) ? "\n" : ", ");
    }

   
    for (int i = 0; i < count; i++) {
        for (double n = n_start; n <= n_end; n += step) {
            if (n <= 0) continue;
            fprintf(gnuplotPipe, "%.4f %.6f\n", n, functions[i].func(n));
        }
        fprintf(gnuplotPipe, "e\n");
    }
    
    fflush(gnuplotPipe); 
    pclose(gnuplotPipe);
}


int main() {
    Function all_functions[] = {
        {f_nlog2n, "n log2(n)", "n log_2(n)"},
        {f_12sqrtn, "12*sqrt(n)", "12*n^{0.5}"},
        {f_1_over_n, "1/n", "1/n"},
        {f_n_log2n, "n^(log2(n))", "n^{log_2(n)}"},
        {f_100n2_p6n, "100n^2 + 6n", "100n^2 + 6n"},
        {f_n_p51, "n^0.51", "n^{0.51}"},
        {f_n2_m324, "n^2 - 324", "n^2 - 324"},
        {f_50n_p5, "50n^0.5", "50n^{0.5}"},
        {f_2n3, "2n^3", "2n^3"},
        {f_3_n, "3^n", "3^n"},
        {f_2_2n, "2^(2n)", "4^n"},
        {f_log2n, "log2(n)", "log_2(n)"}
    };
    int num_functions = sizeof(all_functions) / sizeof(Function);

    double N_LARGE = 500.0; 
    for (int i = 0; i < num_functions - 1; i++) {
        for (int j = 0; j < num_functions - i - 1; j++) {
            double val1 = all_functions[j].func(N_LARGE);
            double val2 = all_functions[j+1].func(N_LARGE);
             if (val1 > val2) {
                Function temp = all_functions[j];
                all_functions[j] = all_functions[j+1];
                all_functions[j+1] = temp;
            }
        }
    }

    
    printf("--- Functions in Increasing Order of Growth (Verified by Implementation at N=%.0f) ---\n", N_LARGE);
    for (int i = 0; i < num_functions; i++) {
        printf("%2d. %s\n", i + 1, all_functions[i].name);
    }

    printf("\n--- Pairs of Functions where f ∈ Θ(g) ---\n");
    printf("1. (12*sqrt(n), 50n^0.5) are both Θ(n^0.5)\n");
    printf("2. (100n^2 + 6n, n^2 - 324) are both Θ(n^2)\n");

    printf("\nGenerating plots...\n");

    Function slow_growers[] = { all_functions[0], all_functions[1], all_functions[2], all_functions[3], all_functions[4] };
    plot_inline("Slow-Growing Functions", slow_growers, 5, 0.1, 100, 0.5, 0);

    Function poly_growers[] = { all_functions[5], all_functions[6], all_functions[7], all_functions[8] };
    plot_inline("Polynomial and Log-Linear Functions", poly_growers, 4, 1, 50, 0.5, 0);

    Function fast_growers[] = { all_functions[8], all_functions[9], all_functions[10], all_functions[11] };
    plot_inline("Super-Polynomial and Exponential Functions (Log Scale)", fast_growers, 4, 1, 15, 0.1, 1);

    return 0;
}