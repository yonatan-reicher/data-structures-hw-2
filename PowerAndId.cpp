#include "PowerAndId.h"

PowerAndId::PowerAndId() {
    this->m_power = 0;
    this->m_id = 0;
}

PowerAndId::PowerAndId(int power, int id) {
    this->m_power = power;
    this->m_id = id;
}

bool PowerAndId::operator<(const PowerAndId& other) const {
    // While higher power means higher order, lower id means higher order.
    // This is because lower ids win in ties.
    if (m_power < other.m_power)
        return true;
    if (m_power > other.m_power)
        return false;
    return m_id > other.m_id;
}

bool PowerAndId::operator>(const PowerAndId& other) const {
    return other < *this;
}

bool PowerAndId::operator==(const PowerAndId& other) const {
    return m_power == other.m_power && m_id == other.m_id;
}

