;Sum two numbers
;b a — a + b
native '+', plus
    pop rax
    add [rsp], rax
    jmp next

;Deduct one number from another
;b a — a - b
native '-', minus
    pop rax
    pop rcx
    sub rcx, rax
    push rcx
    jmp next

;Multiple two numbers
;b a — a * b
native '*', mul
  pop rax
  pop rdx
  imul rdx
  push rax
  jmp next

;Divide one number by another
;b a — a / b
native '/', div
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rax
    jmp next

;Read a word from stdin and save it, starting from an address
;Length of the word goes to stack
native 'word', word
    pop rdi
    call read_word
    push rdx
    jmp next

;Print a null-terminated string
native 'prints', prints
  pop rdi
  call print_string
  jmp next

;Print definite symbols from string
native 'printc', printc
  pop rdx
  pop rsi
  mov rdi, 1
  mov rax, 1
  syscall
  jmp next

;Get an element from the stack and send it to stdout
native '.', dot
   cmp     rsp, [stackHead]
    je      .error
    pop     rdi
    call    print_int
    call    print_newline
    jmp     .f
.error:
    mov     rdi, underflow
    call    print_string
    call    print_newline
.f:
    jmp     next

;Drop top of stack
native 'drop', drop
    pop     rax
    jmp     next

;Repeat top of stack
native 'dup', dup
    pop     rax
    push    rax
    push    rax
    jmp     next

;Swap top of stack and element follows after if
native 'swap', swap
    pop     rax
    pop     rcx
    push    rax
    push    rcx
    jmp     next

;Check if less
;y x — x < y
native '<', less
  	pop rax
	pop rdx
	cmp rdx, rax
	jge .false 
	.true:
	push 1
	jmp .final
	.false:
	push 0
	.final:
	jmp next

;Print stack without changing it
native '.S', show_stack
	mov rax, rsp
	.loop:
	cmp rax, [stack_start]
	jge next
	mov rdi, [rax]
	push rax
	call print_int
	call print_newline
	pop rax
	add rax, 8
	jmp .loop

;Check if equals
; a b — c (if a == b => c = 1; else c = 0)
native '=', equals
  	pop rax
	pop rdx
	cmp rax, rdx
	jne .false
	.true:
	push 1
	jmp .final
	.false:
	push 0
	.final:
	jmp next

;b a — a mod b
native "%", mod
	xor edx, edx
	pop rcx
	pop rax
	idiv rcx
	push rdx
	jmp next

;Bitwise AND
;a b — a & b
native 'and', and
    pop rax
    and [rsp], rax
    jmp next

;Bitwise OR
;a b — a | b
native 'or', or
    pop rax
    or [rsp], rax
    jmp next

;Transforn 1 to 0 and 0 to 1
native 'not', not
  pop rax
  test rax, rax
  jz .true
.false:
  push 0
  jmp next
.true:
  push 1
  jmp next

;Circular shift on stack
;a b c — b c a
native 'rot', rot
    pop     rax
    pop     rcx
    pop     rdx
    push    rcx
    push    rax
    push    rdx
    jmp     next 

;Read symbol from stdin and write if to the top of stack
native 'key', key
  	call read_char
	push rax
	jmp next

;Output a single char to stdout
native 'emit', emit
  	mov rdi, [rsp]
	call print_char
	jmp next

;Send to stack beginning of the user's memory
native 'mem', mem
	push qword[ustackHead]
	jmp next

;Save value by address
native '!', write
  	pop rax
  pop rdx
  mov [rax], rdx
  jmp next

;Fetch value from memory
native '@', fetch
  pop rax
  push qword[rax]
  jmp next

;Save value by address
native 'c!', write_char
  pop rax
  pop rdx
  mov [rax], dl
  jmp next

;Read one byte starting in addr
native 'c@', fetch_char
  pop rax
  movzx rax, byte [rax]
  push rax
  jmp next

;Pop from data stack to return stack
native '>r', to_r
  pop rax
  rpush rax
  jmp next

;Push from return stack to data stack
native 'r>', from_r
  rpop rax
  push rax
  jmp next

;Copy from the top of return stack to data stack
native "r@", r_fetch
  push qword [rstack]
  jmp next

;Logical OR
native 'lor', lor
    dq      xt_not
    dq      xt_swap
    dq      xt_not
    dq      xt_and
    dq      xt_not
    dq      xt_exit

;Logical AND
native "land", land
    pop     rax
    pop     rcx
    and     rcx, rax
    cmp     rcx, 0
    je      .null
    mov     rcx, 1
.null:
    push    rcx
    jmp     next

;Length of null-terminated string
native 'count', count
	pop rdi
	call string_length
    	push rax
	jmp next
	
;Parse int from string
native 'number', number
  	call read_word
	mov rax, rdi
	call parse_int
	push rax
	jmp next

;Find string, return its address 
native 'find', find
  pop rdi               
  call find_word
  push rax
  jmp next

;Converts word header start address to the execution token
native 'cfa', cfa
    pop rsi
    add rsi, 9
    .loop:
    mov al, [rsi]
    test al, al
    jz .end
    inc rsi
    jmp .loop
    .end:
    add rsi, 2
    push rsi
    jmp next

;Exit
native 'bye', bye
  mov rax, 60
  xor rdi, rdi
  syscall

;Print error message
native 'error', error
  mov  rdi, unknown_word
  call print_string
  call print_newline
  jmp next

;Send string address to rdi
native 'parse_int', parse_int
  pop rdi
  call parse_int         
  push rax              
  push rdx
  jmp next

;Push value to stack
native 'pushmode', pushmode
  xor eax, eax
  mov rax, [mode]
  push rax
  jmp next

;Push address to stack
native 'pushmode_address', pushmode_address
  xor eax, eax
  mov rax, mode
  push rax
  jmp next

;Add x to the word
native ',', comma
  mov rax, [here]
  pop qword [rax]
  add qword [here], 8
  jmp next

;Pushes a value immediately following this XT
native 'lit', lit
  push qword [pc]
  add pc, 8
  jmp next

;Initialization
native 'init', init
  mov rstack, rstack_start
  mov stack, stack_start
  xor eax, eax
  mov rax, [in_fd]
  rpush rax
  xor eax, eax
  pop rax
  mov [in_fd], rax
  mov qword[mode], 0
  mov pc, xt_run
  cmp qword [stack_start], 0
  je  .first
  mov rsp, [stack_start]
  jmp next
  .first:
  mov [stack_start], rsp
  jmp next

;Save address of buffer
native 'buffer', buffer
  push qword input_buf
  jmp next

;Used to construct words. Implements colon-words
native 'docol', docol
  	sub rstack, 8
	mov [rstack], pc
	add w, 8
	mov pc, w
	jmp next

;Exit
native 'exit', exit
  mov pc, [rstack]
  add rstack, 8
  jmp next

;Jump to an absolute address
native 'branch', branch
  mov pc, [pc]
  jmp next

;Jump to a location if top of stack = 0
native 'branch0', branch0
    pop rax
    cmp rax, 0
    jne .f
    mov pc, [pc]
    jmp next
    .f
    add pc, 8
    jmp next

;Create an entry in dictionary
native 'create', create
  mov rcx, [last_word]
  mov rsi, [here]
  mov [rsi], rcx
  mov [last_word], rsi
  add rsi, 8
  pop rdi
  push rsi
  call string_copy
  pop rsi
  push rsi
  mov rdi, rsi
  call string_length
  pop rsi
  add rsi, rax
  inc rsi              
  pop rax
  mov [rsi], al
  inc rsi
  mov [here], rsi
  jmp next
	
const dp, dp
const here, [here]
const in_fd, in_fd
const state, state

;Execute syscalls
native "syscall", syscall
  pop r9
  pop r8
  pop r10
  pop rdx
  pop rsi
  pop rdi
  pop rax
  syscall
  push rax
  push rdx
  jmp next
    
;Execute word with the token in top of stack
native 'execute', execute
	pop rax
	mov w, rax
	jmp [rax]

;Check if a greater then b
;If a less than b then top of stack is 0, otherwise top of stack is 1 
colon '>', greater
  dq xt_swap
  dq xt_less
  dq xt_exit

;Read word from stdin and start defining it
colon ':', colon
  .restart:
    dq xt_buffer
    dq xt_word
    branchif0 .restart
    dq xt_lit, 0         
    dq xt_buffer
    dq xt_create
    dq xt_lit, 1, xt_pushmode_address
    dq xt_write
    dq xt_lit, docol_impl
    dq xt_comma
    dq xt_exit

;End the word definition
colon ';', semicolon, 1
  dq xt_lit, 0, xt_pushmode_address
  dq xt_write
  dq xt_lit, xt_exit
  dq xt_comma
  dq xt_exit

;Read word and place its address to stack
colon "'", tick, 1
  dq xt_buffer
  dq xt_word
  dq xt_drop
  dq xt_buffer
  dq xt_find, xt_dup
  branchif0 .no_such_word
  dq xt_cfa
  dq xt_pushmode
  branchif0 .interpret
  dq xt_lit, xt_lit
  dq xt_comma
  dq xt_comma
  .interpret:
    dq xt_exit
    .no_such_word:
      dq xt_error
      dq xt_exit
