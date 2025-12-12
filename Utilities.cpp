/*
Name-Yogiraj Mehta
email-ymehta4@myseneca.ca
id-162867238
date-23-11-2025
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/
#include "Utilities.h"
#include "Station.h"
#include <stdexcept>
namespace seneca {

    char Utilities::m_delimiter = ',';

    void Utilities::setFieldWidth(size_t newWidth) {
        m_widthField = newWidth;
    }

    size_t Utilities::getFieldWidth() const {
        return m_widthField;
    }

    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
        std::string token = "";

        if (next_pos >= str.length()) {
            more = false;
            return token;
        }

        size_t start_pos = next_pos;

        size_t end_pos = str.find(m_delimiter, start_pos);
        if (start_pos == end_pos) {
            more = false; 
            throw std::runtime_error("No token."); 
        }

        if (end_pos != std::string::npos) {
            token = str.substr(start_pos, end_pos - start_pos);
            next_pos = end_pos + 1;
            more = true;
        }
        else {
            token = str.substr(start_pos);
            next_pos = str.length();
            more = false; 
        }
        size_t first = token.find_first_not_of(' ');
        if (std::string::npos == first) {
            token.clear(); 
        }
        else {
            size_t last = token.find_last_not_of(' ');
            token = token.substr(first, (last - first + 1));
        }
        m_widthField = std::max(m_widthField, token.length());
        return token;
    }

    void Utilities::setDelimiter(char newDelimiter) {
        m_delimiter = newDelimiter;
    }

    const char Utilities::getDelimiter() {
        return m_delimiter;
    }

}
