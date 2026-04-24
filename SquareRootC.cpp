#include <iostream>
#include <iomanip> // Para mejorar la salida de decimales
#include <cmath>
#include <ctime>

// Prototipo de tu función en ASM
extern "C" double calculateSQRT(double x, double precision);

// Versión en C++ del Método de Newton
double calculateSQRTCpp(double x, double precision) {
    double guess = 1.0;
    double next;
    while (true) {
        next = 0.5 * (guess + (x / guess));
        if (std::abs(next - guess) <= precision) {
            break;
        }
        guess = next;
    }
    return next;
}

int main() {
    // Parámetros del inciso c)
    double valores[] = {2000.0, 15000.0};
    double precision = 0.001;

    struct timespec ti, tf;
    double elapsed;

    for (double x : valores) {
        std::cout << "--- Evaluando x = " << x << " ---" << std::endl;

        // 1. Medir C++
        clock_gettime(CLOCK_REALTIME, &ti);
        double resC = calculateSQRTCpp(x, precision);
        clock_gettime(CLOCK_REALTIME, &tf);
        elapsed = (tf.tv_sec - ti.tv_sec) * 1e9 + (tf.tv_nsec - ti.tv_nsec);
        std::cout << "C++:  " << std::fixed << std::setprecision(6) << resC 
                  << " | Tiempo: " << elapsed << " ns" << std::endl;

        // 2. Medir ASM
        clock_gettime(CLOCK_REALTIME, &ti);
        double resAsm = calculateSQRT(x, precision);
        clock_gettime(CLOCK_REALTIME, &tf);
        elapsed = (tf.tv_sec - ti.tv_sec) * 1e9 + (tf.tv_nsec - ti.tv_nsec);
        std::cout << "ASM:  " << resAsm 
                  << " | Tiempo: " << elapsed << " ns" << std::endl;

        // 3. Verificación con la librería estándar (math.h)
        std::cout << "sqrt: " << std::sqrt(x) << " (Referencia)" << std::endl;
        
        // Error relativo entre ASM y la referencia real
        double relerr = std::abs(std::sqrt(x) - resAsm) / std::sqrt(x);
        std::cout << "Error relativo (ASM vs Referencia): " << relerr << "\n" << std::endl;
    }

    return 0;
}