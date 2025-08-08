#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

string getMood(int hunger, int happiness, int energy) {
    if (hunger <= 30 || happiness <= 30 || energy <= 30) return "Unhappy";
    if (hunger >= 80 && happiness >= 80 && energy >= 80) return "Happy";
    return "Neutral";
}

void feedPet(string name, int& hunger, int& happiness) {
    int choice;
    cout << "Choose what to feed " << name << ":\n1. Snack (+5 hunger)\n2. Meal (+15 hunger, +5 happiness)\n>> ";
    cin >> choice;

    if (choice == 1) {
        hunger += 5;
        cout << name << " enjoyed the snack! :)\n";
    }
    else if (choice == 2) {
        hunger += 15;
        happiness += 5;
        cout << name << " enjoyed the meal! :)\n";
    }
    else {
        cout << "Invalid choice. Nothing was given.\n";
    }

    if (hunger > 100) hunger = 100;
    if (happiness > 100) happiness = 100;
}


void playWithPet(string name, int& happiness, int& energy, int& hunger) {
    happiness += 10;
    if (happiness > 100) happiness = 100;
    energy -= 15;
    if (energy < 0) energy = 0;
    string messages[] = {
       name + " wagged its tail happily!",
       name + " chased a butterfly!",
       name + " jumped around in circles!",
       name + " nuzzled your hand!",
       name + " rolled in the grass!",
       name + " did a silly little dance!",
       name + " played fetch with you!"
    };

    int index = rand() % 7;
    cout << "You played with " << name << "!\n";
    cout << messages[index] << "\n";
    cout << "Happiness: " << happiness << " | Energy: " << energy << " | Hunger: " << hunger << endl;
}

void petSleep(string name, int& energy) {
    energy += 20;
    if (energy > 100) energy = 100;
    cout << name << " curled up and took a peaceful nap. \n";
}

void giveMedicine(string name, int& health) {
    if (health < 100) {
        health += 30;
        if (health > 100) health = 100;
        cout << name << " took some medicine and is feeling better!\n";
    }
    else {
        cout << name << " is already healthy.\n";
    }
}

void showStatus(string name, int hunger, int happiness, int energy, int age, string stage, int days, int health) {
    cout << "\n--- " << name << "'s Status ---\n";
    cout << "Day: " << days << "\nStage: " << stage << "\nAge: " << age << " years\n";
    cout << "Hunger: " << hunger << "/100\nHappiness: " << happiness << "/100\n";
    cout << "Energy: " << energy << "/100\nHealth: " << health << "/100\n";
    cout << "Mood: " << getMood(hunger, happiness, energy) << "\n";
}

void warnLowStats(int hunger, int happiness, int energy) {
    if (hunger <= 20 || happiness <= 20 || energy <= 20)
        cout << "Warning: Your pet is in critical condition!\n";
}

bool checkPetStatus(string name, int hunger, int happiness, int energy, int health) {
    if (hunger <= 0) { cout << name << " was starving...\n"; return false; }
    if (happiness <= 0) { cout << name << " became too sad and ran away.\n"; return false; }
    if (energy <= 0) { cout << name << " collapsed from exhaustion.\n"; return false; }
    if (health <= 0) { cout << name << " fell too sick to continue...\n"; return false; }
    return true;
}

void saveProgress(string profile, string password, string name, int hunger, int happiness, int energy, int health, int age, string stage, int days) {
    ofstream file("pet_" + profile + ".txt");
    file << password << "\n" << name << "\n" << hunger << " " << happiness << " " << energy << "\n";
    file << health << "\n" << age << "\n" << stage << "\n" << days << "\n";
    file.close();
}

bool loadProgress(string profile, string password, string& name, int& hunger, int& happiness, int& energy, int& health, int& age, string& stage, int& days) {
    ifstream file("pet_" + profile + ".txt");
    if (!file) return false;
    string savedPassword;
    getline(file, savedPassword);
    if (savedPassword != password) return false;
    getline(file, name);
    file >> hunger >> happiness >> energy;
    file >> health >> age;
    file.ignore(); getline(file, stage);
    file >> days;
    return true;
}

int main() {
    srand(time(0));
    string profile, password;
    cout << "Enter your profile name: "; cin >> profile;
    cout << "Enter your password: "; cin >> password;

    int days = 1, hunger, happiness, energy, health = 100, age = 0, turn = 0, lowStatCounter = 0, consecutiveHighStatsDays = 0;
    string petName, stage = "Baby";
    bool achieved10Days = false, achievedHighStats = false;

    cout << "Load last pet? (yes/y/1): ";
    string load; cin >> load;
    if ((load == "yes" || load == "y" || load == "1") && loadProgress(profile, password, petName, hunger, happiness, energy, health, age, stage, days)) {
        cout << "Welcome back, " << petName << "!\n";
    }
    else {
        cout << "Pet name: "; cin >> petName;
        hunger = happiness = energy = 60;
        health = 100;
    }

    int choice;
    do {
        cout << "\n--- Day " << days << " ---\n";
        cout << "1. Feed " << petName << endl;
        cout << "2. Play with " << petName << endl;
        cout << "3. Let " << petName << " sleep" << endl;
        cout << "4. Show status" << endl;
        cout << "5. Give " << petName << " a medicine \n";
        cout << "6. Save the Game\n7. Exit\n>> ";

        cin >> choice;

        bool fed = false, played = false, slept = false;

        if (choice == 1) { feedPet(petName, hunger, happiness); fed = true; }
        else if (choice == 2) { playWithPet(petName, happiness, energy, hunger); played = true; }
        else if (choice == 3) { petSleep(petName, energy); slept = true; }
        else if (choice == 4) showStatus(petName, hunger, happiness, energy, age, stage, days, health);
        else if (choice == 5) giveMedicine(petName, health);
        else if (choice == 6) {
            saveProgress(profile, password, petName, hunger, happiness, energy, health, age, stage, days);
            cout << "Game saved successfully!\n";
        }
        else if (choice == 7) {
            break;
        }
        else cout << "Invalid option.\n";

        warnLowStats(hunger, happiness, energy);

        if (!checkPetStatus(petName, hunger, happiness, energy, health)) break;

        static int inactiveTurns = 0;

        if (!fed && !played && !slept) {
            inactiveTurns++;
            if (inactiveTurns >= 6) {
                int loss = rand() % 2 + 2; // 2 or 3
                health -= loss;
                if (health < 0) health = 0;
                cout << petName << " felt neglected for too long. -" << loss << " health\n";
                inactiveTurns = 0;
            }
        }
        else {
            inactiveTurns = 0;
        }


        turn++;
        if (turn % 5 == 0) {
            hunger -= 5; if (hunger < 0) hunger = 0;
            days++;
            cout << petName << "'s stomach growls... -5 hunger\n";
        }

        if (hunger <= 20 || happiness <= 20 || energy <= 20) {
            lowStatCounter++;
            if (lowStatCounter >= 3) {
                health -= 10; if (health < 0) health = 0;
                cout << petName << " is sick! -10 health\n";
                lowStatCounter = 0;
            }
        }
        else lowStatCounter = 0;

        if (turn % 3 == 0) {
            int event = rand() % 3;
            if (event == 0) {
                happiness += 5;
                if (happiness > 100) happiness = 100;
                cout << petName << " found a toy! +5 happiness\n";
            }
            else if (event == 1) {
                energy -= 5;
                if (energy < 0) energy = 0;
                cout << petName << " was scared by thunder. -5 energy\n";
            }
            else {
                happiness += 10;
                if (happiness > 100) happiness = 100;
                cout << petName << " got cuddles! +10 happiness\n";
            }
        }

        if (turn % 10 == 0) {
            age++;
            if (age >= 10) stage = "Adult";
            else if (age >= 5) stage = "Teen";
        }

        if (days >= 10 && !achieved10Days) { cout << "Achievement unlocked: Survived 10 Days!\n"; achieved10Days = true; }

        if (hunger >= 80 && happiness >= 80 && energy >= 80) {
            consecutiveHighStatsDays++;
            if (consecutiveHighStatsDays >= 3 && !achievedHighStats) {
                cout << "Achievement unlocked: Perfect Care - All Stats High for 3 Days!\n";
                achievedHighStats = true;
            }
        }
        else consecutiveHighStatsDays = 0;

        if (hunger >= 70 && happiness >= 70 && energy >= 70) {
            string trick;
            cout << "Type a trick (sit / roll / none): "; cin >> trick;
            if (trick == "sit") cout << petName << " sits like a good pet!\n";
            else if (trick == "roll") cout << petName << " rolls over!\n";
            else if (trick != "none") cout << petName << " doesn't know that trick yet.\n";
        }

    } while (choice != 7);

    cout << "\nGoodbye! Thanks for playing.\n";
    return 0;
}

// My Pet Pal -
//               by Alexia Dinca <3
