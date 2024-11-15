Delete points in mesh by collapsing into the closest neighbor.

This was created for internal use by `remesh`. However there are no prerequisites for the input so this can be used generally, with one caveat being `mesh_out` will always be triangulated.