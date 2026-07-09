#ifndef KNIGHT_H
#define KNIGHT_H

#include <string>
#include <vector>
#include "model/items/Item.h"

class Knight
{
public:
    // Regular constructor used by our generator
    Knight(std::string n, std::string o, std::string t, int h, int w, int c)
        : m_name(n), m_origin(o), m_title(t), m_height(h), m_weight(w), m_level(1), m_cost(c){}

    // Getters so your UI can display the stats
    std::string getName() const { return m_name; }
    std::string getOrigin() const { return m_origin; }
    std::string getTitle() const { return m_title; }
    int getHeight() const { return m_height; }
    int getWeight() const { return m_weight; }
    int getLevel() const { return m_level; }

    Item getArmour() const { return m_armour; }
    
    void equipArmour(const Item & armour) { m_armour = armour; }

    Item getRightHandWeapon() const { return m_rightHandWeapon; }
    void equipRightHandWeapon(const Item & weapon) { m_rightHandWeapon = weapon; }
    int attack();

    void setCost(int cost) { m_cost = cost; }
    int getCost() const { return m_cost; }

    int getHorsemanship() const { return m_horsemanship; }
    int getLancePrecision() const { return m_lancePrecision; }
    int getPosie() const { return m_poise; }
    int getSwordplay() const { return m_swordplay; }
    int getFootwork() const { return m_footwork; }
    int getVigor() const { return m_vigor; }
    int getMarksmanship() const { return m_marksmanship; }
    int getFocus() const { return m_focus; }

private:
    std::string m_name;
    std::string m_origin;
    std::string m_title;
    int m_height;
    int m_weight;
    int m_level;
    int m_cost;
    Item m_armour;
    Item m_rightHandWeapon;
    Item m_leftHandWeapon;

    // Skills
    int m_horsemanship; // Horse controll
    int m_lancePrecision; // Controll the lance
    int m_poise; // Take a hit
    int m_swordplay; // Skill with blade
    int m_footwork; // Agility
    int m_vigor; // Physical power
    int m_marksmanship; // Accuracy
    int m_focus; // Concentration

    // Other
    int m_chivalry; // Love and fame for this knight
    int m_fortitude; // psychological strength 
};

#endif