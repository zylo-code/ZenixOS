[org 0x7C00]

start:
	mov ax, 0x0003
	int 0x10

	mov si, msg
	call print
	jmp $

print:
	lodsb
	or al, al
	jz done
	mov ah, 0x0e
	int 0x10
	jmp print
done: ret

msg db "Hello, World!", 0x0A, 0x0D, 0
times 510 - ($ - $$) db 0
dw 0xAA55
