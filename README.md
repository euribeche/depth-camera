# Timed Capture with RealSense Camera
This program interfaces with a RealSense camera to perform periodic image captures and exposes teh latest capture through a GET request.

This program was developed on a Raspberry Pi 5 Model B running Ubuntu 24.04.2 LTS. It uses FastAPI to handle incoming requests and listens on port 8000. By default, it captures an image every 5 seconds. A watchdog process ensures that only the five most recent captures are retained.

## Repository Structure
The program is organized into four main components:

- `src/rs_timer/rs_timer`
    
    A C++ script responsible for capturing and saving images from the RealSense camera. The capture interval can be adjusted by modifying the `#define SLEEP_S` constant. Lower values will result in faster snapshots.

- `api_service/src`

    Contains two python scripts:
    - `api_handler.py` : Handles incoming GET request and serves the most recent capture.
    - `file_handler.py` : Monitors the working directory for new files and ensure only the five most recent captures are retained. To adjust this number, modify the `file_num` variable.

- `systemd_hooks`

    Includes systemd service files  for `api_handler`, `file_handler`, and `rs_timer`. These files should be copied to `/etc/systemd/system` to enable proper service management.

- `librealsense`
    
    A Git submodule containing the RealSense SDK, required for camera funcionality. Follow the installtion instructions to set up correctly.

## Installation Guide

### Repository initialization
Clone this repository into your working directory, then navigate to the `librealsense` folder and run the following commands:
```sh
git submodule init
git submodule update
```

This will initialize and update the submodule, pulling in the RealSense SDK files. Refer to the next section for installation instructions.

### Installing the RealSense SDK
For full setup details, refer to the [official RealSense SDK guide](https://github.com/IntelRealSense/librealsense/blob/master/doc/installation.md). The instructions below include only the steps relevant to this project.

#### Install dependencies

1. Make Ubuntu up-to-date including the latest stable kernel:
   ```sh
   sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade
   ```
2. Install the core packages required to build _librealsense_ binaries and the affected kernel modules:
   ```sh
   sudo apt-get install libssl-dev libusb-1.0-0-dev libudev-dev pkg-config libgtk-3-dev
   ```
   **Cmake Note:** certain _librealsense_ [CMAKE](https://cmake.org/download/) flags (e.g. CUDA) require version 3.8+ which is currently not made available via apt manager for Ubuntu LTS.
3. Install build tools
   ```sh
   sudo apt-get install git wget cmake build-essential
   ```
4. Prepare Linux Backend and the Dev. Environment \
   Unplug any connected Intel RealSense camera and run:
   ```sh
   sudo apt-get install libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev at
   ```

#### Install librealsense2

1. Run Intel Realsense permissions script from _librealsense2_ root directory:
   ```sh
   ./scripts/setup_udev_rules.sh
   ```
   Notice: You can always remove permissions by running: `./scripts/setup_udev_rules.sh --uninstall`
2. Build and apply patched kernel modules for:
    * Ubuntu 20/22 (focal/jammy) with LTS kernel 5.13, 5.15, 5.19, 6.2, 6.5 \
      `./scripts/patch-realsense-ubuntu-lts-hwe.sh`
    * Ubuntu 18/20 with LTS kernel (< 5.13) \
     `./scripts/patch-realsense-ubuntu-lts.sh`

    **Note:** What the *.sh script perform?
    The script above will download, patch and build realsense-affected kernel modules (drivers). \
    Then it will attempt to insert the patched module instead of the active one. If failed
    the original uvc modules will be restored.

#### Building librealsense2 SDK

  * Navigate to _librealsense2_ root directory and run:
    ```sh
    mkdir build && cd build
    ```
  * Run cmake configure step. 
    ```sh
    cmake ../
    ```
  * Recompile and install _librealsense2_ binaries:
    ```sh
    sudo make uninstall && make clean && make && sudo make install
    ```

### Compiling and Running `rs_timer`
Any changes made to rs_timer.cpp must be compiled before they will take effect. To compile the code, use the following command:
```sh
g++ filename.cpp -o output_file -lrealsense2
```

If your edits involve image processing or rendering, use the command below instead.

**Note**: The camera relies on additional include files located in the `third_party` folder.
```sh
g++ filename.cpp -o output_file -lrealsense2 -lglfw -lGL -lGLU
```

Complete SDK documentation is available at the [official Intel RealSense developer documentation](https://dev.intelrealsense.com/docs/supported-platforms-and-languages).

### FastAPI Documentation
To start the FastAPI application, run the following command:
```sh
uvicorn main:app --host 0.0.0.0 --port 8000
```
This will launch the server on port 8000, making it accessible from any network interface.

To learn more about configuring request handlers and other features, see the [FastAPI documentation](https://fastapi.tiangolo.com/).


### Installing systemd Hooks

To enable automatic startup and management of the rs_timer, api_handler, and file_handler components, systemd service files are provided in the systemd_hooks directory. These files must be copied to your system’s service directory:
```sh
sudo cp systemd_hooks/*.service /etc/systemd/system/
```
Once copied, you can enable and start each service using:
```sh
sudo systemctl enable <service_name>
sudo systemctl start <service_name>
```
**Note**: Be sure to update the WorkingDirectory and ExecStart fields in each service file to match your specific working directory. These must be set as absolute paths.

To explore additional configuration options for systemd service files, see the [systemd.service man page](https://www.freedesktop.org/software/systemd/man/latest/systemd.service.html).
