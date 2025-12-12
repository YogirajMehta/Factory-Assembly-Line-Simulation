#include "Station.h"
#include "Utilities.h"
#include <iomanip>

namespace seneca {
    size_t Station::m_widthField = 0;
    size_t Station::id_generator = 0;

    Station::Station(const std::string& record) {
        m_id = ++id_generator;

        Utilities util; 
        size_t next_pos = 0;
        bool more = true;

        m_itemName = util.extractToken(record, next_pos, more);

        std::string serialStr = util.extractToken(record, next_pos, more);
        try {
            m_nextSerialNumber = std::stoul(serialStr);
        }
        catch (...) {
            m_nextSerialNumber = 0; 
        }
        std::string quantityStr = util.extractToken(record, next_pos, more);
        try {
            m_quantity = std::stoul(quantityStr);
        }
        catch (...) {
            m_quantity = 0; 
        }
        m_widthField = std::max(m_widthField, util.getFieldWidth());

        m_description = util.extractToken(record, next_pos, more);
    }

    const std::string& Station::getItemName() const {
        return m_itemName;
    }

    size_t Station::getNextSerialNumber() {
        return m_nextSerialNumber++;
    }

    size_t Station::getQuantity() const {
        return m_quantity;
    }

    void Station::updateQuantity() {
        if (m_quantity > 0) {
            m_quantity--;
        }
    }

    void Station::display(std::ostream& os, bool full) const {
        os << std::right << std::setw(3) << std::setfill('0') << m_id << " | ";
        os << std::left << std::setw(m_widthField) << std::setfill(' ') << m_itemName << " | ";
        os << std::right << std::setw(6) << std::setfill('0') << m_nextSerialNumber << " | ";

        if (full) {
            os << std::right << std::setw(4) << std::setfill(' ') << m_quantity << " | ";
            os << m_description;
        }

        os << std::endl;
    }

}
