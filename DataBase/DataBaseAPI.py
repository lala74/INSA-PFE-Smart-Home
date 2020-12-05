#!/usr/bin/python3
# -*- coding: utf-8 -*-

# This library create an abstract to interact with SQL library

from __future__ import print_function
import sqlite3
from sqlite3.dbapi2 import connect

dataFileName = 'data.db'
tableName = 'sensors'
tableElements = '(date timestamp, sensor_id int, temp real, humid real)'


class DataBaseAPI:
    # def __init__(self, imgPath):
    #     self.imgPath = imgPath
    conn = None
    cursor = None

    def __connect(self):
        self.conn = sqlite3.connect(dataFileName)
        self.cursor = self.conn.cursor()

    def __close(self):
        if self.conn != None:
            self.conn.close()

    def create_table(self):
        self.__connect()

        # '''CREATE TABLE IF NOT EXISTS sensors (date timestamp, sensor_id int, temp real, humid real)'''
        cmd = "CREATE TABLE IF NOT EXISTS " + tableName + tableElements
        self.cursor.execute(cmd)
        self.__close()

    def insert_data(self, date, sensorId, temp, humid):
        self.__connect()

        # "INSERT INTO sensors VALUES (123345, 3, 23.7, 0.8)"
        cmd = "INSERT INTO sensors VALUES (" + str(date) + "," + str(
            sensorId) + "," + str(temp)+"," + str(humid)+")"
        self.cursor.execute(cmd)
        self.conn.commit()

        self.__close()

    # def delete_database():
        
