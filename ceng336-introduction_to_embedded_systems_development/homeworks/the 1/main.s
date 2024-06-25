PROCESSOR    18F4620

#include <xc.inc>

; CONFIGURATION (DO NOT EDIT)
CONFIG OSC = HSPLL      ; Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
CONFIG FCMEN = OFF      ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
CONFIG IESO = OFF       ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
; CONFIG2L
CONFIG PWRT = ON        ; Power-up Timer Enable bit (PWRT enabled)
CONFIG BOREN = OFF      ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
CONFIG BORV = 3         ; Brown Out Reset Voltage bits (Minimum setting)
; CONFIG2H
CONFIG WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
; CONFIG3H
CONFIG PBADEN = OFF     ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
CONFIG LPT1OSC = OFF    ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
CONFIG MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
; CONFIG4L
CONFIG LVP = OFF        ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
CONFIG XINST = OFF      ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; GLOBAL SYMBOLS
; You need to add your variables here if you want to debug them.
GLOBAL var1
GLOBAL var2
GLOBAL var3    
GLOBAL bar
GLOBAL var4
GLOBAL var5
GLOBAL varb
GLOBAL pause
GLOBAL svar
GLOBAL speed
GLOBAL barlen
; Define space for the variables in RAM
PSECT udata_acs
var1: DS 1
var2: DS 1
var3: DS 1
bar: DS 1
var4: DS 1
var5: DS 1
varb: DS 1
pause: DS 1
svar: DS 1
speed: DS 1
barlen: DS 1

PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto       main

PSECT CODE
main:
    clrf TRISA ; make PORTA an output
    setf TRISB ; make PORTB an input
   
    movlw 00000111B ; light up RA1, RA2, RA3
    movwf LATA
    call busy_wait
    clrf LATA
   
    clrf var1
    clrf var2
    clrf bar
    clrf var4
    clrf var5
    clrf varb
    clrf pause
    clrf speed
    movlw 50
    movwf svar
    movlw 8
    movwf barlen
   
;------------------------------------------------------      
main_loop:
    call check_buttons
    btfss pause, 0 ; if pause is 0
    call metronome_update
   
    goto main_loop
;------------------------------------------------------      

check_buttons:
   
    btfsc PORTB, 0 ; if RB0 is 1
    goto  rb0_one
    call rb0_0
    rb0:
    
    
    btfsc PORTB, 1 ; if RB1 is 1
    goto rb1_one
    call rb1_0
    rb1:
    
    btfsc PORTB, 2 ; if RB2 is 1
    goto rb2_one
    call rb2_0
    rb2:
    
    btfsc PORTB, 3 ; if RB3 is 1
    goto rb3_one
    call rb3_0
    rb3:    
    
    btfsc PORTB, 4 ; if RB4 is 1   
    goto rb4_one
    call rb4_0
    rb4:
    return
    
rb0_one:
    bsf varb, 0
    goto rb0
rb1_one:
    bsf varb, 1
    goto rb1   
rb2_one:
    bsf varb, 2
    goto rb2   
rb3_one:
    bsf varb, 3
    goto rb3   
rb4_one:
    bsf varb, 4
    goto rb4   
    
    
    
rb0_0:
    btfsc varb, 0 ; if varb0 is 1
    call rb0_clicked
    return

rb0_clicked: ;pause/resume
    bcf varb, 0
    BTG pause, 0
    btfsc pause, 0 ; if pause is 1
    call pause_meturome
    return
   
rb1_0:
    btfsc varb, 1 ; if varb1 is 1
    call rb1_clicked
    return
   
rb1_clicked: ;speed
    bcf varb, 1
    BTG speed, 0
    btfsc speed, 0 ; if speed is 1
    movlw 150
    btfss speed, 0 ; if speed is 0
    movlw 50
    movwf svar
    return
   
rb2_0: ; reset bar length
    btfsc varb, 2 ; if varb2 is 1
    call rb2_clicked
    return  
   
    
 rb2_clicked:
    movlw 7
    cpfslt bar ; if bar is less than 7 skip
    movwf bar
    
    BTG varb, 2
    movlw 8
    movwf barlen
    return

rb3_0: ; decrease bar length
    btfsc varb, 3 ; if varb3 is 1
    call rb3_clicked
    return    

rb3_clicked:
    BTG varb, 3
    movlw 2
    subwf barlen, 1
    return
   
rb4_0: ; increase bar length
    btfsc varb, 4 ; if varb4 is 1
    call rb4_clicked
    return    

rb4_clicked:
    BTG varb, 4
    movlw 2
    addwf barlen, 1
    return
pause_meturome:
    bcf LATA, 0    
    bcf LATA, 1
    bsf LATA, 2
    return    
   
;------------------------------------------------------  
metronome_update:
    btfsc LATA, 2
    bcf LATA, 2
    movlw 7
    addwf var1
    bc var1_overflown
    return

var1_overflown:
    movff svar, var1
    incf var2
    bc var2_overflown
    return

var2_overflown:
    movf barlen, 0
    subwf bar ,0 ; store result in wreg
    cpfslt var3 ;compare var3 with wreg
    clrf bar
    BTG LATA, 0
    movlw 1
    cpfsgt bar
    BTG LATA, 1
    incf bar
    return  
;------------------------------------------------------  
busy_wait:
    clrf var3
    clrf var1
    loop_3:
clrf var2
loop_2:
  movlw 253
  movwf var1
  movf var1
  movf var1
  loop_1:
   incfsz var1
   goto loop_1
 incfsz var2
 goto loop_2
    incfsz var3
    goto loop_3
    return    
;------------------------------------------------------  
end resetVec