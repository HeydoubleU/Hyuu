Overloaded function for remapping scalars, typically between 0 and 1. `function` can either be a FCurve, or float (applied as exponent).

This is intended for remapping values, where typically an FCurve would be ideal. However in many cases the desired curve can be approximated with a simple power function (i.e. the float overload), which can be more convenient to adjust.