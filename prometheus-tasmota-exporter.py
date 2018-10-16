import json
import time
import urllib2
from prometheus_client import start_http_server
from prometheus_client.core import GaugeMetricFamily, StateSetMetricFamily, REGISTRY
import pyjq

statusQuery = '.StatusSTS | to_entries | map(select(.key | match("^POWER";"i"))) | map({key: .key, value: .value | test("ON") }) | from_entries'
energyQuery = '.StatusSNS | select(.ENERGY) | .ENERGY | to_entries | map(select( .key == "Total" or .key == "Yesterday" or .key == "Today")) | from_entries'


class TasmotaCollector(object):

    def __init__(self, urls):
        self.urls = urls

    def collect(self):
        for u in self.urls:
            tmp = self._collectStatus(u, "http://%s/cm?cmnd=status%%200" % u)
            if tmp:
              yield tmp
            tmp = self._collectEnergy(u, "http://%s/cm?cmnd=status%%200" % u)
            if tmp:
              yield tmp

    def _collectStatus(self, node, url):
        metric = StateSetMetricFamily(
            'switch_state',
            'State of switches',
            labels=["node", "domain"])
        result = pyjq.one(statusQuery, json.load(urllib2.urlopen(url)))
        metric.add_metric(
            [node, "switch"], result)
        return metric

    def _collectEnergy(self, node, url):
        print url, energyQuery
        result = pyjq.first(energyQuery, json.load(urllib2.urlopen(url)))
        print result
        if result:
            metric = GaugeMetricFamily(
                'energy',
                'Energy reported by sensor',
                labels=["node", "domain", "type", "unit", "context"])
            for k, v in result.iteritems():
                print k, v
                metric.add_metric(
                    [node, "sensor", "energy", "kWh", k], v)
            return metric


if __name__ == "__main__":
    REGISTRY.register(TasmotaCollector(
        ["albohes-1.home", "sonoff-pow-1.home"]))
    start_http_server(9118)
    while True:
        time.sleep(1)
