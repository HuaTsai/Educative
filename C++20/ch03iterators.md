# Ch03 Iterators

| Concept                    | Description                                                                                                               |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------- |
| `indirectly_readable`      | An iterator can be read by the dereference operator, `*`. This includes pointers, smart pointers, and input iterators.    |
| `indirectly_writable`      | The object reference of the iterator is writable.                                                                         |
| `weakly_incrementable`     | This can be incremented with `++` but does not preserve equality. For example, where `a == b`, `++a` may not equal `++b.` |
| `incrementable`            | This can be incremented with `++` and equality is preserved.                                                              |
| `input_or_output_iterator` | An iterator can be incremented and dereferenced. Every iterator most satisfy this concept.                                |
| `sentinel_for`             | A sentinel iterator is used to find the end of an object of indeterminate size, such as an input stream.                  |
| `sized_sentinel_for`       | A sentinel iterator may be used with another iterator and the `-` operator to determine its distance in constant time.    |
| `input_iterator`           | An iterator that may be read and incremented.                                                                             |
| `output_iterator`          | An iterator that may be written to and incremented.                                                                       |
| `forward_iterator`         | This modifies `input_iterator` to include incrementable.                                                                  |
| `bidirectional_iterator`   | This modifies `forward_iterator` by adding the ability to decrement with the `--` operator. It preserves equality.        |
| `random_access_iterator`   | This modifies `bidirectional_iterator` by adding support for the `+`, `+=`, `-`, `-=`, and `[ ]` operators.               |
| `contiguous_iterator`      | This modifies `random_access_iterator` to indicate contiguous storage.                                                    |
