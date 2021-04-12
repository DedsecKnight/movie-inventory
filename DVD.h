/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */
#include <string>
#ifndef DVD_H
#define DVD_H

class DVD {
public:
    DVD();
    DVD(std::string, int, int);
    
    std::string getTitle() const;
    int getAvailable() const;
    int getRented() const;
    
    void setTitle(std::string);
    void setAvailable(int);
    void setRented(int);
    
    bool operator<(const DVD&); 
    bool operator==(const DVD&);
    
private:
    std::string title;
    int available;
    int rented;
};

#endif
