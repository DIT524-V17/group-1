import pygame
import serial
import math
import os

joysticks = []
tmpJS = []
detected = True
angle = 0;
clock = pygame.time.Clock()

ser = serial.Serial('/dev/ttyACM0',9600)

while True:
        clock.tick(10)
        if detected:
                for fn in os.listdir('/dev/input'):
                        if fn.startswith('js'):
                                tmpJS.append(fn)
                if len(tmpJS) > 0:
                        detected = False
                        pygame.init()
                        for i in range(0, pygame.joystick.get_count()):
                                joysticks.append(pygame.joystick.Joystick(i))
                                joysticks[-1].init()
                                print "Detected joystick '",joysticks[-1].get_name(),"'"

        else:
                event = pygame.event.wait()
                if event.type == pygame.JOYAXISMOTION:
                        x , y = joysticks[-1].get_axis(0), joysticks[-1].get_axis(1)
                        try:
                                angle = math.degrees(math.atan(y/x))
                                angle += 180 if x < 0 else 360
                                angle = round(angle % 360)
                        except:
                                print 'Zero division'
                        if x > -0.2 and x < 0.2 and y > -0.2 and y < 0.2:
                                ser.write(b'x')
                        elif angle > 170 and angle < 190:
                                ser.write(b'a')
                        elif angle >= 10 and angle < 350:
                                ser.write(b'd')
                        elif angle > 80 and angle < 100:
                                ser.write(b's')
                        elif angle > 260 and angle < 280:
                                ser.write(b'w')
                        elif angle > 280 and angle < 350:
                                ser.write(b'e')
                        elif angle > 190 and angle < 260:
                                ser.write(b'q')
                        elif angle > 100 and angle < 170:
                                ser.write(b'z')
                        elif angle > 10 and angle < 80:
                                ser.write(b'c')
                elif event.type == pygame.JOYBUTTONDOWN:
                        if event.button == 1:
                                ser.write(b'h')
#                        if event.button == 6:
#                                print 'button BACK'
                ser.flushOutput()
                pygame.event.clear()
