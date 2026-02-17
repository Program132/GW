# Native Module: Time

This module provides native bindings for time-related operations.

## Exposed Primitive Functions

The following functions are available in the global scope when the interpreter is initialized with `registerNatives`.

### `__native_time() -> Integer`

Returns the current Unix timestamp (seconds since January 1, 1970 UTC).

### `__native_sleep(seconds: Integer)`

Pauses the execution of the current thread for the specified number of seconds.

## Standard Library Implementation

The following pure GW code demonstrates how to implement a full `Time` and `DateUtils` library using these primitives. this implementation includes a full UTC `ctime` formatter.

```kotlin
class DateUtils {
    static func isLeapYear(year: Int) -> Boolean {
        if (year % 400 == 0) { return true; }
        if (year % 100 == 0) { return false; }
        if (year % 4 == 0) { return true; }
        return false;
    }

    static func getDaysInMonth(month: Int, year: Int) -> Int {
        if (month == 2) {
            if (DateUtils.isLeapYear(year)) { return 29; }
            return 28;
        }
        if (month == 4 || month == 6 || month == 9 || month == 11) {
            return 30;
        }
        return 31;
    }

    static func div(a: Int, b: Int) -> Int {
        return (a - (a % b)) / b;
    }

    static func getMonthName(month: Int) -> String {
        if (month == 1) { return "Jan"; }
        if (month == 2) { return "Feb"; }
        if (month == 3) { return "Mar"; }
        if (month == 4) { return "Apr"; }
        if (month == 5) { return "May"; }
        if (month == 6) { return "Jun"; }
        if (month == 7) { return "Jul"; }
        if (month == 8) { return "Aug"; }
        if (month == 9) { return "Sep"; }
        if (month == 10) { return "Oct"; }
        if (month == 11) { return "Nov"; }
        return "Dec";
    }

    static func getDayName(dayIndex: Int) -> String {
        if (dayIndex == 0) { return "Sun"; }
        if (dayIndex == 1) { return "Mon"; }
        if (dayIndex == 2) { return "Tue"; }
        if (dayIndex == 3) { return "Wed"; }
        if (dayIndex == 4) { return "Thu"; }
        if (dayIndex == 5) { return "Fri"; }
        return "Sat";
    }

    static func timestampToString(ts: Int) -> String {
        var seconds = ts;
        var minutes = DateUtils.div(seconds, 60);
        seconds = seconds % 60;
        var hours = DateUtils.div(minutes, 60);
        minutes = minutes % 60;
        
        var totalDays = DateUtils.div(hours, 24);
        var dayOfWeek = (4 + totalDays) % 7;

        var days = totalDays;
        hours = hours % 24;

        var year = 1970;
        var daysInYear = 365;
        while (true) {
            daysInYear = 365;
            if (DateUtils.isLeapYear(year)) {
                daysInYear = 366;
            }
            if (days < daysInYear) {
                break;
            }
            days = days - daysInYear;
            year = year + 1;
        }

        var month = 1;
        var daysInMonth = 0; 
        while (true) {
            daysInMonth = DateUtils.getDaysInMonth(month, year);
            if (days < daysInMonth) {
                break;
            }
            days = days - daysInMonth;
            month = month + 1;
        }

        var day = days + 1;
        
        var dayName = DateUtils.getDayName(dayOfWeek);
        var monthName = DateUtils.getMonthName(month);

        var timeStr = "";
        if (hours < 10) { timeStr = timeStr + "0"; }
        timeStr = timeStr + hours + ":";
        if (minutes < 10) { timeStr = timeStr + "0"; }
        timeStr = timeStr + minutes + ":";
        if (seconds < 10) { timeStr = timeStr + "0"; }
        timeStr = timeStr + seconds;

        var dayStr = "" + day;
        if (day < 10) { dayStr = " " + day; }

        return dayName + " " + monthName + " " + dayStr + " " + timeStr + " " + year;
    }
}

class Time {
    static func now() -> Int {
        return __native_time();
    }
    
    static func sleep(seconds: Int) {
        __native_sleep(seconds);
    }

    static func ctime(timestamp: Int) -> String {
        return DateUtils.timestampToString(timestamp);
    }
}

# Test Logic
print("UTC Date for 0 (1970-1-1): " + Time.ctime(0));
print('\n');
print("UTC Date for 2000 (1970-1-1 + 2000s): " + Time.ctime(2000));
print('\n');
print("UTC Date for 400000000 (1982-9-5): " + Time.ctime(400000000));
print('\n');

var now = Time.now();
println("Timestamps (now): " + now);
print("Current UTC Time: " + Time.ctime(now));
print('\n');
``` 