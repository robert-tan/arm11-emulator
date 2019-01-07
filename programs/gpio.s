mov r2, #1
lsl r2, #18
ldr r0, =0x20200000
str r2, [r0, #4]
mov r1, #1
lsl r1, #16
loop1:
str r1, [r0, #40]
mov r2, #16777216
loop2:
sub r2, r2, #1
cmp r2, #0
bne loop2
str r1, [r0, #28]
mov r2, #16777216
loop3:
sub r2, r2, #1
cmp r2, #0
bne loop3
b loop1
andeq r0, r0, r0