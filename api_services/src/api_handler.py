# uses FastApi - see documentation at https://fastapi.tiangolo.com/

import glob
import os
from fastapi import FastAPI
from fastapi.responses import FileResponse

# returns the latest image assuming folder structure hasn't changed
def get_latest_image():
    file = glob.iglob("../../src/rs_timer/rs-save-*.png")
    return max(file, key=os.path.getmtime, default=None)

app = FastAPI()

# get request to get latest image
@app.get("/", response_class=FileResponse)
async def main():
    image = get_latest_image()
    return FileResponse(image)
