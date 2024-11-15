Cut mesh along the given plane.

### `properties`
Properties to transfer to the new mesh, currently only point properties are supported.

### `tag_components`
create `point_cut` and `face_cut` tag properties to identify new components created by the cut.

### `cross_section`
Enable/disable cross section faces. (the `mesh_cross_section` output will always contain the cross section faces)

### `cross_section_remesh`
Cutting can result in very large ngons:

- **None** <br> Skip remesh.

- **Triangulate** <br> Triangulate cross section.

- **Auto** <br> Split faces as needed to prevent face edges from exiting the face's perimeter.