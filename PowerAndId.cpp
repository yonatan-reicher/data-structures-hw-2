#include "PowerAndId.h"

PowerAndId::PowerAndId(int strength, int id) {
    this->m_strength = strength;
    this->m_id = id;
}

bool PowerAndId::operator<(const PowerAndId& other) const {
    if (m_strength < other.m_strength)
        return true;
    if (m_strength > other.m_strength)
        return false;
    return m_id < other.m_id;
}

bool PowerAndId::operator>(const PowerAndId& other) const {
    return other < *this;
}

bool PowerAndId::operator==(const PowerAndId& other) const {
    return m_strength == other.m_strength && m_id == other.m_id;
}

