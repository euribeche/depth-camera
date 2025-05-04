import glob
import os
from fastapi import FastAPI
from fastapi.responses import FileResponse

def get_latest_image():
    file = glob.iglob("../../src/rs_timer/rs-save-*.png")
    return max(file, key=os.path.getmtime, default=None)

app = FastAPI()

@app.get("/", response_class=FileResponse)
async def main():
    image = get_latest_image()
    return FileResponse(image)
