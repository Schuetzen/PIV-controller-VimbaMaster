from gpiozero import LED
from time import sleep

# Initialize GPIO17 as an LED (even if it's not an actual LED)
led = LED(17)

while True:
    led.on()  # Turn on GPIO17
    sleep(1)  # Wait for 10 seconds

    led.off()  # Turn off GPIO17