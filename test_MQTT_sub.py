#!/usr/bin/python3
# -*- coding: utf-8 -*-

from Module.MQTT_module import MQTT_sub
import time

def main():

    mySub1 = MQTT_sub('192.168.43.38', 'baoLE', '12345678', 'home/outdoor')
    mySub2 = MQTT_sub('192.168.43.38', 'baoLE', '12345678', 'home/indoor')
    
    print("create 1")

    mySub1.connect_broker()
    mySub1.mqtt_client.loop_start()

    print("create 2")

    mySub2.connect_broker()
    print(" iss connected")
    mySub2.mqtt_client.loop_start()
    
    time.sleep(1)
    print(str(mySub2.get_connected_flag())+"1234")

    while True:
        time.sleep(1)
        while mySub2.get_connected_flag()== False:
            print("reconnect sub_client 2")
            mySub2.reconnect_broker()
            mySub2.mqtt_client.loop_start()
            time.sleep(1)


if __name__ == '__main__':
    print('----------------Start-------------------')
    main()
