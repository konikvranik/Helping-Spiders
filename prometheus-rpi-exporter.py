from prometheus_client import start_http_server
from prometheus_client.core import GaugeMetricFamily, StateSetMetricFamily, REGISTRY
import logging
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
        metric = StateSetMetricFamily(
            'temperature',
            'RPi temperature',
            labels=["node", "domain"])
        f = open('/sys/class/thermal/thermal_zone0/temp', 'r')
        
        metric.add_metric(
            [socket.gethostname(), "switch"], int(f.read()) * 0.001)
        return metric

if __name__ == "__main__":
    REGISTRY.register(RPiCollector())
    start_http_server(9118)
    while True:
        time.sleep(1)
