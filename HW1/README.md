# Queue\<Anon>

* Author: Rylee Hilde
* Class: CS452 Section 001
* Semester: Fall 2026

## Overview

This program implements a double-ended doubly linked queue of pointers to arbitrary data objects.

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

This project went quite smooth. Although it took some time to understand, I felt like I was able to develop some clean code to implement the interface that was given. The double endedness of this abstract data type is also very interesting and I am looking forward to hopefully using this in future projects. 

## Results

When running main.c I tested the following:

- Adding nodes from the head and tail using Put
- Removing nodes from the head and tail using Get
- Removing from an empty list using Get
- Searching and Removing from an empty list using Ith and Rem
- Searching and Removing from a populated list using Ith and Rem
- Searching and Removing the edges (Head or Tail) from a populating list using Ith and Rem

These tests simply highlight the behavior of the implemented methods and do not show whether the behavior is correct. It is up to the person that runs main to see if the behavior is correct. As this was mainly a tool for the developer (me), I believe that the behavior of my tests are correct and tests for most cases. 

## Sources used

No sources were used other than what were provided by the class

----------
