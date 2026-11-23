#ifndef TRAVELER_H
#define TRAVELER_H
#include <string>
#include "raylib.h"
using namespace std;

class Traveler {
    private:
        string name;
        int age;
        string nationality;
        string passportNumber; // 5 digits
        string visaStatus; // valid, expired or none

        // how the traveler really looks
        bool woman;
        Color shirtColor;
        Color hairColor;
        Color skinColor;
        bool glasses;
        bool beard;

        // how the traveler looks in the ID photo
        bool photoWoman;
        Color photoHairColor;
        bool photoGlasses;

        bool wanted;

    public:
        Traveler(string travelerName, int travelerAge, string travelerNationality,
                 string travelerPassportNumber, string travelerVisaStatus);

        // Getters
        string getName();
        int getAge();
        string getNationality();
        string getPassportNumber();
        string getVisaStatus();
        bool isWoman();
        Color getShirtColor();
        Color getHairColor();
        Color getSkinColor();
        bool hasGlasses();
        bool hasBeard();
        bool isPhotoWoman();
        Color getPhotoHairColor();
        bool photoHasGlasses();
        bool isWanted();

        // Setters
        void setLook(bool isWoman, Color shirt, Color hair, Color skin, bool hasGlasses, bool hasBeard);
        void setPhoto(bool isWoman, Color hair, bool hasGlasses);
        void setWanted(bool travelerIsWanted);

        // Rules
        bool photoMatches();   // is the ID photo the same person?
        bool canEnter();       // age 18+, valid visa, photo matches
};
#endif