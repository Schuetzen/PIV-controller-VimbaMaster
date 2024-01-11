# PIV-controller-VimbaMaster
Different from PIV-controller, this project designed for Vimba Camera, with better coding, better architecture.

## Setting Documents - GC1380

Path
```bash
export LD_LIBRARY_PATH=/opt/Vimba_6_1/VimbaCPP/DynamicLib/arm_64bit:$LD_LIBRARY_PATH
```

### Connecting
Use Ipv4 connection

* IP Address: 169.254.100.1
* Subnet mask: 255.255.0.0 
* Default gateway: blank


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
