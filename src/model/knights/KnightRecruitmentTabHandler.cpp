#include "model/knights/KnightRecruitmentTabHandler.h"
#include "model/knights/Knight.h"

KnightRecruitmentTabHandler::KnightRecruitmentTabHandler()
: m_recruitableKnights()
, m_random_engine(std::random_device {}())
{
    startDay();
}

void KnightRecruitmentTabHandler::startDay()
{
    m_recruitableKnights.clear();

    std::uniform_int_distribution<int> uniform_dist(1,3);

    std::vector<std::string> knightNames = {"Gawain", "Lancelot", "Bors", "Galahad", "Percival"};
    std::vector<std::string> knightLocations = {"English", "French", "Bavaria", "Spain", "Scotland"};
    std::vector<std::string> knightTitles = {"Knight", "Chivalric Knight", "Baron", "Count", "Earl", "Duke", "Prince"};

    std::uniform_int_distribution<int> uniform_dist2(0,knightNames.size()-1);
    std::uniform_int_distribution<int> uniform_dist3(0,knightLocations.size()-1);
    std::uniform_int_distribution<int> uniform_dist4(0,knightTitles.size()-1);
    std::uniform_int_distribution<int> uniform_dist5(150,210);
    std::uniform_int_distribution<int> uniform_dist6(70,90);
    std::uniform_int_distribution<int> uniform_dist7(100,500);

    int numKnights = uniform_dist(m_random_engine);
    for (int i = 0; i < numKnights; ++i)
    {
        Knight k(knightNames[uniform_dist2(m_random_engine)],
                 knightLocations[uniform_dist3(m_random_engine)],
                 knightTitles[uniform_dist4(m_random_engine)],
                 uniform_dist5(m_random_engine),
                 uniform_dist6(m_random_engine),
                 uniform_dist7(m_random_engine));
        m_recruitableKnights.push_back(k);
    }
}

void KnightRecruitmentTabHandler::sellKnight(const Knight& soldKnight)
{
    // Loop using an iterator to find and wipe out the target element item block
    for (auto it = m_recruitableKnights.begin(); it != m_recruitableKnights.end(); ++it)
    {
        if (it->getName() == soldKnight.getName())
        {
            m_recruitableKnights.erase(it); // Erase the matching entry item layout
            break; // Stop looking after removing one instance matching this signature
        }
    }
}