.MODEL Small
.286
.STACK 256
 
INTNO = 21h             		; номер прерывания

.CODE

Handler:                                ; новый обработчик прерывания
	cmp     ah, 09h
	jne     OldHandler

	push    cs
	lea     dx, Message
	pushf
	call    dword ptr [OldAddr]	; старый обработчик прерывания	
	pop     ds
	iret                    	; выход из прерывания

Message         DB      4 dup('$')

OldHandler: DB 0EAh            		
OldAddr DW ?,?
string db 30,30 dup ('$')  
TSREnd = $
ORG $-4
 
;----------------------------------------------------------------------------;
Start:
	mov     ah,3fh
        lea     dx,string
        mov     cx,30
        mov     bx,0    
        int  	21h
        sub     ax,2 			; считаем длину нашей строку без символа возврата каретки и перевода строки
        mov     cx,ax
        xor     si,si
        xor     ax,ax
 
        mov     al,' '   
        lea     di,string 
	cld
 
label1:    
	repne   scasb 			; повторяем для каждого символа
	jne     label2
        inc     si      
        jmp     label1     

label2:
        inc 	si			; количество слов
	push    ds              	; PSP

	;mov     ah,49h
	;mov     es,ds:[2Ch]
	;int     21h           
	
	mov     ax,3500h + INTNO
	int     21h             	; получаем вектор прерывания
	mov     OldAddr[0], bx
	mov     OldAddr[2], es

	mov     ah, 25h
	lea     dx, Handler
	push    cs
	pop     ds              	; DS = CS
	int     21h            	 	; устанавливаем новый обработчик

 	mov 	ax, si
	xor 	cx, cx      		; количество цифр в числе
        mov     bx, 10      		; система счисления
 
        @@DivLoop:
                xor dx, dx
                div     bx
                push    dx
                inc     cx
                or      ax,     ax
        jnz     @@DivLoop
 
        mov     dx,     4
        sub     dx,     cx
        lea     si,     Message

        @@SaveLoop:
                pop     ax
                add     al,     '0'
                mov     [si],   al
                inc     si
        loop    @@SaveLoop

	mov ah, 09h
	int 21h
 
	pop     ax         
	push    cs
	pop     dx
	sub     dx,ax
	add     dx, (TSREnd-Handler+15)/16
	mov     ax, 3100h
	int     21h			; остаёмся резидентно
 
END             Start