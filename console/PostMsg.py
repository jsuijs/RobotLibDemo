# simple script to post message on robotlib msg 
import paho.mqtt.client as mqtt
import os
from time import sleep  


Packet = "GRID 1 2 200 2 3 150 3 4 100 4 5 50 5 6 0"


#create an mqtt client
mypid = os.getpid()
client_uniq = "arduino_pub_"+str(mypid)
mqttc = mqtt.Client(client_uniq)

#connect to broker
mqttc.connect('127.0.0.1', 1883, 60)

mqttc.loop_start() # run mqtt client in separate thread

#read data from serial and publish
mqttc.publish("Robotlib/MsgFromRobot", Packet)       
sleep(0.5) 
