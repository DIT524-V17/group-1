import socket
import RPi.GPIO as GPIO
import time

sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.bind(('',5000))
sock.listen(25)

GPIO.setmode(GPIO.BOARD)

servoMotor1 = 11
servoMotor2 = 12

GPIO.setup(servoMotor1, GPIO.OUT)
GPIO.setup(servoMotor2, GPIO.OUT)

pwm1 = GPIO.PWM(servoMotor1, 50)
pwm2 = GPIO.PWM(servoMotor2, 50)

pwm1.start(5)
pwm2.start(5)

while True:
    newsock,address=sock.accept()
    result=newsock.recv(8192)

    DC1 = 1./20.*(int(result.decode("utf-8").split()[0])) + 3
    DC2 = 1./20.*(180 - int(result.decode("utf-8").split()[1])) + 3

    if DC1 >= 3 and DC1 <= 12:
        pwm1.ChangeDutyCycle(DC1)

    time.sleep(.01)

    if DC2 >= 3 and DC2 <= 12:
        pwm2.ChangeDutyCycle(DC2)

    time.sleep(.01)

pwm1.stop()
pwm2.stop()
GPIO.cleanup()
