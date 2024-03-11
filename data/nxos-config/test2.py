import requests
import json
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

watched_path = "/var/log/snort/"

switchuser='admin'
switchpassword='admin'
url='https://172.16.138.201/ins'
myheaders={'content-type':'application/json-rpc'}
payload=[
  {
    "jsonrpc": "2.0",
    "method": "cli_ascii",
    "params": {
      "cmd": "interface e1/5-13",
      "version": 1
    },
    "id": 1
  },
  {
    "jsonrpc": "2.0",
    "method": "cli_ascii",
    "params": {
      "cmd": "  switchport",
      "version": 1
    },
    "id": 2
  },
  {
    "jsonrpc": "2.0",
    "method": "cli_ascii",
    "params": {
      "cmd": "  switchport access vlan 60",
      "version": 1
    },
    "id": 3
  },
  {
    "jsonrpc": "2.0",
    "method": "cli_ascii",
    "params": {
      "cmd": "  no shutdown",
      "version": 1
    },
    "id": 4
  },
  {
    "jsonrpc": "2.0",
    "method": "cli_ascii",
    "params": {
      "cmd": "interface e1/2",
      "version": 1
    },
    "id": 5
  },
  {
    "jsonrpc": "2.0",
    "method": "cli_ascii",
    "params": {
      "cmd": "  switchport",
      "version": 1
    },
    "id": 6
  },
  {
    "jsonrpc": "2.0",
    "method": "cli_ascii",
    "params": {
      "cmd": "  switchport access vlan 60",
      "version": 1
    },
    "id": 7
  },
  {
    "jsonrpc": "2.0",
    "method": "cli_ascii",
    "params": {
      "cmd": "  no shutdown",
      "version": 1
    },
    "id": 8
  }
]

def ChangeVlan():
    response = requests.post(url,data=json.dumps(payload), headers=myheaders,auth=(switchuser,switchpassword), verify=False).json()


class MyHandler(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path == watched_path + "alert":
            print(f'event type: {event.event_type}  path : {event.src_path}')
            ChangeVlan()


if __name__ == "__main__":
    event_handler = MyHandler()
    observer = Observer()
    observer.schedule(event_handler, path=watched_path, recursive=False)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()
