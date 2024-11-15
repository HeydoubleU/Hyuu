General purpose circular kernel mask generator.

### `extents`
Half width/height of the kernel mask, a center row/column is added to this implicitly. This ensures kernel dimensions are always odd:

```
dimensions = 2 * extents + 1
```

### `scale`
Scale x/y of the generated pattern.

### `rotate`
Rotation of the pattern in degrees.

## Function

### `gaussian`
Generate from gaussian falloff function.

### `falloff`
If not using gaussian, this controls the falloff of the pattern. As a float this is used as an exponent, e.g. `1.0` is linear falloff, `2.0` is quadratic, etc. Alternatively, this port's type can be changed to `FCurve` for precise falloff control.


### `normalize`
Normalize mask weights, i.e. weights sum to 1.0.
> NOTE: This does not show in diagnostic view as it often makes the mask dark and hard to discern.