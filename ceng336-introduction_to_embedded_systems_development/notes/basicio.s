; An assembly implementation of the Basic I/O Round-Robin Software 
; Structure in CENG 336.
;
; Task: PortB pin 1 should toggle when the user presses a button 
; connected to PortB pin 0
;
; Make sure to add "-Wl,-PresetVec=0x0,-Wl,-Map=mapfile.map" 
; to pic-as global options to make sure resetVec is properly
; located during linking
PROCESSOR 18F4620
    
#include <xc.inc>

; configuration flag settings
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

; variables to be accessed globally (necessary only if multiple files/modules are used)
GLOBAL button_state, toggle_led
    
; Access bank DATA segment: allocates memory in data bank for variables
PSECT udata_acs
    button_state:  DS 1
    toggle_led:    DS 1

; reset Vector CODE segment
; set reset vector to indicate the start of your program
PSECT resetVec,class=CODE,reloc=2
resetVec: 
    goto    main
  ORG 0x08  ; ORG sets the address within the *current segment*
  isrHigh: 
    retfie  ; High priority ISR
  ORG 0x18
    isrLow:  
    retfie  ; Low-priority ISR

; CODE segment to hold various utility functions
PSECT utils, class=CODE, reloc=2
init_ports:
    movlw 0b00001111
    movwf ADCON1,a
    movlw 0b11111101  ; Port B pin 0: input, pin 1: output
    movwf TRISB,a
    return

init_vars:
    ; button_state should reflect initial state of PortB pin 0
    movf PORTB,a,w
    andlw 0x01
    movwf button_state,a
    clrf toggle_led,a
    return

; CODE segment to hold task implementations
PSECT tasks,class=CODE,reloc=2

; ****** BUTTON TASK *********
button_task:
    movf button_state,a,w 
    bz   button_zero
    bra button_notzero

  button_zero:
    btfss PORTB,0,a
    bra button_finish   ; button_state = 0 and button = 0, return
    movlw 0x01
    movwf button_state,a

    movlw 0x01		
    movwf toggle_led,a	; Signals led_task to toggle
    bra button_finish
    
  button_notzero:
    btfsc PORTB,0,a
    bra button_finish   ; button_state = 1 and button = 1, return
    movlw 0x00
    movwf button_state,a
  button_finish:
    return
    
; ****** LED TASK *********
led_task:
    movf toggle_led,a,w
    bz   led_finish
    movlw 0x02
    xorwf LATB,a,f
    clrf toggle_led,a
  led_finish:
    return

; CODE segment to hold the main routine
PSECT main,class=CODE,reloc=2
main:
    
    call init_vars
    call init_ports
    loop:
        call button_task
        call led_task
        bra loop

END resetVec; This (normally) should define the entry point for the program