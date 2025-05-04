import glob
import os
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

path = "../../src/rs_timer/"

class FileCreatedEventHandler(FileSystemEventHandler):
    def on_created(self, event):
        file = glob.iglob("../../src/rs_timer/rs-save-*.png")
        file.sort(key=os.path.getmtime, default=None)
        for f in file[:-5]:
            os.remove(file)

event_handler = FileCreatedEventHandler()
observer = Observer()
observer.schedule(event_handler, path)
observer.start()
try:
    while observer.is_alive():
        observer.join(1)
finally:
    observer.stop()
    observer.join()
