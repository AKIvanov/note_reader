# 📄 Note Reader – A Tag-Based Search Engine in C++

## Overview
**Note Reader** is a small C++20 project that lets you search a text file of notes using hashtags (`#tags`).  
It builds an in-memory index of tags → line numbers and supports **Boolean queries** with `AND` and `OR`.

This project demonstrates:
- Text parsing in C++  
- STL containers (`unordered_map`, `vector`)  
- STL algorithms (`sort`, `unique`)  
- Basic Boolean search logic  


## Example Input File

`notes.txt`:

## Example Usage

Run the program:
./note_reader

## Example Inputs:
cpp AND stl
cpp OR threads

## Example Output: 
Line Number: 2, Line: Started building a small key-value database project #cpp #stl #project
Line Number: 3, Line: Remember to check iterator invalidation rules when erasing from vector #cpp #stl #iterators
