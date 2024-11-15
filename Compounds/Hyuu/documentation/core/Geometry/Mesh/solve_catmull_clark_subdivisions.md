```
build_catmull_clark_subdivisions --> solve_catmull_clark_subdivisions
```

This pair of compounds split procedure of the subdividing a mesh into 2 steps, as opposed to the standard `catmull_clark_subdivide_mesh` which does the whole thing in one step.

**Step 1.** build the base connectivity of each subdivition level:
> `build_catmull_clark_subdivisions`


**Step 2.** solve for the actual vertex positions:

> `solve_catmull_clark_subdivisions`


If we're subdividing a deforming mesh (i.e. face/point count are constant), we can cache the result of `build_catmull_clark_subdivisions`, as this only needs to be computed once. Then update positions as needing with `solve_catmull_clark_subdivisions`

NOTE: Spliting the procedure into 2 compounds does add some overhead. If the subdivisions aren't being cached, `catmull_clark_subdivide_mesh` should be used instead.