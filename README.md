# PIV-Controller-VimbaMaster: Enhanced Vimba Camera Integration

This project, distinct from the original PIV-controller, is specifically tailored for Vimba Cameras. It offers enhanced coding standards and a more robust architectural design. Upon completion of all coding work, this project will seamlessly integrate with the existing PIV-control project.

**Link to the Former Project (All-in-One):** [PIV-Control Repository](https://github.com/Schuetzen/PIV-control)

## Key Features

* **Exposure Time Setting:** Customize exposure duration for precise imaging.
* **Image Dimensions:** Set height and width parameters.
* **Frame Rate Adjustment:** Options include 1.0, 10.0, 20.0 fps.
* **Config File Reading:** Supports input of floating-point numbers.
* **Calibration Capture Tools:** Console-based tools for efficient calibration.

## Reminds
* error: Segmentation fault

## Git Workflow Guidelines

To ensure a consistent and error-free workflow, please follow these Git commands:

```git
git clone https://github.com/Schuetzen/PIV-controller-VimbaMaster.git
git add .
git commit -m "Data(01012024)-v1-time1s.0"
git push
```

## Setup Documentation for GC1380

### Library Path Configuration

Set the library path using the following bash command:

```bash
export LD_LIBRARY_PATH=/opt/Vimba_6_1/VimbaCPP/DynamicLib/arm_64bit:$LD_LIBRARY_PATH
```

### Connection Setup

For connecting the device, use an IPv4 configuration:

* **IP Address:** 169.254.100.1
* **Subnet Mask:** 255.255.0.0
* **Default Gateway:** Leave this field blank


# Troubleshooting

| Error Code                | Value | Description                                                                                               |
|---------------------------|-------|-----------------------------------------------------------------------------------------------------------|
| VmbErrorSuccess           | 0     | No error                                                                                                  |
| VmbErrorInternalFault     | -1    | Unexpected fault in Vmb or driver                                                                         |
| VmbErrorApiNotStarted     | -2    | VmbStartup was not called before the current command                                                      |
| VmbErrorNotFound          | -3    | The designated instance (camera, feature, etc.) cannot be found                                           |
| VmbErrorBadHandle         | -4    | The given handle is not valid                                                                             |
| VmbErrorDeviceNotOpen     | -5    | Device was not opened for usage                                                                           |
| VmbErrorInvalidAccess     | -6    | Operation is invalid with the current access mode                                                        |
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
