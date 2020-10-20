.data
string_space: .space 1024
is_palin_msg: .asciiz "This string is a palindrome."
not_palin_msg: .asciiz "This string is not a palindrome."

.text
main:
la $30 0
li $31 1024
li $29 8
syscall
li $1 0
li $2 0
length_loop:
lb $3 $2
beqz $3 2
addi $2 $2 1
b -4
end_length_loop:
subi $2 $2 1
test_loop:
bge $1 $2 6
lb $3 $1
lb $4 $2
bne $3 $4 7
addi $1 $1 1
subi $2 $2 1
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
li $29 10
syscall