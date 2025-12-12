/*
Name-Yogiraj Mehta
email-ymehta4@myseneca.ca
id-162867238
date-23-11-2025
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/
#include "Workstation.h"

namespace seneca {
    // Define Global Queues
    std::deque<CustomerOrder> g_pending;
    std::deque<CustomerOrder> g_completed;
    std::deque<CustomerOrder> g_incomplete;

    Workstation::Workstation(const std::string& record) : Station(record) {}

    void Workstation::fill(std::ostream& os) {
        if (!m_orders.empty()) {
            m_orders.front().fillItem(*this, os);
        }
    }

    bool Workstation::attemptToMoveOrder() {
        if (m_orders.empty()) {
            return false;
        }

        CustomerOrder& currentOrder = m_orders.front();
        std::string stationItemName = getItemName();

        bool readyToMove = currentOrder.isItemFilled(stationItemName) || (getQuantity() == 0);

        if (readyToMove) {
            if (m_pNextStation) {
                *m_pNextStation += std::move(currentOrder);
            }
            else {
                if (currentOrder.isOrderFilled()) {
                    g_completed.push_back(std::move(currentOrder));
                }
                else {
                    g_incomplete.push_back(std::move(currentOrder));
                }
            }

            m_orders.pop_front();
            return true;
        }

        return false;
    }

    void Workstation::setNextStation(Workstation* station) {
        m_pNextStation = station;
    }

    Workstation* Workstation::getNextStation() const {
        return m_pNextStation;
    }
    void Workstation::display(std::ostream& os) const {
        os << getItemName() << " --> ";
        if (m_pNextStation) {
            os << m_pNextStation->getItemName() << std::endl;
        }
        else {
            os << "End of Line" << std::endl;
        }
    }

    Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
        m_orders.push_back(std::move(newOrder));
        return *this;
    }
}