#!/usr/bin/python3
# -*- coding: utf-8 -*-

from MQTT_module import MQTT_sub

def main():
	
	# créer deux subcribeur pour chaque topic
    mySub1 = MQTT_sub('172.20.10.11', 'baoLE', '12345678', 'home/outdoor')
    mySub2 = MQTT_sub('172.20.10.11', 'baoLE', '12345678', 'home/indoor')
    	
	# établiser la connection au broker MQTT
    print("create 1")

    mySub1.connect_broker()
    mySub1.mqtt_client.loop_start()

    print("create 2")

    mySub2.connect_broker()
    mySub2.mqtt_client.loop_start()
    
    p = True
    while p == True:
        p = True

if __name__ == '__main__':
    print('----------------Start-------------------')
    main()
