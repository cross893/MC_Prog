.code
;-------------------------------------------------------------------------------------------------------------
makeSum proc
	; extern "C" int makeSum(int firstValue, int secondValue);
	mov eax, ecx
	add eax, edx
	ret
makeSum endp
;-------------------------------------------------------------------------------------------------------------
drawStartSymbol proc
	mov eax, r8d
	mov rbx, r8
	shr rbx, 32
	mov ax, bx
	stosd
	ret
drawStartSymbol endp
;-------------------------------------------------------------------------------------------------------------
drawEndSymbol proc
	mov rbx, r8
	shr rbx, 48
	mov ax, bx
	stosd
	ret
drawEndSymbol endp
;-------------------------------------------------------------------------------------------------------------
getPosAddress proc

	; 1. ��������� ����� ������: addressOffset = (pos.yPos * pos.screenWidth + pos.xPos) * 4
	; 1.1. ��������� pos.yPos * pos.screenWidth
	mov rax, rdx
	shr rax, 16
	movzx rax, ax

	mov rbx, rdx
	shr rbx, 32
	movzx rbx, bx

	imul rax, rbx

	; 1.2. ������� pos.xPos � RAX
	movzx rbx, dx
	add rax, rbx

	; 1.3. ��������� ���������� �������� � �������� ��������
	shl rax, 2
	
	mov rdi, rcx
	add rdi, rax

	ret
getPosAddress endp
;-------------------------------------------------------------------------------------------------------------
getScreenWidthSize proc
	
	; ���������� �������� ������� ������

	mov r11, rdx
	shr r11, 32
	movzx r11, r11w
	shl r11, 2

	ret
getScreenWidthSize endp
;-------------------------------------------------------------------------------------------------------------
drawLineHorizontal proc
	
	; extern "C" void drawPairSymbol(CHAR_INFO* screenBuffer, sPosition pos, aSymbol symbol);
	
	push rax
	push rbx
	push rcx
	push rdi
	
	; 1. ��������� ����� ������
	call getPosAddress

	; 2. ������� ��������� ������
	call drawStartSymbol

	; 3. ������� �������
	mov eax, r8d
	mov rcx, rdx
	shr rcx, 48
	
	rep stosd

	; 4. ������� �������� ������
	call drawEndSymbol

	pop rdi
	pop rcx
	pop rbx
	pop rax

	ret
drawLineHorizontal endp
;-------------------------------------------------------------------------------------------------------------
drawLineVertical proc
	; extern "C" void drawLineVertical(CHAR_INFO* screenBuffer, sPosition pos, aSymbol symbol);

	push rax
	push rcx
	push rdi
	push r11

	; 1. ��������� ����� ������
	call getPosAddress
	
	; 2. ���������� �������� ������� ������
	call getScreenWidthSize
	sub r11, 4

	; 3. ������� ��������� ������
	call drawStartSymbol
	add rdi, r11

	; 4. ������� ������� �����
	mov rcx, rdx
	shr rcx, 48

	mov eax, r8d

_loopDrawLineVertical:
	stosd
	add rdi, r11
	loop _loopDrawLineVertical

	; 5. ������� �������� ������
	call drawEndSymbol

	pop r11
	pop rdi
	pop rcx
	pop rax

	ret
drawLineVertical endp
;-------------------------------------------------------------------------------------------------------------
showColors proc

	; extern "C" void showColors(CHAR_INFO* screenBuffer, sPosition pos, CHAR_INFO symbol);

	push rax
	push rbx
	push rcx
	push rdi
	push r10
	push r11

	; 1. ��������� ����� ������
	call getPosAddress

	mov r10, rdi

	; 2. ���������� �������� ������� ������
	call getScreenWidthSize

	; 3. ������� �����
	mov rax, r8
	and rax, 0ffffh
	mov rbx, 16
	
	xor rcx, rcx

	_loopShowColors0:
		mov cl, 16
		_loopShowColors1:
			stosd
			add rax, 010000h
		loop _loopShowColors1
		add r10, r11
		mov rdi, r10
		dec rbx
	jnz _loopShowColors0

	pop r11
	pop r10
	pop rdi
	pop rcx
	pop rbx
	pop rax

	ret
showColors endp
;-------------------------------------------------------------------------------------------------------------
clearArea proc
	
	; extern "C" void clearArea(CHAR_INFO* screenBuffer, sAreaPosition areaPos, aSymbol symbol);

	push rax
	push rbx
	push rcx
	push rdi
	push r10
	push r11

	; 1. ��������� ����� ������
	call getPosAddress

	mov r10, rdi

	; 2. ���������� �������� ������� ������
	call getScreenWidthSize

	; 3. ������� �����
	mov rax, r8
	mov rbx, rdx
	shr rbx, 48
	
	xor rcx, rcx
	
	_loopClearArea0:
		mov cl, bl
		rep stosd
		add r10, r11
		mov rdi, r10
		dec bh
	jnz _loopClearArea0

	pop r11
	pop r10
	pop rdi
	pop rcx
	pop rbx
	pop rax

	ret
clearArea endp
;-------------------------------------------------------------------------------------------------------------
drawBotText proc
	
	; extern "C" int drawBotText(CHAR_INFO* screenBuffer, sTextPosition pos, const wchar_t* str);

	push rbx
	push rdi
	push r8

	; 1. ��������� ����� ������
	call getPosAddress

	mov rax, rdx
	shr rax, 32

	xor rbx, rbx

_drawBotText1:
	mov ax, [ r8 ]
	
	cmp ax, 0
	je _exit

	add r8, 2

	stosd
	inc rbx
	jmp _drawBotText1

_exit:
	mov rax, rbx

	pop r8
	pop rdi
	pop rbx
	
	ret
drawBotText endp
;-------------------------------------------------------------------------------------------------------------
drawBotLimitedText proc

	; extern "C" void drawBotLimitedText(CHAR_INFO* screenBuffer, sTextPosition pos, const wchar_t* str, unsigned short limit);

	push rax
	push rcx
	push rdi
	push r8
	push r9

	; 1. ��������� ����� ������
	call getPosAddress

	mov rax, rdx
	shr rax, 32


_drawBotLimitedText1:
	mov ax, [ r8 ]
	
	cmp ax, 0
	je _fillSpaces

	add r8, 2

	stosd

	dec r9
	cmp r9, 0
	je _exit

	jmp _drawBotLimitedText1

_fillSpaces:
	mov ax, 020h
	mov rcx, r9

	rep stosd

_exit:

	pop r9
	pop r8
	pop rdi
	pop rcx
	pop rax
	
	ret
drawBotLimitedText endp
;-------------------------------------------------------------------------------------------------------------
end