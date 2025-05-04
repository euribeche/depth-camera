#### How to use the packaged Viewer
Run the command `realsense-viewer`

#### How to compile code
`g++ filename.cpp -o output_file -lrealsense2`

#### How to complile with GUI
`g++ filename.cpp -o output_file -lrealsense2 -lglfw -lGL -lGLU`

#### Start FastAPI File
`uvicorn main:app --host 0.0.0.0 --port 8000`
