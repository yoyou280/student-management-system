# Student Record Management System

A high-performance C++ system that combines a generic, character-bounded Trie data structure with a real-time Student Record Management System. The project showcases efficient string indexing, dynamic memory management, and multi-index search capabilities.

## Key Features

* **Generic Bounded Trie (`Trie<V, BeginChar, EndChar>`)**
  * **Compile-Time Bounds:** Configurable branching factor based on a template-defined character range $[BeginChar, EndChar]$.
  * **Full STL-Style Interface:** Supports insertion, lookup, removal, traversal, dynamic deep copying, and operator overloading (`[]`, `+=`, `+`, `==`, `!=`, `<<`).

* **Student Management System (`System`)**
  * **Multi-Index Architecture:** Uses separate Trie instances to index records by Student ID (`sid_index`), given name (`given_name_index`), family name (`family_name_index`), and major (`major_index`).
  * **Case-Insensitive Searching:** Normalizes given and family names to uppercase for seamless querying.
  * **Intersected Lookups:** Supports exact full-name matches by cross-referencing candidate pointers across given and family name indices.

## Architecture & Class Design

| File | Description |
| :--- | :--- |
| `trie.hpp` | Header-only template implementation of the main Trie data structure. |
| `student.hpp` / `student.cpp` | `Student` struct definition and streaming operator (`<<`) formatting. |
| `system.hpp` / `system.cpp` | `System` class handling multi-index storage and search logic. |
| `main.cpp` | Interactive CLI interface for managing and querying records. |
