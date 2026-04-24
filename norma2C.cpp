#include <iostream>
#include <iomanip>
#include <cmath> 

double calculateN2_nopow (int a, int b, long *n){

    //-----------------se calcula a^b------------------

    double denominator = 1.0;
    for (int i = 0; i < b; ++i) {
        denominator *= a;
    }

    //-----------------se calcula lo que está dentro de la sumatoria------------------

    double total_sum = 0.0;
    // Obtenemos el valor del límite desde el puntero
    long N_limit = *n;

    // 2. Bucle de la sumatoria
    for (long i = 1; i <= N_limit; ++i) {
        // CRÍTICO: Convertimos i a double ANTES de elevar al cuadrado.
        // Si haces (i * i) como long, el resultado superará los 64 bits 
        // cuando i > 3,037,000,500 aprox, causando un overflow.
        double n_val = (double)i;
        double term = (n_val * n_val + 1.0) / denominator;
        
        total_sum += term;
    }

    return total_sum;
}

double calculateN2_pow(int a, int b, long *n) {
    // 1. Usamos la función de la librería estándar para el denominador
    // std::pow(base, exponente)
    double denominator = std::pow((double)a, (double)b);

    double total_sum = 0.0;
    long N_limit = *n;

    // 2. Bucle de la sumatoria
    for (long i = 1; i <= N_limit; ++i) {
        double n_val = (double)i;
        // Mantenemos la lógica de la sumatoria idéntica para una comparación justa
        double term = (n_val * n_val + 1.0) / denominator;
        total_sum += term;
    }

    return total_sum;
}

extern "C" double calculateN2_asm(int a, int b, long *n);

int main() {
    // Definimos valores de prueba (Inciso d: valores máximos)
    int a = 7; 
    int b = 7;
    long N = 4294967295L; // El máximo permitido

    struct timespec ti, tf;
    double elapsed;
    double result;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Evaluando para N = " << N << ", a = " << a << ", b = " << b << "\n";
    std::cout << "---------------------------------------------------------" << std::endl;

    // 1. Prueba: C++ Sin Pow
    clock_gettime(CLOCK_REALTIME, &ti);
    result = calculateN2_nopow(a, b, &N);
    clock_gettime(CLOCK_REALTIME, &tf);
    elapsed = (tf.tv_sec - ti.tv_sec) * 1e9 + (tf.tv_nsec - ti.tv_nsec);
    std::cout << "C++ (Sin Pow) | Resultado: " << result << " | Tiempo: " << elapsed << " ns\n";

    // 2. Prueba: C++ Con Pow
    clock_gettime(CLOCK_REALTIME, &ti);
    result = calculateN2_pow(a, b, &N);
    clock_gettime(CLOCK_REALTIME, &tf);
    elapsed = (tf.tv_sec - ti.tv_sec) * 1e9 + (tf.tv_nsec - ti.tv_nsec);
    std::cout << "C++ (Con Pow) | Resultado: " << result << " | Tiempo: " << elapsed << " ns\n";

    // 3. Prueba: ASM (Comenta estas líneas hasta que tengas el archivo .asm compilado)
    
    clock_gettime(CLOCK_REALTIME, &ti);
    result = calculateN2_asm(a, b, &N);
    clock_gettime(CLOCK_REALTIME, &tf);
    elapsed = (tf.tv_sec - ti.tv_sec) * 1e9 + (tf.tv_nsec - ti.tv_nsec);
    std::cout << "ASM           | Resultado: " << result << " | Tiempo: " << elapsed << " ns\n";
    

    return 0;
}