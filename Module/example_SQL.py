#!/usr/bin/python3
# -*- coding: utf-8 -*-

from DataBase_module import DataBaseAPI


def main():
    dataAPI = DataBaseAPI()
    dataAPI.create_table()

    #dataAPI.custom_query('INSERT INTO home_sensor VALUES ("ESP32", "Outdoor", "10:23:13", 3, 23.7, 0.8, 0.7)')


    dataAPI.insert_data("ESP32","inDoor","21-01-02 20:13:30", 12, 7.6, 0.55,1)
    dataAPI.insert_data("ESP32","inDoor","21-01-02 20:14:30", 12, 7.6, 0.55,1)
    dataAPI.insert_data("ESP32","outDoor","21-01-02 20:10:30", 12, 7.6, 0.55,1)
    dataAPI.insert_data("ESP32","outDoor","21-01-02 20:11:30", 12, 7.6, 0.55,1)
    dataAPI.insert_data("ESP32","outDoor","21-01-02 20:12:30", 12, 7.6, 0.55,1)
    dataAPI.insert_data("ESP32","inDoor","21-01-02 20:15:30", 12, 7.6, 0.55,1)
    print("------- All data --------")
    dataAPI.print_database()
    print("------- Data of sensor 12 --------")
    for row in dataAPI.load_data_by_sensor_id("outDoor"):
        print(row)
    print("------- Data of sensor 10 --------")
    for row in dataAPI.custom_query('SELECT * FROM home_sensor WHERE sensor_id = "inDoor"'):
        print(row)
    print("------- Export to CSV --------")
    dataAPI.export_to_csv()
    dataAPI.insert_data("ESP32","inDoor","21-01-02 20:16:30", 12, 7.6, 0.55,1)
    dataAPI.update_csv("outDoor")
    dataAPI.delete_database()


if __name__ == '__main__':
    main()
