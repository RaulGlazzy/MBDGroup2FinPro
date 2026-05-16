#include <Arduino.h>
#include <LiquidCrystal.h>

// ----------------------------------------------------------------------------
// LCD SETUP
// ----------------------------------------------------------------------------
const int rs = 19, en = 18, d4 = 5, d5 = 17, d6 = 16, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ----------------------------------------------------------------------------
// SERVO CONFIGURATION
// ----------------------------------------------------------------------------
#define PIN_FL 13  // Front Left
#define PIN_FR 12  // Front Right
#define PIN_RL 14  // Rear Left
#define PIN_RR 27  // Rear Right

#define SERVO_MIN_US    500
#define SERVO_MAX_US    2500
#define SERVO_PERIOD_US 20000UL

// Helper: Converts angles (0-180) to 16-bit duty cycle
uint32_t angle_to_duty(float angle_deg) {
    if (angle_deg < 0.0f)   angle_deg = 0.0f;
    if (angle_deg > 180.0f) angle_deg = 180.0f;
    uint32_t pulse_us = SERVO_MIN_US + (uint32_t)((angle_deg / 180.0f) * (SERVO_MAX_US - SERVO_MIN_US));
    uint32_t duty = (pulse_us * 65535UL) / SERVO_PERIOD_US;
    if (duty > 65535UL) duty = 65535UL;
    return duty;
}

void move_servo(uint8_t pin, float angle) {
    ledcWrite(pin, angle_to_duty(angle));
}

// ----------------------------------------------------------------------------
// FACE HELPER FUNCTION
// ----------------------------------------------------------------------------
void draw_face(const char* face, const char* text) {
    lcd.clear();
    
    // Automatically center the text on the screen
    int pad_face = (16 - strlen(face)) / 2;
    int pad_text = (16 - strlen(text)) / 2;
    
    // Safety check to prevent negative padding
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
    Serial.begin(115200);
    
    // Start LCD and show custom boot screen
    lcd.begin(16, 2);
    lcd.setCursor(1, 0);
    lcd.print("Group E Walker");
    lcd.setCursor(1, 1);
    lcd.print("ID: 2406368864");
    delay(2000);
    
    // Attach servos
    ledcAttach(PIN_FL, 50, 16);
    ledcAttach(PIN_FR, 50, 16);
    ledcAttach(PIN_RL, 50, 16);
    ledcAttach(PIN_RR, 50, 16);

    // Move to neutral position (90 degrees is the physical "0,0" center)
    move_servo(PIN_FL, 90);
    move_servo(PIN_FR, 90);
    move_servo(PIN_RL, 90);
    move_servo(PIN_RR, 90);
}

// ----------------------------------------------------------------------------
// MAIN WALKING LOOP
// ----------------------------------------------------------------------------
void loop() {
    // Walk cycle parameters
    float forward_angle = 120.0;
    float backward_angle = 60.0;
    int step_delay = 500;  // Time spent moving
    int sleep_delay = 800; // Time spent resting

    // ---------------------------------------------------------
    // PHASE 1: Step forward (Moving = Yeay Face)
    // ---------------------------------------------------------
    draw_face("( ^ _ ^ )", "Yeay!");
    move_servo(PIN_FL, forward_angle);
    move_servo(PIN_RR, forward_angle);
    move_servo(PIN_FR, backward_angle);
    move_servo(PIN_RL, backward_angle);
    delay(step_delay);

    // ---------------------------------------------------------
    // PHASE 2: Neutral stand (Resting = Sleeping Face)
    // ---------------------------------------------------------
    draw_face("( - _ - )", "Zzz...");
    move_servo(PIN_FL, 90);
    move_servo(PIN_RR, 90);
    move_servo(PIN_FR, 90);
    move_servo(PIN_RL, 90);
    delay(sleep_delay);

    // ---------------------------------------------------------
    // PHASE 3: Step alternate (Moving = Yeay Face)
    // ---------------------------------------------------------
    draw_face("( ^ _ ^ )", "Yeay!");
    move_servo(PIN_FL, backward_angle);
    move_servo(PIN_RR, backward_angle);
    move_servo(PIN_FR, forward_angle);
    move_servo(PIN_RL, forward_angle);
    delay(step_delay);

    // ---------------------------------------------------------
    // PHASE 4: Neutral stand (Resting = Sleeping Face)
    // ---------------------------------------------------------
    draw_face("( - _ - )", "Zzz...");
    move_servo(PIN_FL, 90);
    move_servo(PIN_RR, 90);
    move_servo(PIN_FR, 90);
    move_servo(PIN_RL, 90);
    delay(sleep_delay);
}