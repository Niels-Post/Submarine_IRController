#include <IRremote.hpp>

const uint16_t MY_ADDRESS = 0x01;

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
  send_command(MY_ADDRESS, sub_command::FORWARD);
  delay(1000);
  send_command(MY_ADDRESS, sub_command::BACKWARD);
  delay(1000);  
  send_command(MY_ADDRESS, sub_command::LEFT);
  delay(1000);
    send_command(MY_ADDRESS, sub_command::RIGHT);
  delay(1000);
    send_command(MY_ADDRESS, sub_command::DOWN);
  delay(1000);
    send_command(MY_ADDRESS, sub_command::UP);
  delay(1000);
}
