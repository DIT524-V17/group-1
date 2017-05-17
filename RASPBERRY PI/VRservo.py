import socket
import RPi.GPIO as GPIO
import time

# create a object
sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
# open a port and listen it
sock.bind(('',5000))
sock.listen(25)

# choose the BOARD for pin nummering
GPIO.setmode(GPIO.BOARD)

# declare servo motors pins
servoMotor1 = 11
servoMotor2 = 12

# set up channels as outputs
GPIO.setup(servoMotor1, GPIO.OUT)
GPIO.setup(servoMotor2, GPIO.OUT)

# set up channels and frequency
pwm1 = GPIO.PWM(servoMotor1, 50)
pwm2 = GPIO.PWM(servoMotor2, 50)

# duty cycle is 5 for ech servomotor at startup
pwm1.start(5)
pwm2.start(5)

while True:
    # accept connections from outside
    newsock,address=sock.accept()
    # set incoming packet size
    result=newsock.recv(8192)
    
    # formula for change degrees to duty cycle:
    # recieving data decodes to utf-8. first postion of string array is x
    # and second is y after spliting of strings.
    DC1 = 1./20.*(int(result.decode("utf-8").split()[0])) + 3
    DC2 = 1./20.*(180 - int(result.decode("utf-8").split()[1])) + 3
    
    # since values for motors is between 3 and 12 for dc
    if DC1 >= 3 and DC1 <= 12:
        pwm1.ChangeDutyCycle(DC1)

    time.sleep(.01)

    if DC2 >= 3 and DC2 <= 12:
        pwm2.ChangeDutyCycle(DC2)
    
    # a little delay before next data
    time.sleep(.01)

# stoping PWM and cleaning up GPIO pins for next usage
# (anyway these code lines are unreachable at execution and termination of the program)
pwm1.stop()
pwm2.stop()
GPIO.cleanup()
