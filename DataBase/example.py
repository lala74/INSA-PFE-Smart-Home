#!/usr/bin/python3
# -*- coding: utf-8 -*-

from DataBaseAPI import DataBaseAPI


def main():
    print("Hello")
    dataAPI = DataBaseAPI()
    dataAPI.create_table()
    dataAPI.insert_data(213123, 12, 7.6, 0.5)


if __name__ == '__main__':
    main()
