# OpenDMI: Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Add YAML schema validation on regression tests

### Changed

- Migrate out-of-band remote access decoder to stream API
- Migrate system configuration options decoder to stream API
- Migrate system reset information decoder to stream API
- Migrate system information decoder to stream API
- Migrate temperature/voltage/current probe decoders to stream API
- Migrate TPM device characteristics decoder to stream API

### Fixed

- Fix address attributes display format #66
- Fix `entry` command exit code
- Fix table area address display format

## [0.2.2] - March 20, 2026

### Added

- Add `entry` command implementation

### Fixed

- Update outdated SMBIOS data indices
- Fix handling of incorrect UTF-8 sequences in YAML export
- Fix missing tag for binary data in YAML export

## [0.2.1] - March 17, 2026

### Added

- Add YAML emitter error handling
- Add type tags output in YAML export
- Add version number to entry point specification
- Store entry point specification in context
- Add context address size detection
- Add binary data output in JSON export
- Add binary data output in YAML export
- Add raw strings output in JSON export
- Add raw strings output in YAML export
- Add Base64 encoding/decoding functions to utils
- Add Lenovo ThinkCentre M720q 10T8 data

### Changed

- Use literal style for binary data in YAML export

### Fixed

- Fix `opendmi` invocation in reindex script
- Fix error counting in reindex script
- Fix build without YAML/JSON/XML libraries
- Fix CHANGELOG format

## [0.2.0] - March 16, 2026

### Added

- All-new command line interface
- Add AMI, Intel and Sun extension skeletons
- Add FreeBSD backend
- Add JSON output support
- Add SMBIOS data reindexing tool
- Add logging to files
- Add structure levels output in YAML dumps
- Add autoloading of vendor-specific extensions
- Add new structure decoders:
  - AMI type 221 (reverse-engineered, partial)
  - Dell revisions and IDs (type 208)
  - Dell parallel port (type 209)
  - Dell serial port (type 210)
  - Dell infrared port (type 211)
- Add specifications for OEM-specific structures:
  - Dell revisions and IDs (type 208)
  - Dell parallel port (type 209)
  - Dell serial port (type 210)
  - Dell infrared port (type 211)
- Complete system slots decoder
- Add memory controller structure linking
- Add system boot status decoding
- Add support for enumeration range names
- Add reference manual skeleton
- Add automated manual pages generation via AsciiDoctor #7
- Add new manual pages:
  - `dmi_memory_array_addr`
  - `dmi_pointing_device`
  - `dmi_cooling_device`
- Add `--prefix` option to `build.sh` #77
- Add `install` command to `build.sh` #78
- Add test coverage build

### Changed

- Migrate to using CMocka for testing
- Rename `dmi_pointing_device_interface_t` to `dmi_pointing_device_iface_t`
- Move `<opendmi/name.h>` to `<opendmi/utils/name.h>`
- Refactor entity decoders API

### Fixed

- Fix build on FreeBSD platform
- Fix firmware inventory components list decoding
- Fix portable battery SBDS manufacture date decoding
- Fix displaying of end-of-table structure
- Fix Centronics connectors naming
- Fix management protocols naming
- Fix zero handle linking behavior
- Fix group associations items list decoding

## [0.1.4] - January 13, 2026

### Fixed

- Fix memory controller module size decoding
- Fix hardware security settings decoding
- Fix power supply structure minimum length
- Fix cooling device structure minimum length
- Fix current probe structure minimum length
- Fix temperature probe structure mimimum length
- Fix voltage probe structure minimum length
- Fix baseboard structure minumum length

## [0.1.3] - January 11, 2026

### Added

- Add automatic string trimming
- Add baseboard structure linking
- Add physical memory array structure linking
- Add power supply structure linking

### Fixed

- Fix invalid memory access in `dmi_entity_string_ex()`
- Fix memory leak in memory channel decoder
- Fix memory leak in context error queue

## [0.1.2] - January 09, 2026

### Added

- Add `--type` option for filtering by type.
- Add tests for `dmi_date_parse()`.

### Changed

- Downgrade CMake to version 3.25 to improve compatibility.

### Fixed

- Fix `dmi_file_read()` behaviour on SysFS.
- Fix `dmi_file_map()` error handling.
- Fix firmware ROM size decoding.
- Fix platform firmware version decoding.
- Fix embedded controller firmware version decoding.
- Fix processor socket type decoding.
- Fix cache extended maximum and installed sizes decoding.
- Fix memory array extended capacity decoding.
- Fix memory array mapping extended addresses decoding.
- Fix memory device mapping extended addresses decoding.
- Fix cooling device description decoding.
- Fix portable battery SBDS version decoding.
- Fix system event log structure version decoding.

## [0.1.1] - January 07, 2026

### Added

- Add colors for unknown and unspecified values
- Add displaying structure types in handle arrays
- Add new manual pages:
  - `dmi_bswap16()`
  - `dmi_bswap32()`
  - `dmi_bswap64()`
- Mark unspecified values for IPMI device interrupt trigger mode and polarity.

### Changed

- Unify date handling via `dmi_date_t` type:
  - Use `dmi_date_t` for firmware release date
  - Use `dmi_date_t` for portable battery manufacture date
  - Remove field `sbds_manufacture_date` from portable battery descriptor

### Fixed

- Fix baseboard features set decoding.
- Fix battery SBDS manufacture date decoding.
- Fix IPMI device non-volatile storage address decoding.
- Fix incorrect decoding of incomplete structures:
  - system information (`dmi_system_t`);
  - baseboard (`dmi_baseboard_t`);
  - processor (`dmi_processor_t`);
  - memory device (`dmi_memory_device_t`);
  - system enclosure or chassis (`dmi_chassis_t`).
- Fix incorrect address display formats:
  - firmware BIOS segment address;
  - memory array stating & ending addresses;
  - memory device stating & ending addresses;
  - memory error address;
  - management device address.
- Fix formatting signed integers of size less than sizeof(int).

## [0.1] - January 02, 2026

- First public release.
- Full support of SMBIOS specification up to version 3.9.
- Basic implementation of `libopendmi` library with backends for `Linux`, `MacOS` and dump files.
- Basic implementation of `opendmi` command line tool with XML and YAML output formats and color output support.
