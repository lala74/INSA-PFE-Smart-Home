#!/usr/bin/python3
# -*- coding: utf-8 -*-

# This library create an abstract to interact with SQL library

from __future__ import print_function
import sqlite3
from sqlite3.dbapi2 import connect

dataFileName = 'data.db'
tableName = 'sensors'

date_name = 'date'
sensor_id_name = 'sensor_id'
temp_name = 'temperature'
humid_name = 'humidiy'

tableElements = '('+date_name+' timestamp, '+sensor_id_name + \
    ' int, '+temp_name+' real, '+humid_name+' real)'


class DataBaseAPI:
    # def __init__(self, imgPath):
    #     self.imgPath = imgPath
    conn = None
    cursor = None

    def create_table(self):
        self.__connect()
        # '''CREATE TABLE IF NOT EXISTS sensors (date timestamp, sensor_id int, temp real, humid real)'''
        cmd = 'CREATE TABLE IF NOT EXISTS ' + tableName + tableElements
        self.__execute_cmd(cmd)

    def insert_data(self, date, sensorId, temp, humid):
        self.__connect()
        # 'INSERT INTO sensors VALUES (123345, 3, 23.7, 0.8)'
        cmd = 'INSERT INTO ' + tableName + \
            ' VALUES (' + str(date) + ',' + str(sensorId) + \
            ',' + str(temp)+',' + str(humid)+')'
        self.__execute_cmd(cmd)

    def load_data_by_sensor_id(self, sensor_id):
        self.__connect()
        # SELECT * FROM sensors WHERE sensor_id = 12
        cmd = 'SELECT * FROM ' + tableName + \
            ' WHERE ' + sensor_id_name + '=' + str(sensor_id)
        value = self.cursor.execute(cmd)
        return value

    def print_database(self):
        self.__connect()
        for row in self.load_all_data():
            print(row)

    def load_all_data(self):
        self.__connect()
        # SELECT * FROM sensors
        cmd = 'SELECT * FROM ' + tableName + ' ORDER BY ' + date_name
        value = self.cursor.execute(cmd)
        return value

    def delete_database(self):
        self.__connect()
        # DELETE * FROM sensors
        cmd = 'DELETE FROM ' + tableName
        self.__execute_cmd(cmd)

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
