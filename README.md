# PIV-Controller-VimbaMaster: Enhanced Vimba Camera Integration

This project, distinct from the original PIV-controller, is specifically tailored for Vimba Cameras. It offers enhanced coding standards and a more robust architectural design. Upon completion of all coding work, this project will seamlessly integrate with the existing PIV-control project.

**Link to the Former Project (All-in-One):** [PIV-Control Repository](https://github.com/Schuetzen/PIV-control)

## Key Features

* **Exposure Time Setting:** Customize exposure duration for precise imaging.
* **Image Dimensions:** Set height and width parameters.
* **Frame Rate Adjustment:** Options include 1.0, 10.0, 20.0 fps.
* **Config File Reading:** Supports input of floating-point numbers.
* **Calibration Capture Tools:** Console-based tools for efficient calibration.

## Git Workflow Guidelines

To ensure a consistent and error-free workflow, please follow these Git commands:

```git
git clone https://github.com/Schuetzen/PIV-controller-VimbaMaster.git
git add .
git commit -m "Your descriptive commit message"
git push

```

### Vimba SDK Setup (Vimba v6.1)

Download the Vimba SDK from the following link: [Vimba SDK Download](https://www.alliedvision.com/en/products/vimba-sdk/)

Extract the SDK with:

```bash
sudo tar -xvzf Vimba_v6.1_ARM64.tgz -C /opt
```

Update the GigE interface:

```bash
sudo bash /opt/Vimba_6_0/VimbaGigETL/Install.sh
```

### Library Path Configuration

Update and install necessary libraries:

```bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install libtbbmalloc2 libjpeg-dev libtiff5-dev libpng-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libgtk-3-dev libatlas-base-dev gfortran python3-pip python3-xyz cmake build-essential pkg-config
```

Install OpenCV:

```bash
sudo apt-get install libopencv-dev
```

Install GPIO control libraries:

```bash
sudo apt-get install lgpio libgpiod-dev liblgpio-dev
```

Configure the library path:

```bash
export LD_LIBRARY_PATH=/opt/Vimba_6_1/VimbaCPP/DynamicLib/arm_64bit:$LD_LIBRARY_PATH
```


### Connection Setup

For connecting the device, use an IPv4 configuration:

* **IP Address:** 169.254.100.1
* **Subnet Mask:** 255.255.0.0
* **Default Gateway:** Leave this field blank


# Update Information
| Version                | Time | Comment                                                                                              |
|---------------------------|-------|-----------------------------------------------------------------------------------------------------------|
|v1.0.0|Jan 11 2024|  Initial comment, fix out the fixed rate|
|v1.0.1|Jan 17 2024| Update the Multiframe capture method|
|v1.1.1-beta|Jan 25 2024|Successfully measured the PIV image|
|v1.1.2|Jan 27 2024| Update Mono12|
|v1.1.3|Jan 30 2024| Clean folder|
|v1.2.0|Feb 8 2024| Reorganized Readme, Version control; Pack Mono12 Viewer; Prepare function for shadowimage|
|v1.2.1|Feb 12 2024| Update Vimba SDK 6.0 inside; Update Setup; Remove `sample` folder|


# Troubleshooting

| Error Code                | Value | Description                                                                                               |
|---------------------------|-------|-----------------------------------------------------------------------------------------------------------|
| VmbErrorSuccess           | 0     | No error                                                                                                  |
| VmbErrorInternalFault     | -1    | Unexpected fault in Vmb or driver                                                                         |
| VmbErrorApiNotStarted     | -2    | VmbStartup was not called before the current command                                                      |
| VmbErrorNotFound          | -3    | The designated instance (camera, feature, etc.) cannot be found                                           |
| VmbErrorBadHandle         | -4    | The given handle is not valid                                                                             |
| VmbErrorDeviceNotOpen     | -5    | Device was not opened for usage                                                                           |
| VmbErrorInvalidAccess     | -6    | Operation is invalid with the current access mode                                                         |
| VmbErrorBadParameter      | -7    | One of the parameters is invalid (usually an illegal pointer)                                             |
| VmbErrorStructSize        | -8    | The given struct size is not valid for this version of the API                                            |
| VmbErrorMoreData          | -9    | More data available in a string/list than space is provided                                               |
| VmbErrorWrongType         | -10   | Wrong feature type for this access function                                                               |
| VmbErrorInvalidValue      | -11   | The value is not valid; either out of bounds or not an increment of the minimum                           |
| VmbErrorTimeout           | -12   | Timeout during wait                                                                                       |
| VmbErrorOther             | -13   | Other error                                                                                               |
| VmbErrorResources         | -14   | Resources not available (e.g., memory)                                                                    |
| VmbErrorInvalidCall       | -15   | Call is invalid in the current context (e.g. callback)                                                    |
| VmbErrorNoTL              | -16   | No transport layers are found                                                                             |
| VmbErrorNotImplemented    | -17   | API feature is not implemented                                                                            |
| VmbErrorNotSupported      | -18   | API feature is not supported                                                                              |
| VmbErrorIncomplete        | -19   | The current operation was not completed (e.g. a multiple registers read or write)                         |
| VmbErrorIO                | -20   | There was an error during read or write with devices (camera or disk)                                     |
