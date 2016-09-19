# WTF-8 conversion tools

A set of naive tools to convert between broken UTF-16 and WTF-8.
See https://en.wikipedia.org/wiki/UTF-8#WTF-8

The only purpose of these tools is to convert to and from broken UTF-16 (that
is, with unpaired surrogates), which Windows seem to happily generate.

Basically, all it does is happily read or write unpaired surrogate halves.

## (Broken) UTF-16 to WTF-8

`wtf162wtf8` reads UTF-16 code units, and tries to read code points.  If that
succeeds, write the read code point as UTF-8.  If it doesn't succeed, i.e. if
it is a high or low surrogate without its other half, write the surrogate half
as UTF-8 (which makes it WTF-8).

The result is WTF-8, and even UTF-8 if the input is valid UTF-16.

## WTF-8 to (broken) UTF-16

`wtf82utf16` does the revers conversion: given WTF-8 input, it reconstructs
the possibly broken UTF-16 data.  All it does is actually write every code
points below `0x10000` as plain UTF-16 units, even surrogate halves.

## Regarding Endianess

These tools are naive, and don't actually do anything about endianess.  The
result is that if they are run on a Big Endian machine, they read and write
UTF-16BE, and if they are run on a Little Endian machine (fairly more common),
they read and write UTF-16LE.

As those tools are typically useful with UTF-16LE, and most machines are
Little Endian, it should generally work fine.  Hopefully.

## Usage

To convert from (broken) UTF-16 to WTF-8, use `wtf162wtf8 < input > output`.
Similarly, to convert from WTF-8 to (broken) UTF-16, use
`wtf82utf16 < input > output`.

You can control the verbosity through the `VERBOSE` environment variable: set
it to a positive integer to get verbose/debugging output on `stderr`.
