#!/usr/bin/python3.8
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt
import json
from DataBase.DataBaseAPI import DataBaseAPI
import os
import tkinter as tk


class App(tk.Frame):
    def __init__(self, master):
        super().__init__(master)
        self.pack()

        self.entrythingy = tk.Entry()
        self.entrythingy.pack()

        # Create the application variable.
        self.contents = tk.StringVar()
        # Set it to some value.
        self.contents.set("this is a variable")
        # Tell the entry widget to watch this variable.
        self.entrythingy["textvariable"] = self.contents

        # Define a callback for when the user hits return.
        # It prints the current value of the variable.
        self.entrythingy.bind('<Key-Return>',
                             self.print_contents)

    def print_contents(self, event):
        print("Hi. The current entry content is:",
              self.contents.get())


MQTT_ADDRESS = '192.168.43.38'
MQTT_USER = 'baoLE'
MQTT_PASSWORD = '12345678'
MQTT_TOPIC = 'home/#'

dataAPI = DataBaseAPI()
dataAPI.delete_database()
dataAPI.create_table()
dataAPI.export_to_csv()

def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)


def on_message(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    #print(msg.topic + ' ' + str(msg.payload))
    msg_decode=str(msg.payload.decode("utf-8","ignore"))
    print("data Received",msg_decode)
    print("Converting from Json to Object")
    msg_in=json.loads(msg_decode) #decode json data
    dataAPI.insert_data(msg_in["device"],msg_in["sensorType"],msg_in["timestamp"], msg_in["temperature"], msg_in["humidity"],msg_in["mouvement"],msg_in["luminosity"])
    dataAPI.update_csv()

def on_disconnect(client, userdata,rc=0):
    print("DisConnected result code "+str(rc))
    client.loop_stop()

def main():
    

    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_start()

    root = tk.Tk()
    myapp = App(root)
    myapp.mainloop()

if __name__ == '__main__':
    print('----------------Start-------------------')
    main()