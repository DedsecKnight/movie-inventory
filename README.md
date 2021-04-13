# CS 2337 Fall 2020 Project 3 - Movie Inventory System

## Objectives
- Implement a binary search tree class in C++
- Utilize a binary search tree
- Perform simple input validation with string functions

## Project Description
- Develop report for vending kiosk around the US based on rental data transmitted from each unit
- A file containing all of the transitions (rentail, returns, and inventory maintenance) over a given period of time will be 
analyzed so that a report can be generated
- Design a program that will process each transaction and create a report listing all DVD titles and their quantities in the kiosk

## Implementation Details
- The binary search tree will be seeded with an inventory file
- After seeding, the program will parse the transaction log to update the inventory
- DVD title will be used to determine placement in the binary search tree
- There are 5 possible transactions: 
    + <b>Add new title</b>: Insert a new node into the tree 
    + <b>Add copies for an existing title</b>: Find the title and increase its number of available copies
    + <b>Remove copies of an existing title</b>: Find the title and decrease its number of available copies (delete the node if necessary)
    + <b>Rent a DVD</b>: Reduce number of available copies and increase number of rented copies
    + <b>Return a DVD</b>: Increase number of available copies and decrease number of rented copies
- The file name for the inventory and the transaction will be provided through user interface
- If there is any invalid transaction log, output it to <code>error.log</code>
- At the end of the program, display a report about each title sorted in alphabetical order with information about copies available and copies rented
