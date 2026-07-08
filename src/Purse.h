#ifndef PURSE_H
#define PURSE_H

class Purse
{
public:
    Purse() = default;
    explicit Purse(unsigned value) : m_value(value) {}

    Purse &operator+=(unsigned value)
    {
        m_value += value;
        return *this;
    }

    bool buy(unsigned value)
    {
        if (m_value >= value)
        {
            m_value -= value;
            return true;
        }
        return false;
    }

    unsigned get() const
    {
        return m_value;
    }

private:
    unsigned m_value = 0;
};

#endif