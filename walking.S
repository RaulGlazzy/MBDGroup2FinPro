
; ROCKY: CREEP GAIT SEQUENCER - PURE AVR 8-BIT ASSEMBLY
; Target: ATmega328P (Arduino Uno) | Clock: 16 MHz


#include <avr/io.h>

.global setup
.global loop


; REGISTER ALLOCATIONS (To hold the current pulse width of each servo)

#define FL_L r16
#define FL_H r17
#define FR_L r18
#define FR_H r19
#define RL_L r20
#define RL_H r21
#define RR_L r22
#define RR_H r23


; ANGLE TO CPU CYCLE MACROS (1 us = 4 cycles)

; 90 degrees = ~1500us = 6000 loops
#define POS_90_L lo8(6000)
#define POS_90_H hi8(6000)

; 110 degrees = ~1700us = 6800 loops
#define POS_110_L lo8(6800)
#define POS_110_H hi8(6800)

; 50 degrees = ~1050us = 4200 loops
#define POS_50_L lo8(4200)
#define POS_50_H hi8(4200)

; SETUP ROUTINE

setup:
    ; Set Pins to Output (PD6=FL, PD7=FR, PB0=RL, PB1=RR)
    sbi _SFR_IO_ADDR(DDRD), 6
    sbi _SFR_IO_ADDR(DDRD), 7
    sbi _SFR_IO_ADDR(DDRB), 0
    sbi _SFR_IO_ADDR(DDRB), 1

    ; Initialize all servos to 90 degrees (Neutral)
    ldi FL_L, POS_90_L
    ldi FL_H, POS_90_H
    ldi FR_L, POS_90_L
    ldi FR_H, POS_90_H
    ldi RL_L, POS_90_L
    ldi RL_H, POS_90_H
    ldi RR_L, POS_90_L
    ldi RR_H, POS_90_H
    
    ; Wait 1000ms for servos to physically reach neutral
    ldi r24, 50 
    rcall generate_pwm_frames
    ret


; MAIN WALKING LOOP (CREEP GAIT)

loop:
    ; --- SEQUENCE 1: STEPPING FORWARD (One by One) ---
    
    ; 1. Move Front Left to 110
    ldi FL_L, POS_110_L
    ldi FL_H, POS_110_H
    ldi r24, 15            ; Generate 15 frames (~300ms sequence delay)
    rcall generate_pwm_frames

    ; 2. Move Rear Right to 110
    ldi RR_L, POS_110_L
    ldi RR_H, POS_110_H
    ldi r24, 15            
    rcall generate_pwm_frames

    ; 3. Move Front Right to 50
    ldi FR_L, POS_50_L
    ldi FR_H, POS_50_H
    ldi r24, 15            
    rcall generate_pwm_frames

    ; 4. Move Rear Left to 50
    ldi RL_L, POS_50_L
    ldi RL_H, POS_50_H
    ldi r24, 15            
    rcall generate_pwm_frames

    ; --- SEQUENCE 2: PAUSE (600ms = 30 frames) ---
    ldi r24, 30
    rcall generate_pwm_frames

    ; --- SEQUENCE 3: RESET TO NEUTRAL (One by One) ---
    
    ; 1. Reset Front Left
    ldi FL_L, POS_90_L
    ldi FL_H, POS_90_H
    ldi r24, 15            
    rcall generate_pwm_frames

    ; 2. Reset Rear Right
    ldi RR_L, POS_90_L
    ldi RR_H, POS_90_H
    ldi r24, 15            
    rcall generate_pwm_frames

    ; 3. Reset Front Right
    ldi FR_L, POS_90_L
    ldi FR_H, POS_90_H
    ldi r24, 15            
    rcall generate_pwm_frames

    ; 4. Reset Rear Left
    ldi RL_L, POS_90_L
    ldi RL_H, POS_90_H
    ldi r24, 15            
    rcall generate_pwm_frames

    ; --- SEQUENCE 4: PAUSE (600ms = 30 frames) ---
    ldi r24, 30
    rcall generate_pwm_frames

    rjmp loop ; Repeat indefinitely


; SOFTWARE PWM ENGINE
; Takes 'r24' as the number of 20ms frames to generate.
; This engine sequentially pulses each servo so they don't overlap, 

generate_pwm_frames:
frame_loop:
    ; --- Pulse Front Left (PD6) ---
    sbi _SFR_IO_ADDR(PORTD), 6
    movw r26, FL_L          ; Copy FL state to X register
    rcall delay_4x
    cbi _SFR_IO_ADDR(PORTD), 6

    ; --- Pulse Front Right (PD7) ---
    sbi _SFR_IO_ADDR(PORTD), 7
    movw r26, FR_L          ; Copy FR state to X register
    rcall delay_4x
    cbi _SFR_IO_ADDR(PORTD), 7

    ; --- Pulse Rear Left (PB0) ---
    sbi _SFR_IO_ADDR(PORTB), 0
    movw r26, RL_L          ; Copy RL state to X register
    rcall delay_4x
    cbi _SFR_IO_ADDR(PORTB), 0

    ; --- Pulse Rear Right (PB1) ---
    sbi _SFR_IO_ADDR(PORTB), 1
    movw r26, RR_L          ; Copy RR state to X register
    rcall delay_4x
    cbi _SFR_IO_ADDR(PORTB), 1

    ; --- Pad the rest of the 20ms frame ---
    ; 4 servos * ~1.5ms = ~6ms used. We wait ~14ms here.
    ldi r26, lo8(56000)
    ldi r27, hi8(56000)
    rcall delay_4x

    dec r24                 ; Decrement frame counter
    brne frame_loop         ; Loop until r24 hits 0
    ret

; CYCLE-ACCURATE TIMING SUBROUTINE
; Consumes exactly (X * 4) CPU cycles. 
; At 16MHz, setting X=1000 delays for exactly 250 microseconds.

delay_4x:
    sbiw r26, 1             ; Subtract 1 from X register (2 cycles)
    brne delay_4x           ; Branch if not equal to zero (2 cycles)
    ret                     ; Return