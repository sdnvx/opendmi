# OpenDMI: Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

- Added automatic string trimming
- Added baseboard structure linking
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
