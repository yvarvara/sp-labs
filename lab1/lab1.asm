.model small, c

.stack

.data

    fileSpec db 'labs\files\*.???', 0
    fileSpecLen dd 15
	
    filename db 9, ?, 9 dup (?)
    handler dw ?
    ErrorMessage db 'File open error', '$'

    message1 db "Enter target files' extension:", 0dh, 0ah, '$'
    message2 db 'Enter result file name:', 0dh, 0ah, '$'
    newLine db 0dh, 0ah, '$'

    newLineLen dw 2

    DTA db 128h dup(0)

.code

start:

    mov ax, @Data
    mov ds, ax

    call setExtensionProc	; enter target files' extension
    call setFileNameProc	; enter result file name

    mov ah, 3dh			; open file
    mov al, 1
    mov dx, offset filename + 2
    int 21h
	
    jnc ifFileExists

    mov ah, 3ch			; create file
    mov cx, 0
    int 21h

    jc errMessage

ifFileExists:

    mov handler, ax

    mov ah, 42h			; move to the end of file
    mov bx, handler
    mov cx, 0
    mov dx, 0
    mov al, 2
    int 21h

    mov dx, offset DTA
    mov ah, 1Ah
    int 21h

    mov dx, offset fileSpec
    xor cx, cx
    mov ah, 4Eh			; find first file
    int 21h 
    jc exit

print_name:

    mov cx,0FFFFh 
    mov si, offset DTA + 1eh

l1:

    inc cx
    inc si
    cmp byte ptr [si], 0
    jnz l1

    inc cx

    lea dx, DTA + 1eh 
    mov ah, 40h
    mov bx, handler
    int 21h

    mov ah, 40h
    mov bx, handler
    mov cx, newLineLen             
    lea dx, newLine
    int 21h

    mov dx, offset DTA
    xor cx, cx
    mov ah, 4fh			; find next file
    int 21h
    jnc print_name

    mov ah, 3eh			; close file
    mov bx, handler
    int 21h      
    jmp exit

errMessage:

    mov dx, offset ErrorMessage
    mov ah, 09h
    int 21h

exit:   

    mov ax, 4c00h
    int 21h

setExtensionProc proc

    mov ah, 09h
    mov dx, offset message1
    int 21h

    mov ah, 01h
    int 21h
    mov [fileSpec + 13], al

    mov ah, 01h
    int 21h
    mov [fileSpec + 14], al

    mov ah, 01h
    int 21h
    mov [fileSpec + 15], al

    mov al, 13			; new line
    int 29h
    mov al, 10			; carriage return
    int 29h

    ret

setExtensionProc endp

setFileNameProc proc

    mov ah, 09h
    mov dx, offset message2
    int 21h

    mov ah, 0ah
    mov dx, offset filename
    int 21h

    xor bx, bx
    mov bl, filename + 1
    mov byte ptr filename[bx + 2], 0

    ret

setFileNameProc endp

end start