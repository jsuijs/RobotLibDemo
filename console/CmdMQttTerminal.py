
import paho.mqtt.client as mqtt
import os
import sys

# for Windows-based systems
import msvcrt # If successful, we are on Windows

broker = "127.0.0.1"
port = 1883


def on_connect(client, userdata, flags, rc):
    print("Connection returned result: ", rc)
    if rc == 0:
    #rc 0 successful connect
        print("Connected")
    else:
        raise Exception

def on_publish(client, userdata, mid):  
    if userdata is None:
        userdata = "^"
    print("Published " + userdata, mid)        


# subscribe part
def on_message(client, userdata, message):
#    print("Received message '" + str(message.payload) + "' on topic '"
#        + message.topic + "' with QoS " + str(message.qos))       
    print(message.payload.decode("cp1252", 'ignore'), end='')    
    sys.stdout.flush() 
        
#    ser.write(message.payload)
#    ser.write(b"\r")
#    ser.flush()


#try:  
def main():
    #create an mqtt client
    mypid = os.getpid()
    client_uniq = "arduino_pub_"+str(mypid)
    mqttc = mqtt.Client(client_uniq)
    mqttc.on_connect = on_connect
#    mqttc.on_publish = on_publish

    #connect to broker
    mqttc.connect(broker, port, 60)

     # subscribe
    mqttc.on_message = on_message
    mqttc.subscribe("Robotlib/ComRawRx", qos=0)

#    while 1:        
    while mqttc.loop(timeout=0.05) == 0:        
        
        line = ''  
        if msvcrt.kbhit():
            while msvcrt.kbhit(): 
                line += msvcrt.getch().decode("cp1252", 'ignore')           
#            ser.write(ch)       
            mqttc.publish("Robotlib/ComRawTx", line)
            #print("Publish");
        pass
    printf("einde")
    
main()
    
#except:
#    print("Failed...")
#    #unable to continue with no serial input
#    raise SystemExit


            
    