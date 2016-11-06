#include "gpio.h"

#include <stdint.h>

#include "unity.h"

void setup_test(void) { setbuf(stdout, NULL); }

void teardown_test(void) {}

// gpio_init

// Tests that the GPIO init works. This should simply return true 100% of time unless the test is on
// x86 and the configuration is incorrect in which case fix it!
void test_gpio_init(void) { TEST_ASSERT_TRUE(gpio_init()); }

// gpio_init_pin

// Test that a valid gpio configuration will work.
void test_valid_gpio_init_pin(void) {
  // Default settings for a pin.
  GPIOSettings settings = {GPIO_DIR_IN, GPIO_STATE_LOW, GPIO_RES_NONE, GPIO_ALTFN_NONE};
  // A pin that should be valid on all configurations.
  GPIOAddress address = {0, 0};
  TEST_ASSERT_TRUE(gpio_init());
  TEST_ASSERT_TRUE(gpio_init_pin(&address, &settings));
}

// Tests a set of addresses far outside normal range.
void test_invalid_address_gpio_init_pin(void) {
  // Default settings for a pin.
  GPIOSettings settings = {GPIO_DIR_IN, GPIO_STATE_LOW, GPIO_RES_NONE, GPIO_ALTFN_NONE};
  TEST_ASSERT_TRUE(gpio_init());
  // A port that should be invalid on all configurations.
  GPIOAddress address = {100, 0};
  TEST_ASSERT_FALSE(gpio_init_pin(&address, &settings));
  address.pin = 100;
  TEST_ASSERT_FALSE(gpio_init_pin(&address, &settings));
  address.port = 0;
  TEST_ASSERT_FALSE(gpio_init_pin(&address, &settings));
}

// Tests a set of settings outside normal range.
void test_invalid_settings_gpio_init_pin(void) {
  // Bad settings for a pin.
  GPIOSettings settings = {NUM_GPIO_DIR, GPIO_STATE_LOW, GPIO_RES_NONE, GPIO_ALTFN_NONE};
  TEST_ASSERT_TRUE(gpio_init());
  // A port that should be valid on all configurations.
  GPIOAddress address = {0, 0};
  TEST_ASSERT_FALSE(gpio_init_pin(&address, &settings));
  settings.direction = GPIO_DIR_IN;
  settings.state = NUM_GPIO_STATE;
  TEST_ASSERT_FALSE(gpio_init_pin(&address, &settings));
  settings.state = GPIO_STATE_LOW;
  settings.resistor = NUM_GPIO_RES;
  TEST_ASSERT_FALSE(gpio_init_pin(&address, &settings));
  settings.resistor = GPIO_RES_NONE;
  settings.alt_function = NUM_GPIO_ALTFN;
  TEST_ASSERT_FALSE(gpio_init_pin(&address, &settings));
}

// Tests that if the pin is set to direction out a high state will be picked up in the input
// register.
void test_output_gpio_init_pin(void) {
  // Default settings for a pin.
  GPIOSettings settings = {GPIO_DIR_IN, GPIO_STATE_HIGH, GPIO_RES_NONE, GPIO_ALTFN_NONE};
  // A pin that should be valid on all configurations.
  GPIOAddress address = {0, 0};
  TEST_ASSERT_TRUE(gpio_init());
  TEST_ASSERT_TRUE(gpio_init_pin(&address, &settings));
  GPIOState state;
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  // Check low (assuming nothing is being input on the pin!)
  TEST_ASSERT_EQUAL(GPIO_STATE_LOW, state);
  settings.direction = GPIO_DIR_OUT;
  TEST_ASSERT_TRUE(gpio_init_pin(&address, &settings));
  // Check high
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  TEST_ASSERT_EQUAL(GPIO_STATE_HIGH, state);
}

// TODO(ckitagawa): Figure out how to verify: PUPDR, ALTFN.

// gpio_set_pin_state

// Test that a valid state change will work.
void test_valid_gpio_set_pin_state(void) {
  // Default output settings for a pin.
  GPIOSettings settings = {GPIO_DIR_OUT, GPIO_STATE_LOW, GPIO_RES_NONE, GPIO_ALTFN_NONE};
  // A pin that should be valid on all boards.
  GPIOAddress address = {0, 0};
  TEST_ASSERT_TRUE(gpio_init());
  TEST_ASSERT_TRUE(gpio_init_pin(&address, &settings));
  GPIOState state;
  // OFF to ON
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  TEST_ASSERT_EQUAL(GPIO_STATE_LOW, state);
  TEST_ASSERT_TRUE(gpio_set_pin_state(&address, GPIO_STATE_HIGH));
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  TEST_ASSERT_EQUAL(GPIO_STATE_HIGH, state);
  // ON to OFF
  TEST_ASSERT_TRUE(gpio_set_pin_state(&address, GPIO_STATE_LOW));
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  TEST_ASSERT_EQUAL(GPIO_STATE_LOW, state);
}

// Test that an invalid address is caught.
void test_invalid_address_gpio_set_pin_state(void) {
  TEST_ASSERT_TRUE(gpio_init());
  // A port that should be invalid on all configurations.
  GPIOAddress address = {100, 0};
  TEST_ASSERT_FALSE(gpio_set_pin_state(&address, GPIO_STATE_LOW));
  address.pin = 100;
  TEST_ASSERT_FALSE(gpio_set_pin_state(&address, GPIO_STATE_LOW));
  address.port = 0;
  TEST_ASSERT_FALSE(gpio_set_pin_state(&address, GPIO_STATE_LOW));
}

// Test that an invalid state is caught.
void test_invalid_state_gpio_set_pin_state(void) {
  TEST_ASSERT_TRUE(gpio_init());
  // A port that should be valid on all configurations.
  GPIOAddress address = {0, 0};
  TEST_ASSERT_FALSE(gpio_set_pin_state(&address, NUM_GPIO_STATE));
  TEST_ASSERT_FALSE(gpio_set_pin_state(&address, -1));
}

// gpio_toggle_state

// Test that a valid state toggle will work.
void test_valid_gpio_toggle_state(void) {
  // Default output settings for a pin.
  GPIOSettings settings = {GPIO_DIR_OUT, GPIO_STATE_LOW, GPIO_RES_NONE, GPIO_ALTFN_NONE};
  // A pin that should be valid on all boards.
  GPIOAddress address = {0, 0};
  TEST_ASSERT_TRUE(gpio_init());
  TEST_ASSERT_TRUE(gpio_init_pin(&address, &settings));
  GPIOState state;
  // OFF to ON
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  TEST_ASSERT_EQUAL(GPIO_STATE_LOW, state);
  TEST_ASSERT_TRUE(gpio_toggle_state(&address));
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  TEST_ASSERT_EQUAL(GPIO_STATE_HIGH, state);
  // ON to OFF
  TEST_ASSERT_TRUE(gpio_toggle_state(&address));
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  TEST_ASSERT_EQUAL(GPIO_STATE_LOW, state);
}

// Test that an invalid address is caught.
void test_invalid_address_gpio_toggle_state(void) {
  TEST_ASSERT_TRUE(gpio_init());
  // A port that should be invalid on all configurations.
  GPIOAddress address = {100, 0};
  TEST_ASSERT_FALSE(gpio_toggle_state(&address));
  address.pin = 100;
  TEST_ASSERT_FALSE(gpio_toggle_state(&address));
  address.port = 0;
  TEST_ASSERT_FALSE(gpio_toggle_state(&address));
}

// gpio_get_value

// Test that a valid get value will work. This doubles as a test to verify that the state setting on
// gpio_init_pin works.
void test_valid_gpio_get_value(void) {
  // Default output settings for a pin.
  GPIOSettings settings = {GPIO_DIR_OUT, GPIO_STATE_LOW, GPIO_RES_NONE, GPIO_ALTFN_NONE};
  // A pin that should be valid on all boards.
  GPIOAddress address = {0, 0};
  TEST_ASSERT_TRUE(gpio_init());
  TEST_ASSERT_TRUE(gpio_init_pin(&address, &settings));
  GPIOState state;
  // Get Low
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  TEST_ASSERT_EQUAL(GPIO_STATE_LOW, state);
  settings.state = GPIO_STATE_HIGH;
  // Get High
  TEST_ASSERT_TRUE(gpio_init_pin(&address, &settings));
  TEST_ASSERT_TRUE(gpio_get_value(&address, &state));
  TEST_ASSERT_EQUAL(GPIO_STATE_HIGH, state);
}

// Test that an invalid address is caught.
void test_invalid_address_gpio_get_value(void) {
  TEST_ASSERT_TRUE(gpio_init());
  // A port that should be invalid on all configurations.
  GPIOAddress address = {100, 0};
  GPIOState state;
  TEST_ASSERT_FALSE(gpio_get_value(&address, &state));
  address.pin = 100;
  TEST_ASSERT_FALSE(gpio_get_value(&address, &state));
  address.port = 0;
  TEST_ASSERT_FALSE(gpio_get_value(&address, &state));
}
