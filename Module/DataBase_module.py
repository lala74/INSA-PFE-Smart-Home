#!/usr/bin/python3
# -*- coding: utf-8 -*-

# This library create an abstract to interact with SQL library

from __future__ import print_function
import sqlite3
from sqlite3.dbapi2 import connect
import csv
import os

dataFileName = './Data/data.db'
tableName = 'home_sensor'

device_name = 'device'
sensor_id_name = 'sensor_id'
date_name = 'timestamp'
temp_name = 'temperature'
humid_name = 'humidity'
mouv_name = 'mouvement'
lum_name = 'luminosity'

tableElements = '('+device_name+' VARCHAR(100), '+sensor_id_name +' VARCHAR(100), '+date_name+' timestamp, '\
    +temp_name+' real, '+humid_name+' real, '+mouv_name+' real, '+lum_name+' real)'


class DataBaseAPI:
    # def __init__(self, imgPath):
    #     self.imgPath = imgPath
    conn = None
    cursor = None

    def create_table(self):
        self.__connect()
        # CREATE TABLE IF NOT EXISTS home_sensor (device VARCHAR(100), sensor_id VARCHAR(100), timestamp timestamp, sensor_id int, temp real, humid real, mouv real, lum real)
        cmd = 'CREATE TABLE IF NOT EXISTS ' + tableName + tableElements
        self.__execute_cmd(cmd)

    def insert_data(self, device, sensorID, timestamp, temp, humid, mouv, lum):
        self.__connect()
        # INSERT INTO home_sensor VALUES ('ESP32', 'Outdoor', '10:23:13', 3, 23.7, 0.8, 0.7)
        cmd = 'INSERT INTO ' + tableName + \
            ' VALUES (' + "'" +str(device) + "'" + ',' + "'" + str(sensorID) + "'" + ',' + "'" + str(timestamp) + "'" + \
            ',' + str(temp)+ ',' + str(humid)+ ',' + str(mouv) + ','+ str(lum) + ')'
        print(cmd)
        self.__execute_cmd(cmd)

    def load_data_by_sensor_id(self, sensor_id):
        self.__connect()
        # SELECT * FROM home_sensor WHERE sensor_id = 12
        cmd = 'SELECT * FROM ' + tableName + \
            ' WHERE ' + sensor_id_name + '=' + "'" +str(sensor_id) + "'"
        value = self.cursor.execute(cmd)
        return value

    def print_database(self):
        self.__connect()
        for row in self.load_all_data():
            print(row)

    def load_all_data(self):
        self.__connect()
        # SELECT * FROM home_sensor ORDER BY timestamp
        cmd = 'SELECT * FROM ' + tableName + ' ORDER BY ' + date_name
        value = self.cursor.execute(cmd)
        return value

    def load_last_data(self, sensor_id):
        self.__connect()
        cmd = 'SELECT * FROM ' + tableName + ' WHERE ' + sensor_id_name + '=' + "'" +str(sensor_id) + "'" + ' ORDER BY ' + date_name + ' DESC LIMIT 1'
        value = self.cursor.execute(cmd)
        return value

    def export_to_csv(self):
        self.__connect()
        self.load_all_data()
        c = self.cursor
        with open("./Data/data.csv", "w") as csv_file:
            csv_writer = csv.writer(csv_file, delimiter="\t")
            csv_writer.writerow([i[0] for i in c.description])
            csv_writer.writerows(c)
        self.__close()
    
    def create_csv(self):
        self.__connect()
        self.load_all_data()
        c = self.cursor
        with open("./Data/data.csv", "a") as csv_file:
            csv_writer = csv.writer(csv_file, delimiter="\t")
            csv_writer.writerow([i[0] for i in c.description])
        self.__close()

    def update_csv(self,sensor_id):
        self.__connect()
        with open("./Data/data.csv", "a") as csv_file:
            csv_writer = csv.writer(csv_file, delimiter="\t")
            csv_writer.writerows(self.load_last_data(sensor_id))
        self.__close()

    def delete_database(self):
        self.__connect()
        # DELETE * FROM home_sensor
        cmd = 'DELETE FROM ' + tableName
        self.__execute_cmd(cmd)


    def custom_query(self, cmd):
        self.__connect()
        value = self.cursor.execute(cmd)
        return value

    def __connect(self):
        self.conn = sqlite3.connect(dataFileName)
        self.cursor = self.conn.cursor()

    def __close(self):
        if self.conn != None:
            self.conn.close()

    def __execute_cmd(self, cmd):
        self.cursor.execute(cmd)
        self.conn.commit()
        self.__close()
