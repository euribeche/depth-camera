import glob
import os
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

path = "../../src/rs_timer/"

class FileCreatedEventHandler(FileSystemEventHandler):
    def on_created(self, event):
        file = glob.glob(os.path.join(path, "rs-save-*.png"))
        file.sort(key=os.path.getmtime)
        for f in file[:-5]:
            os.remove(f)

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
