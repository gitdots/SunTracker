import argparse

# Define the command-line arguments
parser = argparse.ArgumentParser(description='Set a value for a given axis and pin')
parser.add_argument('pin', help='The pin of the servomotor in BCM mode')
parser.add_argument('value', help='The value to set')

# Parse the command-line arguments
args = parser.parse_args()

# Access the argument values
pin = int(args.pin)
value = int(args.value)

# Print the values for verification
print(f'Setting value {value} for pin {pin}')

import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)
GPIO.setup(pin, GPIO.OUT)
pwm = GPIO.PWM(pin, 50) # set 50MHz
try:
    duty = value / 18 + 2.5
    GPIO.output(pin, True)
    pwm.start(duty)
    sleep(.5)
    pwm.stop()
except KeyboardInterrupt: # If CTRL+C is pressed, exit cleanly:
    print('Keyboard interrupt')
except:
    print('Unknown error occured') 
finally:
    print('Clean up') 
    GPIO.cleanup() # cleanup all GPIO 
