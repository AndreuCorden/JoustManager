#include "Knight.h"

int Knight::attack()
{
    return m_rightHandWeapon.getBonus();
}

void Knight::gainXP(int amount) {
    m_xp += amount;
    // Handle multiple level ups if they gain massive XP
    while (m_xp >= getXPThreshold()) {
        m_xp -= getXPThreshold();
        m_level++;
        m_pendingSkillPoints += 3; // Award 3 attribute points per level
    }
}

bool Knight::upgradeSkill(Skill skill) {
    if (m_pendingSkillPoints <= 0) return false;

    switch (skill) {
        case Skill::Horsemanship:   if (m_horsemanship >= 100) return false; m_horsemanship++; break;
        case Skill::LancePrecision: if (m_lancePrecision >= 100) return false; m_lancePrecision++; break;
        case Skill::Poise:          if (m_poise >= 100) return false; m_poise++; break;
        case Skill::Swordplay:      if (m_swordplay >= 100) return false; m_swordplay++; break;
        case Skill::Footwork:       if (m_footwork >= 100) return false; m_footwork++; break;
        case Skill::Vigor:          if (m_vigor >= 100) return false; m_vigor++; break;
        case Skill::Marksmanship:   if (m_marksmanship >= 100) return false; m_marksmanship++; break;
        case Skill::Focus:          if (m_focus >= 100) return false; m_focus++; break;
    }
    
    m_pendingSkillPoints--;
    // Automatically recalculate the knight's market value based on upgraded stats
    m_cost = (m_level * 100) + ((m_horsemanship + m_lancePrecision + m_poise + m_swordplay + m_footwork + m_vigor + m_marksmanship + m_focus) * 10);
    return true;
}