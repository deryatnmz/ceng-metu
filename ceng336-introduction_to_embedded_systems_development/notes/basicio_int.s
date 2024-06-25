; An assembly implementation of the Basic I/O Round-Robin with
; interrupts Software Structure in CENG 336.
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
GLOBAL toggle_led
    
; Access bank DATA segment: allocates memory in data bank for variables
PSECT udata_acs
    toggle_led:    DS 1

; reset Vector CODE segment
; set reset vector to indicate the start of your program
PSECT resetVec,class=CODE,reloc=2
resetVec: goto    main
  org 0x08         ; This sets the address within the current segment
  isrHigh:
    goto high_isr
  org 0x18
  isrLow:  
    retfie  ; Low-priority ISR
  high_isr:
    bcf INTCON,1,a
    movlw 0x01
    movwf toggle_led,a
    retfie  ; High priority ISR

; CODE segment to hold various utility functions
PSECT utils,class=CODE,reloc=2
init_ports:
    movlw 0b00001111
    movwf ADCON1
    movlw 0b11111101  ; Port B pin 0: input, pin 1: output
    movwf TRISB,a
    return

init_vars:
    clrf toggle_led,a
    return

init_interrupts:
    movlw 0x10	    ; Enable external interrupt INT0
    movwf INTCON,a
    bsf INTCON,7,au    ; Enable global interrupts
    return
    
; CODE segment to hold task implementations
PSECT tasks,class=CODE,reloc=2
; *** LED TASK ***
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
    call init_interrupts
    loop:
        call led_task
        bra loop

END resetVec; This (normally) should define the entry point for the program