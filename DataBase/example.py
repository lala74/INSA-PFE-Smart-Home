#!/usr/bin/python3
# -*- coding: utf-8 -*-

from DataBaseAPI import DataBaseAPI


def main():
    dataAPI = DataBaseAPI()
    dataAPI.create_table()
    dataAPI.insert_data(200, 12, 7.6, 0.55)
    dataAPI.insert_data(201, 12, 7.7, 0.65)
    dataAPI.insert_data(202, 12, 7.8, 0.75)
    dataAPI.insert_data(123, 10, 2.6, 0.5)
    dataAPI.insert_data(124, 10, 3.6, 0.6)
    dataAPI.insert_data(125, 10, 4.6, 0.7)
    print("------- All data --------")
    dataAPI.print_database()
    print("------- Data of sensor 12 --------")
    for row in dataAPI.load_data_by_sensor_id(12):
        print(row)
    print("------- Data of sensor 10 --------")
    for row in dataAPI.custom_query('SELECT * FROM sensors WHERE sensor_id = 10'):
        print(row)
    print("------- Export to CSV --------")
    dataAPI.export_to_csv()
    dataAPI.delete_database()


if __name__ == '__main__':
    main()
