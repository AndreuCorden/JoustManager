#include "Player.h"
// Include specific items ONLY here in the implementation file
#include "Items/Weapon/CourteousLance.h" 
#include "Items/Armour/GothicArmour.h"

Player::Player() {
    gold = 500; 
    
    // Seed some starting items for testing
    weaponInventory.push_back(CourteousLance());
    armorInventory.push_back(GothicArmour());
    
    // Seed an initial knight
    knightsInRoster.push_back(Knight::generateRandomKnight());
}