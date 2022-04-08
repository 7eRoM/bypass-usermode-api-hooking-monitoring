IFDEF RAX

;;;;;;;;;;;;;;;;;;;; X64
.code

MyNtCreateFile proc
	mov  r10, rcx
	mov rax, 55h
	syscall	
	ret
MyNtCreateFile endp


ELSE
;;;;;;;;;;;;;;;;;;;; X86
.MODEL flat, C
.stack 4096
.data
.code

MyNtCreateFile proc
		mov     eax, 178h
        call    offset_d
        ret     2Ch
offset_d:    mov     edx, esp
        sysenter
        ret
MyNtCreateFile endp

ENDIF

end
