# Changelog

All notable changes to the `L3_Sensors` project will be documented in this file.

## [1.1.0] - 2026-10-07

### Refactored
* Renamed `ultrasonic_distance` header to `UltrasonicRanger.hpp` for unified family driver support.
* Fixed syntax guard directives and updated driver const-correctness.

### Added
* Updated `IRBreakBeamChannel` (`ir_breakbeam.h` / `ir_breakbeam.cpp`) with non-blocking `update()` loop.
* Added configurable software debouncing (`debounceMicros`) and pulse duration tracking.
* Added callback mechanism (`IRTriggerCallback`) for event-driven architectures.

## [1.0.0] - Initial Release
* Initial repository setup for embedded hardware driver abstractions.