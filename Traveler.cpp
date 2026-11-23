#include "Traveler.h"

// compares two colors
bool sameColor(Color a, Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b; // I searched how to compare colors and Claude suggested me to use the .r, .g, and .b that refers to r for red, g for green, b for blue. This way it gets how much red, green, and blue from (200, 40, 100) for example.
}

// constructor
Traveler::Traveler(string travelerName, int travelerAge, string travelerNationality, string travelerPassportNumber, string travelerVisaStatus) {
    name = travelerName;
    age = travelerAge;
    nationality = travelerNationality;
    passportNumber = travelerPassportNumber;
    visaStatus = travelerVisaStatus;

    // Standard and can be changed through the setter dunctions
    woman = false;
    shirtColor = WHITE;
    hairColor = BLACK;
    skinColor = WHITE;
    glasses = false;
    beard = false;
    photoWoman = false;
    photoHairColor = BLACK;
    photoGlasses = false;
    wanted = false;
} /// end of the constructor

/////////////////////////////////////////////////////////////////

// getters
string Traveler::getName() {
    return name;
}
int Traveler::getAge() {
    return age;
}
string Traveler::getNationality() {
    return nationality;
}
string Traveler::getPassportNumber() {
    return passportNumber;
}
string Traveler::getVisaStatus() {
    return visaStatus;
}
bool Traveler::isWoman() {
    return woman;
}
Color Traveler::getShirtColor() {
    return shirtColor;
}
Color Traveler::getHairColor() {
    return hairColor;
}
Color Traveler::getSkinColor() {
    return skinColor;
}
bool Traveler::hasGlasses() {
    return glasses;
}
bool Traveler::hasBeard() {
    return beard;
}
bool Traveler::isPhotoWoman() {
    return photoWoman;}
Color Traveler::getPhotoHairColor() {
    return photoHairColor;
}
bool Traveler::photoHasGlasses() {
    return photoGlasses;
}
bool Traveler::isWanted() {
    return wanted;
} // end of the getters

////////////////////////////////////////////////////////////////

// setters
void Traveler::setLook(bool isWoman, Color shirt, Color hair, Color skin, bool hasGlasses, bool hasBeard) {
    woman = isWoman;
    shirtColor = shirt;
    hairColor = hair;
    skinColor = skin;
    glasses = hasGlasses;
    beard = hasBeard;
}

void Traveler::setPhoto(bool isWoman, Color hair, bool hasGlasses) {
    photoWoman = isWoman;
    photoHairColor = hair;
    photoGlasses = hasGlasses;
}

void Traveler::setWanted(bool travelerIsWanted) {
    wanted = travelerIsWanted;
} // end of the setters

////////////////////////////////////////////////////////////

// checkers
bool Traveler::photoMatches() {
    return (photoWoman == woman) && (photoGlasses == glasses) && sameColor(photoHairColor, hairColor);
}

bool Traveler::canEnter() {
    return age >= 18 && visaStatus == "valid" && photoMatches();
}