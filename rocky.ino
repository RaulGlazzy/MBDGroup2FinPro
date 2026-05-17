#include <LiquidCrystal.h>
#include <Servo.h>

// ----------------------------------------------------------------------------
// LCD SETUP
// ----------------------------------------------------------------------------
// Arduino Pins: RS=12, EN=11, D4=5, D5=4, D6=3, D7=2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// ----------------------------------------------------------------------------
// SERVO SETUP
// ----------------------------------------------------------------------------
Servo servoFL;  // Front Left
Servo servoFR;  // Front Right
Servo servoRL;  // Rear Left
Servo servoRR;  // Rear Right

#define PIN_FL 6
#define PIN_FR 7
#define PIN_RL 8
#define PIN_RR 9

// ----------------------------------------------------------------------------
// FACE HELPER FUNCTION
// ----------------------------------------------------------------------------
void draw_face(const char* face, const char* text) {
    lcd.clear();
    
    int pad_face = (16 - strlen(face)) / 2;
    int pad_text = (16 - strlen(text)) / 2;
    
    if (pad_face < 0) pad_face = 0;
    if (pad_text < 0) pad_text = 0;
    
    lcd.setCursor(pad_face, 0);
    lcd.print(face);
    
    lcd.setCursor(pad_text, 1);
    lcd.print(text);
}

// ----------------------------------------------------------------------------
// SETUP
// ----------------------------------------------------------------------------
void setup() {
    Serial.begin(9600);
    
    lcd.begin(16, 2);
    draw_face("( o _ o )", "Booting...");
    delay(1500);
    
    // Attach servos
    servoFL.attach(PIN_FL);
    servoFR.attach(PIN_FR);
    servoRL.attach(PIN_RL);
    servoRR.attach(PIN_RR);

    // Initial neutral stance
    servoFL.write(90);
    servoFR.write(90);
    servoRL.write(90);
    servoRR.write(90);
    delay(1000);
}

// ----------------------------------------------------------------------------
// MAIN WALKING LOOP (CREEP GAIT)
// ----------------------------------------------------------------------------
void loop() {
    int forward_angle = 110;
    int backward_angle = 50;
    
    // The delay between EACH individual servo movement
    int sequence_delay = 300; 

    // ---------------------------------------------------------
    // SEQUENCE 1: STEPPING FORWARD (One by One)
    // ---------------------------------------------------------
    draw_face("( ^ _ ^ )", "Creeping...");

    servoFL.write(forward_angle);  // 1. Move Front Left
    delay(sequence_delay);

    servoRR.write(forward_angle);  // 2. Move Rear Right
    delay(sequence_delay);

    servoFR.write(backward_angle); // 3. Move Front Right
    delay(sequence_delay);

    servoRL.write(backward_angle); // 4. Move Rear Left
    delay(sequence_delay);

    // ---------------------------------------------------------
    // SEQUENCE 2: PAUSE
    // ---------------------------------------------------------
    draw_face("( - _ - )", "Pause...");
    delay(600);

    // ---------------------------------------------------------
    // SEQUENCE 3: RESET TO NEUTRAL (One by One)
    // ---------------------------------------------------------
    draw_face("( > _ < )", "Resetting...");

    servoFL.write(90);  // 1. Reset Front Left
    delay(sequence_delay);

    servoRR.write(90);  // 2. Reset Rear Right
    delay(sequence_delay);

    servoFR.write(90);  // 3. Reset Front Right
    delay(sequence_delay);

    servoRL.write(90);  // 4. Reset Rear Left
    delay(sequence_delay);

    // ---------------------------------------------------------
    // SEQUENCE 4: PAUSE
    // ---------------------------------------------------------
    draw_face("( - _ - )", "Zzz...");
    delay(600);
}