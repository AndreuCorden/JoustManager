#ifndef KNIGHTRECRUITMENTHANDLERTAB_H
#define KNIGHTRECRUITMENTHANDLERTAB_H

#pragma once

#include "model/knights/Knight.h"

#include <list>
#include <random>

class KnightRecruitmentTabHandler
{
public:
    KnightRecruitmentTabHandler();

    void startDay();

    const std::list<Knight>& getBuyableKnight() const { return m_recruitableKnights; }
    void sellKnight(const Knight& soldKnight);

private:
    std::list<Knight> m_recruitableKnights;

    std::default_random_engine m_random_engine;
};

#endif