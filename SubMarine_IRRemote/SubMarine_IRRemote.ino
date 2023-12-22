#include <IRremote.hpp>

const uint16_t SUB_ADDRESS = 0x01;

enum class sub_command: uint16_t {
  FORWARD = 0x01,
  BACKWARD = 0x02,
  LEFT = 0x03,
  RIGHT = 0x04,
  DOWN = 0x05,
  UP = 0x06
};

void setup() {
  // put your setup code here, to run once:
  IrSender.begin(2, false, false);
}

void send_command (uint8_t address, sub_command command) {
  IrSender.sendFAST((static_cast<uint8_t>(command) & 0x0F) | ((address & 0x0F) << 4), 3);
}

void loop() {
  // put your main code here, to run repeatedly:

    switch (read) {
        case 'f':
            send_command(SUB_ADDRESS, sub_command::FORWARD);
            break;
        case 'b':
            send_command(SUB_ADDRESS, sub_command::BACKWARD);
            break;
        case 'd':
            send_command(SUB_ADDRESS, sub_command::DOWN);
            break;
        case 'u':
            send_command(SUB_ADDRESS, sub_command::UP);
            break;
        case 'l':
            send_command(SUB_ADDRESS, sub_command::LEFT);
            break;
        case 'r':
            send_command(SUB_ADDRESS, sub_command::RIGHT);
            break;
    }
}
