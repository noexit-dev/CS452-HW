# Memory Hole

* Author: Rylee Hilde
* Class: CS452 Section 001
* Semester: Fall 2026

## Overview

This program implements a Buddy Allocator memory management system that functions in user space.

## Usage

To compile the program please navigate to the project directory and run the following command:

```make
make
```

To run the program run the following command:

```make
make run
```

## Reflection

This project was probably the most difficult projects I have ever had the chance to work on. Despite the difficulty, I found it to be quite fun to learn about and was very proud of the progress I had made to complete it. One of the biggest difficulties I had faced with this project was with the bitmaps and splitting blocks. As I had coded most of the implementation it was hard to trace that these were the root of the problems.

## Results

When running main.c I tested the following:

- Allocation of incorrect size
- Two allocations from the same pool
- Two frees from the same pool
- There is some print functionality that is commented out that show bitmap and head addresses for the freelist, but are not used for readablity

DEQ tests using wrapper.c
- Adding nodes from the head and tail using Put
- Removing nodes from the head and tail using Get
- Removing from an empty list using Get
- Searching and Removing from an empty list using Ith and Rem
- Searching and Removing from a populated list using Ith and Rem
- Searching and Removing the edges (Head or Tail) from a populating list using Ith and Rem

These tests simply highlight the behavior of the implemented methods and do not show whether the behavior is correct. It is up to the person that runs main to see if the behavior is correct. As this was mainly a tool for the developer (me), I believe that the behavior of my tests are correct and tests for most cases. 

----------
