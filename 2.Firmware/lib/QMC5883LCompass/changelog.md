# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [v1.2.3]
### Fixed
- Issue #27. Library version number was not updated.
 
## [v1.2.2] - 2023-09-07 [YANKED]
### Fixed
- Issue #25. A previous change to the calibrate() set values out of range from int variable type. Changed to long.
- Typo in author credit of QMC5883LCompass.cpp.

## [v1.2.1] - 2023-09-07 [YANKED]
### Fixed
- Bug in getBearing function where negative values would cause incorrect bearings.

## [v1.2.0]
### Changed
- Core Calibration has been modified and been moved to the class. Example sketch has been updated as well. (Contrib by paulosincos)
- Calibration var calibrationData has been seeded with default values. (Contrib by ATsaruk)
- Updates to documentation.

## [v1.1.1]
### Changed
- Fixed version number in library.properties

## [v1.1.0]
### Added
- Added calibration functions to library
- Added /examples/calibration/calibration.ino utility sketch.
### Changed
- Modified readme.md to reflect new calibration functions.

## [v1.0.3]
### Changed
- Modified readme.md to correct type in getAzimuth() function.

## [v1.0.2]
### Changed
- Modified readme.md to make it clearer that wire.h is required to run.

## [v1.0.1]
### Changed
- Modified the way getBearing() and getDirection() worked. Both functions now require that the azimuth be passed to them as a parameter.
- Updated readme.md to reflect the above change.
- Updated /examples/bearing/bearing.ino to reflect above change.
- Updated /examples/direction/direction.ino to reflect above change.

### Fixed
- Corrections made to readme.md
- Renamed smoothing() to _smoothing() since this is a private function.
- Renamed writeReg() to _writeReg() since this is a private function.

## [v1.0.0]
### Added
- Documentation to readme.md
- Initial commit.

## [v0.0.2]
### Added
- Added getBearing() and getDirection() functions.

## [v0.0.1]
### Added
- Initial creation.
