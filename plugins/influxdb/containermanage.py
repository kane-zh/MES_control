# This Python file uses the following encoding: utf-8
from PyQt5 import QtCore
from PyQt5 import QtWidgets
import influxdb_client
from influxdb_client import InfluxDBClient

client = InfluxDBClient(url="http://192.168.0.100:9999", token="token")
class containermanage(QObject):
    def __init__(self):
