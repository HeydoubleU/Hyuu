Get strands geo property and promote/demote to both component types.

### `point_data`
If the property's target is "strand_component", the output `point_data` will be the per-strand data promoted to per-point data.

### `strand_data`
If the property's target is "point_component", the output `strand_data` will be the per-point data demoted to per-strand data. `demote_source` determines which point on the strand is used, either to root or tip.