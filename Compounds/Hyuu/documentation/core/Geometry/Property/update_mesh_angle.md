Computes the signed angle between face edges. Concave edges result in positive angles, convex edges in negative angles.

### `angle_unit`
The unit the output angle is in. Default is `radians`.

### `point_angle`
Average of edge angles connected to each point. The output is float2 where `x` is the average of concave edges, and `y` is the average of convex edges. This is to preserve sign information. `x - y` gives the true average of all edge angles.