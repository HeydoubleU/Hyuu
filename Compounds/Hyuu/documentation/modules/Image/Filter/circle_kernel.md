Generates a circular kernel using either a linear, exponential, or gaussian falloff function.

### `extents`
The extents of the kernel, actually width/height is `2 * extents + 1`.

### `scale`
Scale factor of the falloff function.

### `rotate`
Rotates the kernel weights around the center.

### `gaussian`
If `true`, uses a gaussian falloff function.

### `falloff_f`
Falloff function. As a float, this represents the exponent of the falloff function, default of 1.0 being a linear falloff. This port's type can be changed to FCurve for more precise control over the falloff.

### `normalize`
Normalize weights such that they sum to 1.0. NOTE: this is not shown in diagnostic view as it can make it hard to distinguish values.