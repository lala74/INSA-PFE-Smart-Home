#!/usr/bin/python3
# -*- coding: utf-8 -*-

from Module.MQTT_module import MQTT_sub
from Module.MQTT_module import get_host_addr
import time

def main():

    host_addr = get_host_addr()
    print(host_addr)

    mySub1 = MQTT_sub(host_addr, 'baoLE', '12345678', 'home/outdoor')
    mySub2 = MQTT_sub(host_addr, 'baoLE', '12345678', 'home/indoor')
    
    print("create 1")

    mySub1.connect_broker()
    mySub1.mqtt_client.loop_start()

    print("create 2")

    mySub2.connect_broker()
    mySub2.mqtt_client.loop_start()
    
    time.sleep(1)

    while True:
        time.sleep(1)
        while mySub2.get_connected_flag()== False:
            print("reconnect sub_client 2")
            mySub2.reconnect_broker()
            mySub2.mqtt_client.loop_start()
            time.sleep(0.1)
        while mySub1.get_connected_flag()== False:
            print("reconnect sub_client 1")
            mySub1.reconnect_broker()
            mySub1.mqtt_client.loop_start()
            time.sleep(0.1)


if __name__ == '__main__':
    print('----------------Start-------------------')
    main()
