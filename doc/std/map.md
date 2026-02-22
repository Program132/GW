# Standard Library: Map

The `Map` module provides a generic key-value store, allowing you to associate keys of type `K` with values of type `V`. It is implemented using two parallel `List` collections.

To use it: include `std/map.gw`.

---

## Class `Map<K, V>`

A generic collection that maps keys to values.

### Fields

*   **`keys: List<K>`**: Internal list storing the keys.
*   **`values: List<V>`**: Internal list storing the values at the same indices as their corresponding keys.

### Constructors

#### `constructor()`
Initializes a new, empty map.

### Methods

#### `put(key: K, value: V)`
Associates the specified `value` with the specified `key` in this map. If the map previously contained a mapping for the key, the old value is replaced.

#### `get(key: K) -> V`
Returns the value to which the specified `key` is mapped, or `null` if this map contains no mapping for the key.

#### `remove(key: K)`
Removes the mapping for a `key` from this map if it is present.

#### `contains(key: K) -> Boolean`
Returns `true` if this map contains a mapping for the specified `key`.

#### `size() -> Integer`
Returns the number of key-value mappings in this map.

#### `clear()`
Removes all of the mappings from this map.

---

## Example Usage

```kotlin
import "std/map.gw";

# Create a map of String keys to Integer values
var ages = new Map<String, Integer>();

# Add entries
ages.put("Alice", 25);
ages.put("Bob", 30);
ages.put("Charlie", 35);

println("Size: " + ages.size()); # Output: 3
println("Alice's age: " + ages.get("Alice")); # Output: 25

# Check existence
if (ages.contains("Bob")) {
    println("Bob is in the map");
}

# Update an entry
ages.put("Alice", 26);
println("Alice's new age: " + ages.get("Alice")); # Output: 26

# Remove an entry
ages.remove("Charlie");
println("Contains Charlie? " + ages.contains("Charlie")); # Output: false

# Clear the map
ages.clear();
println("Size after clear: " + ages.size()); # Output: 0
```
