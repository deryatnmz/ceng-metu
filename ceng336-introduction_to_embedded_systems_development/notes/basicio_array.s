; An assembly implementation of an extended version of the Basic 
; I/O Round-Robin Software Structure in CENG 336. This implementation
; modularizes tasks and uses arrays to hold various values
;
; Task: Certain PortB pins should toggle when the user presses a button 
; connected to other PortB pins as detailed below
;   RB0(input) toggles RB1(output)
;   RB2(input) toggles RB3(output)
;   RB4(input) toggles RB5(output)
;   RB6(input) toggles RB7(output)
;
; Make sure to add "-Wl,-PresetVec=0x0,-Wl,-Map=mapfile.map" 
; to pic-as global options to make sure resetVec is properly
; located during linking
PROCESSOR 18F4620
#include <xc.inc>

; configuration flag settings
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

; variables to be accessed globally (necessary only if multiple files/modules are used)
GLOBAL button_state, toggle_led, pin_arg, cnt, inp_mask, out_mask
    
; Access bank DATA segment: allocates memory in data bank for variables
PSECT udata_acs
    button_state:  DS 4 ; Will be an array to hold states of 4 buttons
    toggle_led:    DS 4 ; Will be an array to hold toggle signals for 4 leds
    pin_arg:       DS 1 ; Will be 0,1,2 or 3, choosing which LED is current
    cnt:           DS 1 ; Temporary count
    inp_mask:      DS 1 ; Input mask for Port B (0x01, 0x04, 0x10 or 0x40)
    out_mask:      DS 1 ; Output mask for Port B (0x02, 0x08, 0x20 or 0x80)

; reset Vector CODE segment
; set reset vector to indicate the start of your program
PSECT resetVec,class=CODE,reloc=2
resetVec: goto    main
  org 0x08         ; This sets the address within the current segment
  isrHigh: 
    retfie  ; High priority ISR
  org 0x18
  isrLow:  
    retfie  ; Low-priority ISR

; CODE segment to hold various utility functions
PSECT utils, reloc=2
; Function to initialize port settings
init_ports:
    movlw 0b00001111;
    movwf ADCON1,a
    movlw 0b01010101  ; Port B pin 0: input, pin 1: output
    movwf TRISB,a
    return

; Function to initialize variables
init_vars:
    movf PORTB,a,w
    andlw 0x01
    lfsr 0,button_state ; Clear all button states (indirect access)
    clrf POSTINC0,a       ; Should ideally read port states to initialize, 
    clrf POSTINC0,a	; but keeping it simple for now.
    clrf POSTINC0,a
    clrf POSTINC0,a
    lfsr 0,toggle_led   ; Cler all toggle flags (indirect access)
    clrf POSTINC0,a
    clrf POSTINC0,a
    clrf POSTINC0,a
    clrf POSTINC0,a
    return

; Function that does FSR0 <- FSR0+pin_arg
; This essentially implements array access for whatever initial value is loaded
; onto FSR0
lfsr_adjust:
    movf pin_arg,a,w
    addwf FSR0L,a,f
    andlw 0x00
    addwfc FSR0H,a,f
    return

 ; Computes PORTB masks based on pin_arg.
 ; inp_mask = 0x01 << (2*pin_arg)
 ; out_mask = 0x02 << (2*pin_arg)
compute_masks:
    movlw 0x01
    movwf inp_mask,a
    movlw 0x02
    movwf out_mask,a
    
    movf pin_arg,a,w
    movwf cnt,a
    rlncf cnt,a,f
    mask_loop:
	rlncf inp_mask,a,f
	rlncf out_mask,a,f
	decfsz cnt,a,f
	bra mask_loop
    return
    
; CODE segment to hold task implementations
PSECT tasks,reloc=2
; *** BUTTON TASK ***
button_task:
    lfsr 0,button_state	; Use indirect access to retrieve button_state
    call lfsr_adjust
    movf INDF0,a,w
    bz   button_zero
    bra button_notzero
  button_zero:    
    movf PORTB,a,w
    andwf inp_mask,a,w
    bz button_finish    ; button_state = 0 and button = 0, return

    movlw 0x01
    movwf INDF0,a

    lfsr 0,toggle_led	; Use indirect access to set toggle_led
    call lfsr_adjust
    movlw 0x01
    movwf INDF0,a
    bra button_finish
    
  button_notzero:
    movf PORTB,a,w
    andwf inp_mask,a,w
    bnz button_finish    ; button_state = 1 and button = 1, return
    movlw 0x00
    movwf INDF0,a
  button_finish:
    return
    
; *** LED TASK ***
led_task:
    lfsr 0,toggle_led	; Use indirect access to retrieve toggle_led
    call lfsr_adjust
    movf INDF0,a,w
    bz   led_finish
    movf out_mask,a,w	; Use the output mask to toggle PORTB pin
    xorwf LATB,a,f
    clrf INDF0,a
  led_finish:
    return

; CODE segment to hold the main routine
PSECT main,reloc=2
main:
    call init_vars
    call init_ports
    loop:
	clrf pin_arg,a	    ; Pins 1 and 0
	call compute_masks
	call button_task
        call led_task

	incf pin_arg,a,f    ; Pins 3 and 2
	call compute_masks
	call button_task
        call led_task

	incf pin_arg,a,f    ; Pins 5 and 4
	call compute_masks
	call button_task
        call led_task

	incf pin_arg,a,f    ; Pins 7 and 6
	call compute_masks
	call button_task
        call led_task
	goto loop

END resetVec; This (normally) should define the entry point for the program