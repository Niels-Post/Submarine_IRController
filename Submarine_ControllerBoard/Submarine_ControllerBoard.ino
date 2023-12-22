#include <IRremote.hpp>

/**
 * Define the address of this submarine. Commands for other addresses are ignored
 */
const uint8_t MY_ADDRESS = 0x01;

enum class sub_command : uint16_t {
    FORWARD = 0x01,
    BACKWARD = 0x02,
    LEFT = 0x03,
    RIGHT = 0x04,
    DOWN = 0x05,
    UP = 0x06
};


// Pins for the IR receiver. Pins are used in the following order {3V3, GND, SIGNAL}
uint16_t s_ir[3] = {D10, D11, D12};

// Control pins for the 3 motors in the submarine
uint16_t m_steer[2] = {D3, D4};
uint16_t m_depth[2] = {D26, D27};
uint16_t m_speed[2] = {D28, D29};


/**
 * Initialize the pins for a motor in a default LOW-LOW state
 * @param motor Array of 2 motor driving pins
 */
void motor_init(uint16_t motor[2]) {
    pinMode(motor[0], OUTPUT);
    pinMode(motor[1], OUTPUT);

    digitalWrite(motor[0], LOW);
    digitalWrite(motor[1], LOW);
}

/**
 * Set the on state and direction of a motor. Note that the direction of a DC motor can be reversed by swapping the motor's signal wires.
 * @param motor Array of 2 motor driving pins
 * @param on True if the motor should be spinning
 * @param direction True for forward, False for backward
 */
void motor_set(uint16_t motor[2], bool on, bool direction) {
    digitalWrite(motor[0], false);
    digitalWrite(motor[1], false);

    digitalWrite(motor[0], on && direction);
    digitalWrite(motor[1], on && !direction);
}

/**
 * Initialize the IR receiver pins
 * @param ir Array of 3 pins for an IR receiver (VS1838B). Pins are in the order: {VCC, GND, SIG}
 */
void ir_init(uint16_t ir[3]) {
    pinMode(ir[0], OUTPUT);
    pinMode(ir[1], OUTPUT);

    digitalWrite(ir[0], HIGH);
    digitalWrite(ir[1], LOW);

    IrReceiver.begin(ir[2], false);
}

/**
 * Try to receive an IR command, and match it against the submarine address.
 * If the address matches, execute the required action.
 * All actions are automatically stopped .5 seconds after the last command was received.
 */
void ir_get() {
    static uint64_t last_time = millis();
    if (IrReceiver.decodeFAST()) {
        uint16_t data = IrReceiver.decodedIRData.decodedRawData;

        uint8_t address = (data >> 4) & 0x000F;
        uint8_t command = data & 0x000F;

        if (address == MY_ADDRESS) {
            last_time = millis();
            switch (static_cast<sub_command>(command)) {
                case sub_command::FORWARD:
                    motor_set(m_speed, true, true);
                    break;
                case sub_command::BACKWARD:

                    motor_set(m_speed, true, false);
                    break;
                case sub_command::LEFT:

                    motor_set(m_steer, true, true);
                    break;
                case sub_command::RIGHT:

                    motor_set(m_steer, true, false);
                    break;
                case sub_command::DOWN:
                    motor_set(m_depth, true, true);
                    break;
                case sub_command::UP:

                    motor_set(m_depth, true, false);
                    break;
            }
        }

        IrReceiver.resume();
    }

    // Stop all motors .5 seconds after the last command received
    if (millis() > (last_time + 500)) {
        last_time = millis();
        motor_set(m_speed, false, false);
        motor_set(m_depth, false, false);
        motor_set(m_steer, false, false);
        IrReceiver.resume();
    }
}


void setup() {
    Serial.begin(115200);
    motor_init(m_speed);
    motor_init(m_depth);
    motor_init(m_steer);
    ir_init(s_ir);
}


void loop() {
    ir_get();
}
