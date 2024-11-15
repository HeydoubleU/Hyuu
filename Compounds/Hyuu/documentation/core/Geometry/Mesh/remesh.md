Iteratively splits/collapses edges to create an evenly triangulated mesh at a specific edge length, similar to maya's remesh operation. Compared to volume remeshing, this works for open meshes and allows for feature preservation.

## Tags

point/edge/face preservation tags are used to indicate what features of the mesh are preserved.

### `point_preserve`
A preserved point will not be deleted during remesh, but some of its edges may be collapsed.

### `edge_preserve`
How an edge is preserved depends on what it's connected to. 

- A lone preserved edge (no connection to other preserved edges) will not be collapsed.

- A chain of preserved edges one connected to another, is a type of feature where the points that make up the chain may only collapse into their neighbor along the chain. This preserves the general shape of the edge but still allows the level of detail to be increased/decreased.

- A vertex where 3 or more preserved edges meet is considered a corner and will not be collapsed.

### `face_preserve`
Edges on a preserved face will not be split or collapsed, with the exception of if `allow_preserved_face_split` is `true`. This allows edges at the boundary of a "preserved face island" to be split. This should remain `false` for predictable results, but can be useful in certain cases.



## Split
The splitting stage of the remesh.

### `max_length`
Edges greater than this length will be split.

### `split_iterations`
Splitting may take several iterations before all edges are below `max_length`. Generally this only takes a few iterations at which point the loop exits. In extreme cases however it may take many (e.g. if `max_length` was 0, edges would need to be split infinitely). This parameter limits the number of iterations to prevent excessive compute time.

## Collapse
The collapsing stage of the remesh.

### `collapse_threshold`
This value is a % of `max_length`, edges shorter than this will be collapsed. In other words this is "min edge length" where:

> ```
min_length = collapse_threshold * max_length
```

### `collapse_iterations`
Same as `split_iterations`, but in regards to collapsing edges.

### `pre_sort_edges`
Sort edges by length, prioritizing collapsing shorter edges before longer ones. This can improves mesh quality.

### `unsafe_collapse`
Enabling helps the collapse stage converge faster by collapsing multiple edges per-point per-iteration. However in testing with more "difficult" meshes, this can sometimes lead to defects in the result.

### `flip_edges`
Flip edges where doing so improves triangle uniformity. This can improve mesh quality.

### `filter_poked_triangles`
A "poked triangle" refers to when 3 triangles are contained inside of a larger triangle, this is generally undesirable. Enabling this filters these triangles out after each collapse iteration, potentially improving mesh quality.

## Post Process

### `flip_iterations`
Tries to improve triangle quality by flipping edges over a given number of iterations.


### `reproject`
Reprojects the remeshed points onto the original mesh.


---

TODO:
- property transfer
- improve triangle flip algo
- edge case with preserved point at center of poked face resulting in laminar faces/holes.