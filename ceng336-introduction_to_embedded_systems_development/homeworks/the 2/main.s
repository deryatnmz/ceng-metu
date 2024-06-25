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
GLOBAL barlen
GLOBAL current_bar
GLOBAL display
GLOBAL delay
GLOBAL paused
GLOBAL prevRB
GLOBAL timer0_h
GLOBAL timer0_l
GLOBAL speed_lev
GLOBAL c0_flag


PSECT udata_acs
barlen: DS 1
current_bar: DS 1
display: DS 1
delay: DS 1
paused: DS 1
prevRB: DS 1
timer0_h: DS 1
timer0_l: DS 1
speed_lev: DS 1
c0_flag: DS 1



PSECT CODE

org 0x0000
  goto main

org 0x0008
    interrupt_service_routine:
    btfsc  INTCON, 2 ;Check Timer0 flag, skip if clear
    goto  timer0_interrupt
    btfsc PIR1, 0 ;Check Timer1 flag, skip if clear
    goto  t1_interrupt
    btfsc INTCON, 0 ;Check RB flag, skip if clear
    goto rb_pressed_interrupt
    retfie  1

main:
    ;output initializations
    clrf TRISD ;RD0-RD7: Segments a to g and point in 7-segment display.
    clrf TRISA ;RA0-RA3: Multiplexer for 7-segment display, leftmost to rightmost.
    clrf TRISC ;RC0: Beat LED. Also connected to a buzzer. RC1: LED indicating the start of a bar.

    ;input initializations
    movlw 11110000B
    movwf TRISB
    ;RB4: Start/stop button.
    ;RB5: Increase button. Affects the speed level if paused, bar length if running.
    ;RB6: Decrease button. Affects the speed level if paused, bar length if running.
    ;RB7: Reset button. Affects the speed level if paused, bar length if running.

    ;initialize variables
    movlw 4
    movwf barlen 

    movlw 1
    movwf current_bar
    
    clrf c0_flag
    clrf display
    clrf delay
    clrf prevRB
    
    bsf paused,7 ; start in paused state 
    
    ;timer0 prescaler initialization
    movlw 00000011B ;disable timer0, 16 bit (65536), prescaler 1:16
    movwf T0CON

    ;timer1 prescaler initialization
    movlw 11000000B ;disable timer1, 16 bit (65536), noprescaler 1:16
    movwf T1CON

    movlw   11100000B ;enable global, peripheral, timer0,RB interrupts
    movwf   INTCON
    bsf PIE1, 0 ;enable timer1 interrupt

    ;timer0 initialization
    ;500ms => 500000 / 16 = 31250 cycle needed (initial)
    ;65536-31250 = 34286 => 0X85EE in hex	
    movlw 6 
    movwf speed_lev 
    movlw 0x85
    movwf timer0_h
    movff timer0_h, TMR0H ;higher byte
    movlw 0xEE
    movwf timer0_l
    movff timer0_l, TMR0L ;lower byte

    ;timer1 initialization
    ;50 ms => 50000 cycle needed
    ;65536-50000 = 15536 => 0x3CB0
    movlw 0x3C
    movwf TMR1H ;higher byte
    movlw 0xB0
    movwf TMR1L ;lower byte
    
    ;enable on_change interrupt
    movf PORTB, 0 ;read RB
    nop ;one cycle
    bcf INTCON, 0 ;clear flag
    bsf INTCON, 3 ;enable on_change interrupt  

main_loop:
    btfsc paused,7
    call seven_segment_display_paused
    btfss paused,7
    call seven_segment_display_running
    bra main_loop

update_speed:
    movlw 1
    cpfseq speed_lev 
    goto s2 ;if current speed_lev is not 1
    ;else update speed
    ;1  1000ms => 1000000 / 16 = 62500 cycle needed
	;65536-62500 = 3036 => 0X0BDC in hex
    movlw 0X0B
    movwf timer0_h
    movlw 0XDC
    movwf timer0_l
    goto iend 
s2:
    movlw 2
    cpfseq speed_lev 
    goto s3 ;if current speed_lev is not 2
    ;else update speed
    ;2  900ms => 900000 / 16 = 56250 cycle needed
	;65536-56250 = 9286 => 0x2446 in hex    
    movlw 0X24
    movwf timer0_h
    movlw 0X46
    movwf timer0_l
    goto iend
s3:
    movlw 3
    cpfseq speed_lev 
    goto s4 ;if current speed_lev is not 3
    ;else update speed
    ;3  800ms => 800000 / 16 = 50000 cycle needed
	;65536-50000 = 15536 => 0X3CB0 in hex   
    movlw 0X3C
    movwf timer0_h
    movlw 0XB0
    movwf timer0_l
    goto iend
s4:
    movlw 4
    cpfseq speed_lev 
    goto s5 ;if current speed_lev is not 4
    ;else update speed
    ;4  700ms => 700000 / 16 = 43750 cycle needed
	;65536-43750 = 21786 => 0X551A in hex 
    movlw 0X55
    movwf timer0_h
    movlw 0X1A
    movwf timer0_l
    goto iend
s5:
    movlw 5
    cpfseq speed_lev 
    goto s6 ;if current speed_lev is not 5
    ;else update speed
    ;5  600ms => 600000 / 16 = 37500 cycle needed
	;65536-37500 = 28036 => 0X6D84 in hex
    movlw 0X6D
    movwf timer0_h
    movlw 0X84
    movwf timer0_l
    goto iend
s6:
    movlw 6
    cpfseq speed_lev 
    goto s7 ;if current speed_lev is not 6
    ;else update speed
    ;6  500ms => 500000 / 16 = 31250 cycle needed (initial)
	;65536-31250 = 34286 => 0X85EE in hex
    movlw 0X85
    movwf timer0_h
    movlw 0XEE
    movwf timer0_l
    goto iend
s7:
    movlw 7
    cpfseq speed_lev 
    goto s8 ;if current speed_lev is not 7
    ;else update speed
    ;7  400ms => 400000 / 16 = 25000 cycle needed
	;65536-25000 = 40536 => 0X9E58 in hex
    movlw 0X9e
    movwf timer0_h
    movlw 0X58
    movwf timer0_l
    goto iend
s8:
    movlw 8
    cpfseq speed_lev 
    goto s9 ;if current speed_lev is not 8
    ;else update speed
    ;8  300ms => 300000 / 16 = 18750 cycle needed
	;65536-18750 = 46786 => 0XB6C2 in hex
    movlw 0XB6
    movwf timer0_h
    movlw 0XC2
    movwf timer0_l
    goto iend
s9:
    ;9  200ms => 200000 / 16 = 12500 cycle needed
	;65536-12500 = 53036 => 0XCF2C in hex    
    movlw 0XCF
    movwf timer0_h
    movlw 0X2C
    movwf timer0_l    
iend:
    movff timer0_h, TMR0H ;higher byte
    movff timer0_l, TMR0L ;lowet byte
    return

   
rb_pressed_interrupt:
    btfsc prevRB, 4 ;if prev RB4 is one
    goto prev_RB4_one
    btfss prevRB, 4 ;if prevRB4 is zero
    goto prev_RB4_zero
  prev_RB4_one:
    btfss PORTB, 4 ;if RB4 is zero
    call RB4_pressed
    goto check_RB5
  prev_RB4_zero:
    btfsc PORTB, 4 ;if RB4 is one
    bsf prevRB, 4 ;set prev RB4
    
 check_RB5:
    btfsc prevRB, 5 ;if prev RB5 is one
    goto prev_RB5_one
    btfss prevRB, 5 ;if prevRB5 is zero
    goto prev_RB5_zero
  prev_RB5_one:
    btfss PORTB, 5 ;if RB5 is zero
    call RB5_pressed
    goto check_RB6
  prev_RB5_zero:
    btfsc PORTB, 5 ;if RB5 is one
    bsf prevRB, 5 ;set prev RB5
    
  check_RB6:
    btfsc prevRB, 6 ;if prev RB6 is one
    goto prev_RB6_one
    btfss prevRB, 6 ;if prevRB6 is zero
    goto prev_RB6_zero
  prev_RB6_one:
    btfss PORTB, 6 ;if RB6 is zero
    call RB6_pressed
    goto check_RB7
  prev_RB6_zero:
    btfsc PORTB, 6 ;if RB6 is one
    bsf prevRB, 6 ;set prev RB6
   
  check_RB7:
    btfsc prevRB, 7 ;if prev RB7 is one
    goto prev_RB7_one
    btfss prevRB, 7 ;if prevRB7 is zero
    goto prev_RB7_zero
  prev_RB7_one:
    btfss PORTB, 7 ;if RB7 is zero
    call RB7_pressed
    goto check_end
  prev_RB7_zero:
    btfsc PORTB, 7 ;if RB7 is one
    bsf prevRB, 7 ;set prev RB7
    
  check_end:  
    bcf INTCON, 0 ;clear flag 
    retfie 1

  RB7_pressed:
    btfss paused, 7 ;if not paused
    goto reset_barlen
    movlw 6
    movwf speed_lev
    call update_speed
    goto r7_end
  reset_barlen:
    movlw 4
    movwf barlen
    cpfsgt current_bar
    goto r7_end
    movlw 1
    movwf current_bar
  r7_end:  
    bcf prevRB, 7
    return              
        
 RB6_pressed:
    btfss paused, 7 ;if not paused
    goto decrease_barlen
    movlw 1
    cpfseq speed_lev 
    goto decs
    goto r6_end ;if current speed_lev is 1
  decs:
    movlw 1
    subwf speed_lev, 1
    call update_speed
    goto r6_end
  decrease_barlen:
    movlw 2
    cpfseq barlen ;check if it is 2
    goto decc
    goto r6_end
  decc:
    movlw 1
    subwf barlen, 1
    movf barlen,0
    cpfsgt current_bar
    goto r6_end
    movlw 1
    movwf current_bar
  r6_end:  
    bcf prevRB, 6
    return   
        
RB5_pressed:
    btfss paused, 7 ;if not paused
    goto increase_barlen
    movlw 9
    cpfseq speed_lev 
    goto incs
    goto r5_end ;if current speed_lev is 9
  incs:
    movlw 1
    addwf speed_lev, 1
    call update_speed
    goto r5_end
  increase_barlen:
    movlw 8
    cpfseq barlen ;check if it is 8 
    goto inc
    goto r5_end
  inc:
    movlw 1
    addwf barlen, 1
  r5_end:  
    bcf prevRB, 5
    return
    
RB4_pressed:
    btg paused, 7
    btfsc paused, 7 
    goto end4
    bsf LATC, 0 ;light c0
    bsf T1CON,0 ;enable timer 1
    movlw 1
    cpfseq current_bar
    goto end4
    bsf LATC, 1 ;light c1
  end4:
    btg T0CON, 7 ;enable/disable timer0
    bcf prevRB, 4
    return

seven_segment_display_running:
    incf delay
    bc switch_running
    return
 switch_running:
    clrf LATD
    btfsc LATA, 1 ;if R1
    goto switch_running_R2
    btfsc LATA, 2 ;if R2
    goto switch_running_R3
    goto switch_running_R1
  switch_running_R1: ;show current_bar
    call show_current_bar
    goto switch_running_end
  switch_running_R2: ;show dash
    movlw 00000100B
    movwf LATA; set RA2
    movlw 01000000B;
    movwf LATD
    goto switch_running_end
  switch_running_R3: ;show barlen
    call show_bar_len
    goto switch_running_end
  switch_running_end:
    clrf delay
    return


seven_segment_display_paused:
    incf delay
    bc switch_paused
    return
switch_paused:
    clrf LATD
    btfsc LATA, 0 ;if R0
    goto switch_paused_R3
    goto switch_paused_R0
  switch_paused_R3: ;show barlen
    call show_speed_lev
    goto switch_paused_end
  switch_paused_R0: ;show P
    movlw 00000001B
    movwf LATA; set RA0
    movlw 01110011B; P
    movwf LATD
  switch_paused_end:
    clrf delay
    return
    
 show_speed_lev:
    movlw 00001000B
    movwf LATA; set RA3
    clrf WREG ;wreg=0
    cpfseq speed_lev ;if speed_lev is not zero
    goto sl_one
    call zero
    goto sl_end
   sl_one:
    addlw 1 ;wreg=1
    cpfseq speed_lev ;if speed_lev is not one
    goto sl_two
    call one
    goto sl_end
   sl_two:
    addlw 1 ;wreg=2
    cpfseq speed_lev ;if speed_lev is not two
    goto sl_three
    call two
    goto sl_end
   sl_three:
    addlw 1 ;wreg=3
    cpfseq speed_lev ;if speed_lev is not three
    goto sl_four
    call three
    goto sl_end
   sl_four:
    addlw 1 ;wreg=4
    cpfseq speed_lev ;if speed_lev is not four
    goto sl_five
    call four
    goto sl_end
   sl_five:
    addlw 1 ;wreg=5
    cpfseq speed_lev ;if speed_lev is not five
    goto sl_six
    call five
    goto sl_end
   sl_six:
    addlw 1 ;wreg=6
    cpfseq speed_lev ;if speed_lev is not six
    goto sl_seven
    call six
    goto sl_end
   sl_seven:
    addlw 1 ;wreg=7
    cpfseq speed_lev ;if speed_lev is not seven
    goto sl_eight
    call seven
    goto sl_end
   sl_eight:
    addlw 1 ;wreg=8
    cpfseq speed_lev ;if speed_lev is not eight
    goto sl_nine
    call eight
    goto sl_end
   sl_nine:
    call nine
   sl_end:
    movf display, 0
    movwf LATD
    return

 show_bar_len:
    movlw 00001000B
    movwf LATA; set RA3
    clrf WREG ;wreg=0
    cpfseq barlen ;if barlen is not zero
    goto l_one
    call zero
    goto l_end
   l_one:
    addlw 1 ;wreg=1
    cpfseq barlen ;if barlen is not one
    goto l_two
    call one
    goto l_end
   l_two:
    addlw 1 ;wreg=2
    cpfseq barlen ;if barlen is not two
    goto l_three
    call two
    goto l_end
   l_three:
    addlw 1 ;wreg=3
    cpfseq barlen ;if barlen is not three
    goto l_four
    call three
    goto l_end
   l_four:
    addlw 1 ;wreg=4
    cpfseq barlen ;if barlen is not four
    goto l_five
    call four
    goto l_end
   l_five:
    addlw 1 ;wreg=5
    cpfseq barlen ;if barlen is not five
    goto l_six
    call five
    goto l_end
   l_six:
    addlw 1 ;wreg=6
    cpfseq barlen ;if barlen is not six
    goto l_seven
    call six
    goto l_end
   l_seven:
    addlw 1 ;wreg=7
    cpfseq barlen ;if barlen is not seven
    goto l_eight
    call seven
    goto l_end
   l_eight:
    addlw 1 ;wreg=8
    cpfseq barlen ;if barlen is not eight
    goto l_nine
    call eight
    goto l_end
   l_nine:
    call nine
   l_end:
    movf display, 0
    movwf LATD
    return

show_current_bar:
    movlw 00000010B
    movwf LATA; set RA1
    clrf WREG ;wreg=0
    cpfseq current_bar ;if current_bar is not zero
    goto cl_one
    call zero
    goto cl_end
   cl_one:
    addlw 1 ;wreg=1
    cpfseq current_bar ;if current_bar is not one
    goto cl_two
    call one
    goto cl_end
   cl_two:
    addlw 1 ;wreg=2
    cpfseq current_bar ;if current_bar is not two
    goto cl_three
    call two
    goto cl_end
   cl_three:
    addlw 1 ;wreg=3
    cpfseq current_bar ;if current_bar is not three
    goto cl_four
    call three
    goto cl_end
   cl_four:
    addlw 1 ;wreg=4
    cpfseq current_bar ;if current_bar is not four
    goto cl_five
    call four
    goto cl_end
   cl_five:
    addlw 1 ;wreg=5
    cpfseq current_bar ;if current_bar is not five
    goto cl_six
    call five
    goto cl_end
   cl_six:
    addlw 1 ;wreg=6
    cpfseq current_bar ;if current_bar is not six
    goto cl_seven
    call six
    goto cl_end
   cl_seven:
    addlw 1 ;wreg=7
    cpfseq current_bar ;if current_bar is not seven
    goto cl_eight
    call seven
    goto cl_end
   cl_eight:
    addlw 1 ;wreg=8
    cpfseq current_bar ;if current_bar is not eight
    goto cl_nine
    call eight
    goto cl_end
   cl_nine:
    call nine
   cl_end:
    movf display, 0
    movwf LATD
    return


timer0_interrupt:
    ; call update_speed
    bsf T1CON, 0 ;enable timer1
    bsf LATC, 0 ;light c0
    bsf c0_flag, 7
    movf barlen, W
    cpfseq current_bar
    goto increment
    movlw 1
    movwf current_bar
    bsf LATC, 1 ;new_bar start light c1
    goto cb_end
  increment:
    movlw 1
    addwf current_bar, 1
  cb_end:
    bcf INTCON, 2 ; clear timer0 flag
    movff timer0_h, TMR0H ;higher byte
    movff timer0_l, TMR0L ;lower byte
    retfie 1

t1_interrupt:
    bcf PIR1, 0 ; clear timer1 flag
    bcf T1CON, 0 ;disable timer1
    movlw 0x3C
    movwf TMR1H ;higher byte
    movlw 0xB0
    movwf TMR1L ;lower byte
    clrf c0_flag
    bcf LATC, 0
    bcf LATC, 1
    retfie 1

;7 segment
zero:
    movlw 0x3F
    movwf display
    return
one:
    movlw 0x06
    movwf display
    return
two:
    movlw 0x5B
    movwf display
    return
three:
    movlw 0x4F
    movwf display
    return
four:
    movlw 0x66
    movwf display
    return
five:
    movlw 0x6D
    movwf display
    return
six:
    movlw 0x7D
    movwf display
    return
seven:
    movlw 0x07
    movwf display
    return
eight: 
    movlw 0x7F
    movwf display
    return
nine:
    movlw 0x6F
    movwf display
    return
