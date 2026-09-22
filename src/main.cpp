#include <iostream>
#include <filesystem>
#include "dispatcher/DispatchTypes.hpp"
#include "dispatcher/DispatchUnit.hpp"
#include "dispatcher/Incident.hpp"
#include "dispatcher/EventLog.hpp"
#include "dispatcher/DispatchCenter.hpp"

int main() {
    namespace fs = std::filesystem;

    try {
        dispatcher::EventLog log{fs::path("dispatch.log")};
        dispatcher::DispatchCenter center{log};

        center.addUnit(dispatcher::DispatchUnit{1, "Brigade A", "north"});
        center.addUnit(dispatcher::DispatchUnit{2, "Drone 1", "center"});
        center.addUnit(dispatcher::DispatchUnit{3, "Technician B", "north"});

        center.addIncident(dispatcher::Incident{
            100,
            "Smoke detected near station",
            "north",
            dispatcher::Severity::High,
            dispatcher::VisualMarker::Alert
        });

        center.addIncident(dispatcher::Incident{
            101,
            "Power outage in sector C",
            "center",
            dispatcher::Severity::Critical,
            dispatcher::VisualMarker::Repair
        });

        std::cout << "Initial state:\n";
        center.printState();

        std::cout << "Assigning unit 1 to incident 100:\n";
        center.assign(1, 100);
        center.printState();

        std::cout << "Assigning unit 2 to incident 101:\n";
        center.assign(2, 101);
        center.printState();

        std::cout << "Closing incident 100:\n";
        center.closeIncident(100);
        center.printState();

        std::cout << "Trying to assign unit 1 (now busy) to incident 101:\n";
        center.assign(1, 101);
        center.printState();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}