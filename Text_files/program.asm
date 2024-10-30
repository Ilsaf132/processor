in
pop ax
push ax
push 0.00
je ZERO:
push ax
push 0.00
jb BZRO:
push ax
push ax
push 100.00
div
push 100.00
mul
sub
push 0.00
jne BZRO:
push ax
pop bx
HOME:
push ax
push 1.00
sub
push 1.00
ja NEXT:
push bx
out
hlt
NEXT:
push ax
push 1.00
sub 
push bx
mul
pop bx
push ax
push 1.00
sub 
pop ax
jmp HOME:
ZERO:
push 1.00
out
hlt
BZRO:
push 0.00
out
hlt
