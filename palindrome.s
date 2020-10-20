.data
0x00000000 0
0x00000001 1
0x00000002 2

.text
la $30 0
li $29 0
syscall
li $0 0
li $1 0
lb $2 $1
beqz $2 2
addi $1 $1 1
b -4
subi $1 $1 1
bge $0 $1 6
lb $2 $0
lb $3 $1
bne $2 $3 7
addi $0 $0 1
subi $1 $1 1
b -7
la $31 1
li $29 1
syscall
b 4
la $31 2
li $29 1
syscall
b 0
li $29 2
syscall