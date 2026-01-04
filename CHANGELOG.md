# OpenDMI: Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2] - Unreleased

- Added displaying structure types in handle arrays
- Unified date handling via `dmi_date_t` type:
  - Firmware strcture now uses `dmi_date_t` for release date
  - Portable battery now used `dmi_date_t` for manufacture date
  - Removed field `sbds_manufacture_date` from portable battery descriptor
- Added manpages for `dmi_bswap16()`, `dmi_bswap32()` and `dmi_bswap64()` functions

## [0.1.1] - Unreleased

- Fixed incorrect decoding of incomplete processor structures
- Fixed incorrect decoding of incomplete memory device structures
- Fixed baseboard features set decoding
- Fixed incorrect decoding of incomplete chassis structures
- Fixed formatting signed integers of size less than sizeof(int)
- Fixed incorrect firmware BIOS segment address display format
- Fixed incorrect memory array stating & ending address display format
- Fixed incorrect memory device stating & ending address display format
- Fixed incorrect memory error addresses display format
- Fixed incorrect management device address display format

## [0.1] - January 02, 2026

- First public release.
- Full support of SMBIOS specification up to version 3.9.
- Basic implementation of `libopendmi` library with backends for `Linux`, `MacOS` and dump files.
- Basic implementation of `opendmi` command line tool with XML and YAML output formats and color output support.
