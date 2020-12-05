#!/usr/bin/python3
# -*- coding: utf-8 -*-

from DataStoreAPI import DataStoreAPI


def main():
    print("Hello")
    dataAPI = DataStoreAPI()
    dataAPI.create_table()
    dataAPI.insert_data(213123, 12, 7.6, 0.5)


if __name__ == '__main__':
    main()
