title tstmov

;информируем транслятор о том, что во время исполнения
;программы регистр cs будет содержать начальный адрес сегмента с,
;ds – сегмента d, ss – сегмента s
assume cs:c,ds:d,ss:s

s segment stack 'STACK'
	dw 128 dup('ss')
s ends

d segment

	newLine db 10,13,'$'
	error1 db 10,13,"4ah error$"
	error2 db 10,13,"There's no available memory$"
	count dw 0
	two dw 4
	blockSize dw ?
	newAddress dw ?
	message1 db 10,13,"Run number:$"
	message2 db 10,13,"Current address:$"
	message3 db 10,13,"Block size:$"
	message4 db 10,13,"New address:$"
	
d ends

c segment

UnsignedOut proc
	
	xor cx,cx ; счетчик цифр
	mov bx,10

m: 
	
	inc cx 
	xor dx,dx 
	div bx 
	push dx 
	or ax,ax ;проверяем, есть ли еще цифры
	jnz m

	;при выходе из цикла в стеке лежат цифры, в cx – их количество
	
m1: 
	pop dx 
	add dx,'0' 
	mov ah,2 
	int 21h
	loop m1

	ret 
	
UnsignedOut endp

start: 
	
	mov bx,z
	mov ax,es	; es - сегмент распределенного блока памяти (PSP - program segment prefix)
	sub bx,ax	; желаемый размер блока в параграфах
	mov ah,4ah
	int 21h
	
	jnc label1	; CF = 0

	mov cx,d
	mov ds,cx

	mov ah,9
	lea dx,error1
	int 21h
	mov ah,4ch
	int 21h
	
label1:
	
	mov ax,cs		; cs адресует программный код
	sub ax,c
	add ax,d
	mov ds,ax
	inc count

	mov ah,9
	lea dx,message1		; номер запуска
	int 21h
	mov ax,count
	call UnsignedOut

	mov ah,9
	lea dx,message2		; адрес
	int 21h
	mov ax,cs
	call UnsignedOut

	mov ah,9
	lea dx,message3		; размер блока
	int 21h
	mov ax,es          	; Сегмент PSP
	dec ax             	; Сегмент Memory Control Block
	mov es,ax
	mov ax,es:[3]
	mov blockSize, ax
	call UnsignedOut
	
	mov ah,9
	lea dx,newLine
	int 21h

	mov ax,es:[3]
	mul two
	mov bx, ax
	mov ax, es
	inc ax
	mov es, ax
	mov ah,4ah
	int 21h

	jnc label1		; CF = 0
	
	mov bx, blockSize
	mov ah,4ah
	int 21h

	mov bx, blockSize
	mov ah,48h
	int 21h
	jnc label2

	mov ah,9
	lea dx,error2
	int 21h
	mov ah,4ch
	int 21h
	
label2:
	
	mov es,ax
	mov cl,3
	shl bx,cl
	mov cx,bx
	xor si,si
	
label3: 
	
	mov ax,[si]
	mov es:[si],ax
	inc si
	inc si
	loop label3

	mov ax,es
	add ax,c
	sub ax,d
	
	mov newAddress, ax
	mov ah,9
	lea dx,message4		; адрес
	int 21h
	mov ax, newAddress
	call UnsignedOut

	mov ah,4ch
	int 21h
	
c ends

z segment
z ends

end start