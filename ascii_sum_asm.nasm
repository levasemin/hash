global hash_ascii_sum_asm

hash_ascii_sum_asm:

push rbx 
push rdi

xor eax, eax
xor ebx, ebx

next_symbol:

mov bl, [rdi]

cmp bl, 0
je out

add eax, ebx

inc rdi

jmp next_symbol
out:

pop rdi
pop rbx

ret