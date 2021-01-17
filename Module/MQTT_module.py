#!/usr/bin/python3
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt
import json
import os
import tkinter as tk
import time
import socket

from Module.DataBase_module import DataBaseAPI


class MQTT_sub:
    def __init__(self, MQTT_addr, MQTT_us, MQTT_pwd, MQTT_t):
        # inialisation
        self.MQTT_ADDRESS = MQTT_addr
        self.MQTT_USER = MQTT_us
        self.MQTT_PASSWORD = MQTT_pwd
        self.MQTT_TOPIC = MQTT_t

        self.dataAPI = DataBaseAPI()
        self.dataAPI.create_table()
        self.dataAPI.export_to_csv()
        
        self.mqtt_client = mqtt.Client()

        self.mqtt_client.username_pw_set(self.MQTT_USER, self.MQTT_PASSWORD)
        self.mqtt_client.on_connect = self.on_connect
        self.mqtt_client.on_message = self.on_message
        self.mqtt_client.connected_flag = False
        self.mqtt_client.bad_connected_flag = False

    def on_connect(self, client, userdata, flags, rc):
        """ The callback for when the client receives a CONNACK response from the server."""
        if rc==0:
            client.connected_flag=True #set flag
            print("connected OK")
            client.subscribe(self.MQTT_TOPIC)
        else:
            print("Bad connection Returned code=",rc)
            client.bad_connection_flag=True


    def on_message(self, client, userdata, msg):
        """The callback for when a PUBLISH message is received from the server."""
        #print(msg.topic + ' ' + str(msg.payload))
        msg_decode = str(msg.payload.decode("utf-8", "ignore"))
        print("data Received", msg_decode)
        print("Converting from Json to Object")
        msg_in = json.loads(msg_decode)  # decode json data
        try:
            self.dataAPI.insert_data(msg_in["device"], msg_in["sensorType"], msg_in["timestamp"],
                                 msg_in["temperature"], msg_in["humidity"], msg_in["mouvement"], msg_in["luminosity"])
            self.dataAPI.update_csv(msg_in["sensorType"])
        except:
            print("Failed to save data")
            client.connected_flag = False

    def on_disconnect(self, client, userdata, rc=0):
        print("DisConnected result code "+str(rc))
        client.loop_stop()

    def connect_broker(self):
        self.mqtt_client.connect(self.MQTT_ADDRESS, 1883)
    
    def reconnect_broker(self):
        self.mqtt_client.loop_stop()
        self.mqtt_client.disconnect()
        self.mqtt_client.connect(self.MQTT_ADDRESS, 1883)

    def get_connected_flag(self):
        return self.mqtt_client.connected_flag

def get_host_addr(): 
    try: 
        host_name = socket.gethostname() 
        host_ip = socket.gethostbyname(host_name) 
        return host_ip
    except: 
        print("Unable to get Hostname and IP") 