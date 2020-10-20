.data
string_space: .space 1024
is_palin_msg: .asciiz "This string is a palindrome."
not_palin_msg: .asciiz "This string is not a palindrome."

.text
main:
la $30 0
li $31 1024
li $29 0
syscall
li $0 0
li $1 0
length_loop:
lb $2 $1
beqz $2 2
addi $1 $1 1
b -4
end_length_loop:
subi $1 $1 1
test_loop:
bge $0 $1 6
lb $2 $0
lb $3 $1
bne $2 $3 7
addi $0 $0 1
subi $1 $1 1
b -7
is_palin:
la $31 1
li $29 1
syscall
b 4
not_palin:
la $31 2
li $29 1
syscall
b 0
exit:
li $29 2
syscall