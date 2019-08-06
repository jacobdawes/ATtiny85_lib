# ATTINY85 Watchdog Timer Examples

## Pinout:
B0-B3 are for indicator LEDs with external series resistors
B4 is connected to the button with an external pullup resistor


## Example Programs:

###bin\_counter\_no\_wdt:
Example of a simple 4 bit binary counter. No watchdog timer is implemented

###bin\_counter\_wdt\_int:
Same 4 bit binary counter. Now, a watchdog timer is implemented configured to interrupt after 4 seconds.

###bin\_counter\_wdt\_res:
Same 4 bit counter. Watchdog is configured to reset entire system

###bin\_counter\_wdt\_wdt\_eeprom:
Same 4 bit counter. Watchdog is configured to interrupt first, saving value of counter to EEPROM. After an additonal 4 seconds, a watchdog reset occurs, resetting the entire system. On reboot, the attiny85 restores the value of the counter from the EEPROM.
