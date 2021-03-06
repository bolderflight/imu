# Changelog

## v3.1.1
- Updated CMake compile options

## v3.1.0
- Added default values for accel scale factor and rotation matrix (identity)

## v3.0.0
- Removed mag, since it now has its own interface, and removed device config items, since they are now in the Sensors interface

## v2.2.0
- Moved from Eigen and Enum to plain old data types to ease integration with Simulink autocode.

## v2.1.0
- Added *die_temp_c* data member
- Re-ordered structs to optimize packing

## v2.0.3
- Updated to eigen v2.0.0

## v2.0.2
- Updated the frame rate enum so the value is the rate in Hz

## v2.0.1
- Switched to using a variant instead of optional for selecting the bus interface

## v2.0.0
- Switched to using concepts to define interface

## v1.0.1
- Updated README

## v1.0.0
- Initial baseline
