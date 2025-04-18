Utility for generating kernels with specific values.

### `base_extents`
Half width/height of the kernel mask, a center row/column is added to this implicitly. This ensures kernel dimensions are always odd:

```
dimensions = 2 * extents + 1
```

This determines how much of the following matrix is used, so should be values between 0 and 3.

### `base_weights`
Base weights of the kernel. This matrix represents the top-right quadrant of the kernel, where the bottom left value is the center of the kernel. This matrix is then mirrored along both axes to generate the full kernel.


### `negate_x/y_axis`
Also negate weights when mirroring along the x/y axes.


### `resample`
For generating kernels with extents greater than 3 (larger than 7x7), base kernel can resampled to larger sizes, new weights are generated by interpolating the base weights.


### `normalize`
Normalize weights such that they sum to 1.0. In cases with negative weights, such as for sharpening kernels, this can give unexpected results. In such cases `use_abs_value` should be used. This normalizes weights using their absolute values, then restores the original sign.