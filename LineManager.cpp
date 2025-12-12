#include "LineManager.h"
#include "Workstation.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <set>

namespace seneca {

    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream fin(file);
        if (!fin) {
            throw std::runtime_error("Unable to open file: " + file);
        }

        std::string line;
        std::set<Workstation*> currentStations;
        std::set<Workstation*> nextStations;

        while (std::getline(fin, line)) {
            std::stringstream ss(line);
            std::string currentName, nextName;
            std::getline(ss, currentName, '|');
            std::getline(ss, nextName);

            auto currWS = std::find_if(stations.begin(), stations.end(), [&](Workstation* ws) {
                return ws->getItemName() == currentName;
                });

            if (currWS == stations.end()) {
                throw std::runtime_error("***Invalid record***: " + currentName);
            }

            currentStations.insert(*currWS);

            Workstation* nextWS = nullptr;
            if (!nextName.empty()) {
                auto nextIt = std::find_if(stations.begin(), stations.end(), [&](Workstation* ws) {
                    return ws->getItemName() == nextName;
                    });

                if (nextIt == stations.end()) {
                    throw std::runtime_error("***Invalid record***: " + nextName);
                }

                nextWS = *nextIt;
                nextStations.insert(nextWS);
            }

            (*currWS)->setNextStation(nextWS);
            m_activeLine.push_back(*currWS);
        }

        // Determine first station
        for (auto* station : currentStations) {
            if (nextStations.find(station) == nextStations.end()) {
                if (m_firstStation) {
                    throw std::runtime_error("***Multiple Starting Workstations***");
                }
                m_firstStation = station;
            }
        }

        if (!m_firstStation) {
            throw std::runtime_error("***No Starting Workstation***");
        }

        m_cntCustomerOrder = g_pending.size();
    }

    void LineManager::reorderStations() {
        std::vector<Workstation*> reordered;
        Workstation* current = m_firstStation;

        while (current != nullptr) {
            reordered.push_back(current);
            current = current->getNextStation();
        }

        m_activeLine = std::move(reordered);
    }

    bool LineManager::run(std::ostream& os) {
        static size_t iteration = 1;
        os << "Line Manager Iteration: " << iteration++ << std::endl;

        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        for (auto* station : m_activeLine) {
            station->fill(os);
        }

        for (auto* station : m_activeLine) {
            station->attemptToMoveOrder();
        }

        return g_pending.empty() && (g_completed.size() + g_incomplete.size() == m_cntCustomerOrder);
    }

    void LineManager::display(std::ostream& os) const {
        for (const auto* station : m_activeLine) {
            station->display(os);
        }
    }

}
