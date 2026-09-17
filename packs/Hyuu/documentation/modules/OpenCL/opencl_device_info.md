Returns object with information about system devices.

### device_specs
Contains information about the current device

### devices
List of available devices

#### Select Device
You can set the OpenCL device by adding `HYUU_OPENCL_DEVICE_ID=<device_id>` to your `maya.env`. Device 0 is used by default, likely your system's main GPU.

To use your CPU as the device requires installing an OpenCL CPU runtime, such as that provided by Intel.