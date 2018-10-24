#!/usr/bin/env python
# -*- coding: utf-8 -*-

from prometheus_client import start_http_server
from prometheus_client.core import GaugeMetricFamily, StateSetMetricFamily, REGISTRY
import logging
import time
from systemd.journal import JournaldLogHandler
import socket

logger = logging.getLogger(__name__)
journald_handler = JournaldLogHandler()
journald_handler.setFormatter(logging.Formatter(
    '[%(levelname)s] %(message)s'
))
logger.addHandler(journald_handler)
logger.setLevel(logging.DEBUG)

class RPiCollector(object):

    def collect(self):
        f = open('/sys/class/thermal/thermal_zone0/temp', 'r')

        metric = GaugeMetricFamily('temperature', 'RPi temperature', labels=['node', 'domain','type', 'sensor', 'unit'])
        metric.add_metric([str(socket.gethostname()), 'sensor', 'temperature', 'rpi', u'°C'], int(f.read()) * 0.001)
        yield metric

if __name__ == "__main__":
    REGISTRY.register(RPiCollector())
    start_http_server(9119)
    while True:
        time.sleep(1)
