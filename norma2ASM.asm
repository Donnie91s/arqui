section .data
    const_uno    dq 1.0

section .text
    global calculateN2_asm

calculateN2_asm:
    ; 1. CALCULAR EL DENOMINADOR (a^b)
    ; Usaremos xmm2 para guardar el denominador
    movsd xmm2, [rel const_uno] ; xmm2 = 1.0
    
    ; Convertimos 'a' a double para multiplicar
    cvtsi2sd xmm3, edi          ; xmm3 = (double)a
    
    ; Bucle para elevar a^b
    mov ecx, esi                ; Contador del bucle = b
    cmp ecx, 0
    je  get_n                   ; Si b es 0, a^b es 1.0 (ya está en xmm2)

calc_pow:
    mulsd xmm2, xmm3            ; xmm2 *= a
    loop  calc_pow

get_n:
    ; 2. OBTENER N Y PREPARAR BUCLE PRINCIPAL
    mov r8, [rdx]               ; r8 = valor de N (desde el puntero)
    xorpd xmm0, xmm0            ; xmm0 = 0.0 (Acumulador de la suma total)
    mov rax, 1                  ; rax = n (iterador empezando en 1)
    
    movsd xmm4, [rel const_uno] ; xmm4 = 1.0 para la suma (n^2 + 1)

    ; 3. BUCLE DE LA SUMATORIA
next_sum:
    ; Convertir iterador 'n' a double
    cvtsi2sd xmm1, rax          ; xmm1 = (double)n
    
    ; n^2 + 1
    mulsd xmm1, xmm1            ; xmm1 = n * n
    addsd xmm1, xmm4            ; xmm1 = n^2 + 1
    
    ; Dividir entre denominador
    divsd xmm1, xmm2            ; xmm1 = (n^2 + 1) / a^b
    
    ; Acumular en la suma total
    addsd xmm0, xmm1            ; total_sum += xmm1
    
    ; Control del bucle
    inc rax                     ; n++
    cmp rax, r8                 ; ¿n <= N?
    jbe next_sum                ; Si es menor o igual, repetir

done:
    ; El resultado final ya está en xmm0
    ret