# OpenDMI: Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

- Added raw strings output in YAML export

## [0.2.0] - March 16, 2026

- All-new command line interface
- Introduced reference manual
- Introduced AMI extension
- Introduced Intel extension
- Introduced Sun extension
- Implemented FreeBSD backend
- Migrated to using CMocka for testing
- Added test coverage build
- Added JSON output support
- Added SMBIOS data reindexing tool
- Added logging to file
- Added `modules` command implementation
- Added structure levels output to YAML dumps
- Added autoloading of vendor-specific extensions
- Added AMI type 221 structure decoding (partial)
- Added Dell revisions and IDs structure decoding (type 208)
- Added Dell parallel port structure decoding (type 209)
- Added Dell serial port structure decoding (type 210)
- Added Dell infrared port structure decoding (type 211)
- Added specifications for OEM-specific structures:
  - Dell revisions and IDs (type 208)
  - Dell parallel port (type 209)
  - Dell serial port (type 210)
  - Dell infrared port (type 211)
- Added memory controller structure linking
- Added system boot status decoding
- Added support for enumeration range names
- Added automated manual pages generation via AsciiDoctor #7
- Added manual page for `dmi_memory_array_addr`
- Added manual page for `dmi_pointing_device`
- Added manual page for `dmi_cooling_device`
- Added `--prefix` option to `build.sh` #77
- Added `install` command to `build.sh` #78
- Renamed `dmi_pointing_device_interface_t` to `dmi_pointing_device_iface_t`
- Moved `<opendmi/name.h>` to `<opendmi/utils/name.h>`
- Refactored entity decoders API
- Completed system slots decoder
- Fixed build on FreeBSD platform
- Fixed firmware inventory components list decoding
- Fixed portable battery SBDS manufacture date decoding
- Fixed displaying of end-of-table structure
- Fixed Centronics connectors naming
- Fixed management protocols naming
- Fixed zero handle linking behavior
- Fixed group associations items list decoding

## [0.1.4] - January 13, 2026

- Fixed memory controller module size decoding
- Fixed hardware security settings decoding
- Fixed power supply structure minimum length
- Fixed cooling device structure minimum length
- Fixed current probe structure minimum length
- Fixed temperature probe structure mimimum length
- Fixed voltage probe structure minimum length
- Fixed baseboard structure minumum length

## [0.1.3] - January 11, 2026

- Added automatic string trimming
- Added baseboard structure linking
- Added physical memory array structure linking
- Added power supply structure linking
- Fixed invalid memory access in `dmi_entity_string_ex()`
- Fixed memory leak in memory channel decoder
- Fixed memory leak in context error queue

## [0.1.2] - January 09, 2026

- Downgraded CMake to version 3.25 to improve compatibility.
- Added `--type` option for filtering by type.
- Added tests for `dmi_date_parse()`.
- Fixed `dmi_file_read()` behaviour on SysFS.
- Fixed `dmi_file_map()` error handling.
- Fixed firmware ROM size decoding.
- Fixed platform firmware version decoding.
- Fixed embedded controller firmware version decoding.
- Fixed processor socket type decoding.
- Fixed cache extended maximum and installed sizes decoding.
- Fixed memory array extended capacity decoding.
- Fixed memory array mapping extended addresses decoding.
- Fixed memory device mapping extended addresses decoding.
- Fixed cooling device description decoding.
- Fixed portable battery SBDS version decoding.
- Fixed system event log structure version decoding.

## [0.1.1] - January 07, 2026

- Added colors for unknown and unspecified values
- Added displaying structure types in handle arrays
- Unified date handling via `dmi_date_t` type:
  - Firmware strcture now uses `dmi_date_t` for release date
  - Portable battery now used `dmi_date_t` for manufacture date
  - Removed field `sbds_manufacture_date` from portable battery descriptor
- Marked unspecified values for IPMI device interrupt trigger mode and polarity.
- Fixed baseboard features set decoding.
- Fixed battery SBDS manufacture date decoding.
- Fixed IPMI device non-volatile storage address decoding.
- Fixed incorrect decoding of incomplete structures:
  - system information (`dmi_system_t`);
  - baseboard (`dmi_baseboard_t`);
  - processor (`dmi_processor_t`);
  - memory device (`dmi_memory_device_t`);
  - system enclosure or chassis (`dmi_chassis_t`).
- Fixed incorrect address display formats:
  - firmware BIOS segment address;
  - memory array stating & ending addresses;
  - memory device stating & ending addresses;
  - memory error address;
  - management device address.
- Fixed formatting signed integers of size less than sizeof(int).
- Added manpages for `dmi_bswap16()`, `dmi_bswap32()` and `dmi_bswap64()` functions

## [0.1] - January 02, 2026

- First public release.
- Full support of SMBIOS specification up to version 3.9.
- Basic implementation of `libopendmi` library with backends for `Linux`, `MacOS` and dump files.
- Basic implementation of `opendmi` command line tool with XML and YAML output formats and color output support.
