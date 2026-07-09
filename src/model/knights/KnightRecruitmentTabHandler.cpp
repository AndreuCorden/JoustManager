#include "model/knights/KnightRecruitmentTabHandler.h"
#include "model/knights/Knight.h"
#include <algorithm> // Required for std::remove_if

// 🌟 FIX: Changed type to std::default_random_engine& to match your header file precisely
int roll(int min, int max, std::default_random_engine& engine) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

KnightRecruitmentTabHandler::KnightRecruitmentTabHandler()
    : m_recruitableKnights(), m_random_engine(std::random_device{}())
{
    startDay();
}

void KnightRecruitmentTabHandler::startDay()
{
    m_recruitableKnights.clear();

    const std::vector<std::string> names = {"Gawain", "Lancelot", "Bors", "Galahad", "Percival"};
    const std::vector<std::string> locations = {"English", "French", "Bavaria", "Spain", "Scotland"};
    const std::vector<std::string> titles = {"Knight", "Chivalric Knight", "Baron", "Count", "Earl", "Duke", "Prince"};

    int numKnights = roll(1, 3, m_random_engine);
    
    for (int i = 0; i < numKnights; ++i)
    {
        // Roll Core Identity Metrics
        std::string name = names[roll(0, names.size() - 1, m_random_engine)];
        std::string origin = locations[roll(0, locations.size() - 1, m_random_engine)];
        std::string title = titles[roll(0, titles.size() - 1, m_random_engine)];
        
        int height = roll(150, 210, m_random_engine);
        int weight = roll(height / 3, height / 2, m_random_engine);
        int level = roll(1, 5, m_random_engine);

        // Roll Combat Sub-stats based on Level
        int minStat = 0 + (level / 2);
        int maxStat = 5 + (level / 2);

        int hm = roll(minStat, maxStat, m_random_engine);
        int lp = roll(minStat, maxStat, m_random_engine);
        int ps = roll(minStat, maxStat, m_random_engine);
        int sp = roll(minStat, maxStat, m_random_engine);
        int fw = roll(minStat, maxStat, m_random_engine);
        int vg = roll(minStat, maxStat, m_random_engine);
        int ms = roll(minStat, maxStat, m_random_engine);
        int fc = roll(minStat, maxStat, m_random_engine);

        // Transparent Dynamic Pricing Formula
        int totalStats = hm + lp + ps + sp + fw + vg + ms + fc;
        int goldCost = (level * 100) + (totalStats * 10);

        // Safe sequential construction
        m_recruitableKnights.emplace_back(
            name, origin, title, height, weight, level, goldCost,
            hm, lp, ps, sp, fw, vg, ms, fc
        );
    }
}

void KnightRecruitmentTabHandler::sellKnight(const Knight &soldKnight)
{
    // 🌟 FIX: Replaced C++20 erase_if with the pre-C++20 "Erase-Remove" idiom.
    // std::remove_if shifts all matching items to the back, and .erase() chops them off.
    m_recruitableKnights.erase(
        std::remove_if(m_recruitableKnights.begin(), m_recruitableKnights.end(), 
            [&soldKnight](const Knight& k) {
                return k.getName() == soldKnight.getName();
            }), 
        m_recruitableKnights.end()
    );
}