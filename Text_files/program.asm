push ax
pop ax
push 4
pop [ax+5]
push 8
push [ax+5]
mul
out
hlt
