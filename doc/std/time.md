# Standard Library: Time

The `Time` module provides utilities for handling time, dates, and thread execution.
It is built on top of the native `__native_time_` primitives.

To use it: include `std/time.gw` (or ensure it's loaded).

## Class `Time`

Main entry point for time operations.

### `Time.now() -> Integer`
Returns the current Unix timestamp (seconds since Jan 01 1970 UTC).
*   **Returns**: Integer timestamp.

### `Time.nowd() -> Number`
Returns the current Unix timestamp as a double (with microsecond precision).
*   **Returns**: Number timestamp.

### `Time.sleep(seconds: Integer)`
Pauses the program execution for a given number of seconds.

### `Time.sleepd(seconds: Number)`
Pauses the program execution for a given number of seconds (supports decimals, e.g. `0.5` for 500ms).

### `Time.ctime() -> String`
Returns the current date and time as a formatted string.
*   **Example**: `"Thursday February 19 22:45:00 2026"`

---

## Class `DateUtils`

Helper functions for date manipulation and formatting.

### `DateUtils.formatDate(ts: Integer) -> String`
Converts a Unix timestamp into a readable date string.
*   **ts**: The timestamp to format.

### `DateUtils.getTimeStampFromDate(year: Integer, month: Integer, day: Integer) -> Integer`
Calculates the Unix timestamp for a specific date (at 00:00:00 UTC).
*   **year**: Year (e.g., 2026)
*   **month**: Month (1-12)
*   **day**: Day (1-31)
*   **Returns**: Timestamp in seconds.

### `DateUtils.isLeap(year: Integer) -> Boolean`
Checks if a year is a leap year.

### `DateUtils.getDaysInMonth(month: Integer, year: Integer) -> Integer`
Returns the number of days in a specific month (accounting for leap years).

### `DateUtils.getDayNameFromID(id: Integer) -> String`
Returns the name of the day (1=Monday, ..., 7=Sunday).

### `DateUtils.getFullMonthNameFromID(id: Integer) -> String`
Returns the full name of the month (1=January, ..., 12=December).

---

## Example Usage

```kotlin
# Get current time
var now = Time.now();
println("Timestamp: " + now);
println("Date: " + Time.ctime());

# Measure execution time
var start = Time.nowd();
Time.sleepd(0.5); # Sleep 500ms
var end = Time.nowd();
println("Elapsed: " + (end - start) + "s");

# Calculate specific date
var birthDate = DateUtils.getTimeStampFromDate(2000, 1, 1);
println("Y2K Timestamp: " + birthDate);
println("Formatted: " + DateUtils.formatDate(birthDate));
```
