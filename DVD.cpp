/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */
#include <iostream>
#include <string>
#include <fstream> 
#include "DVD.h"

DVD::DVD() { 
    title = "";
    available = 0, rented = 0;
}

DVD::DVD(std::string t, int a, int r) {
    title = t;
    available = a, rented = r;
}

std::string DVD::getTitle() const { return title; }
int DVD::getAvailable() const { return available; }
int DVD::getRented() const { return rented; }

void DVD::setTitle(std::string t) { title = t; }
void DVD::setAvailable(int a) { available = a; }
void DVD::setRented(int r) { rented = r; }

bool DVD::operator<(const DVD& other) { return title < other.title; }
bool DVD::operator==(const DVD& other) { return title == other.title; }

