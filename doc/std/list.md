# Standard Library: List

The `List` module provides a dynamic, generic collection for storing sequences of elements of any type `T`. It is implemented as a singly-linked list under the hood.

To use it: include `std/list.gw`.

---

## Class `ListNode<T>`

An internal class representing a single element in the `List<T>`.

### Fields

*   **`value: T`**: The data stored in this node.
*   **`next: ListNode<T>`**: A reference to the next node in the sequence.

### Constructors

#### `constructor(value: T)`
Initializes a new node with the given `value` and sets `next` to `null`.

---

## Class `List<T>`

A generic, dynamic list implementation.

### Fields

*   **`head: ListNode<T>`**: A reference to the first node in the list.
*   **`tail: ListNode<T>`**: A reference to the last node in the list.
*   **`size: Integer`**: The current number of elements in the list.

### Constructors

#### `constructor()`
Initializes a new, empty list.

### Methods

#### `append(value: T)`
Adds a new element to the end of the list.

#### `get(index: Integer) -> T`
Returns the element at the specified 0-based `index`. If the index is out of bounds (less than 0 or greater than/equal to the list's length), it returns `null`.

#### `set(index: Integer, value: T)`
Replaces the element at the specified 0-based `index` with the new `value`. Does nothing if the index is out of bounds.

#### `pop() -> T`
Removes and returns the first element (head) of the list. Returns `null` if the list is empty.

#### `clear()`
Removes all elements from the list, resetting its length to 0.

#### `sum() -> T`
Calculates and returns the sum of all elements in the list. Note that this requires the type `T` to support the `+` operator.

#### `product() -> T`
Calculates and returns the product of all elements in the list. Note that this requires the type `T` to support the `*` operator.

#### `length() -> Integer`
Returns the number of elements currently in the list.

---

## Example Usage

```kotlin
# Create a list of integers
var numbers = new List<Integer>();

# Add elements
numbers.append(10);
numbers.append(20);
numbers.append(30);

println("Length: " + numbers.length()); # Output: 3
println("Item at index 1: " + numbers.get(1)); # Output: 20

# Modify an element
numbers.set(1, 25);
println("Modified item at index 1: " + numbers.get(1)); # Output: 25

# Calculate sum and product
println("Sum: " + numbers.sum()); # Output: 65
println("Product: " + numbers.product()); # Output: 7500

# Pop the first element
var firstItem = numbers.pop();
println("Popped item: " + firstItem); # Output: 10
println("New length: " + numbers.length()); # Output: 2

# Clear the list
numbers.clear();
println("Length after clear: " + numbers.length()); # Output: 0
```
