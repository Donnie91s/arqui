section .data

    initial_guess   dq 1.0
    const_medio     dq 0.5
    abs_mask        dq 0x7FFFFFFFFFFFFFFF

section .text

    global calculateSQRT
    ;global _start

;_start:
calculateSQRT:

	; xmm0 para el núermo
    ; xmm1 para la precisión
    movsd xmm2, [rel initial_guess] ; almacenando para trabajar con registros
    movsd xmm3, [rel abs_mask] ; almacenando para trabajar con registros
    movsd xmm4, [rel const_medio] ; almacenando para trabajar con registros
    movsd xmm5, xmm0 ; xmm0 se utilizará para los cálulos

next:
    ; --------------- Hacer la operación ------------------
    movsd xmm0, xmm5 ; se almacena el valor de xmm5 en xmm0 considerando la naturaleza iterativa del problema
    divsd xmm0, xmm2 ; se divide x/guess y se alamacena el valor en el registro de X
    addsd xmm0, xmm2 ; se suma el resultado al current guess
    mulsd xmm0, xmm4 ; se multiplica por 1/2

    ; --------------- cálculo del error ------------------
    movsd xmm6, xmm0 ; se alamacena el valor de next_guess en xmm6 para no alterarlo luego
    subsd xmm6, xmm2 ; se resta current guess con el valor de next guess
    andps xmm6, xmm3 ; xmm6 ahora contiene |xmm6| 

    ; --------------- condición de salida ------------------
    ucomisd xmm6, xmm1
    jbe done;

    ; --------------- actualizar para la siguiente iteración ------------------
    movsd xmm2, xmm0; current_guess = next_guess
    jmp next

done:

    ret



