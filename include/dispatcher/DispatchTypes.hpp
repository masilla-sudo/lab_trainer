#pragma once
#include <string_view>

namespace dispatcher {

enum class UnitStatus { Available, Busy, Maintenance, Offline };
enum class IncidentStatus { Open, Assigned, Closed };
enum class Severity { Low, Medium, High, Critical };
enum class VisualMarker { None, Alert, Route, Repair, Evacuation, Sensor };

// ТОЛЬКО объявления! Без тела функции
std::string_view toString(UnitStatus status);
std::string_view toString(IncidentStatus status);
std::string_view toString(Severity severity);
std::string_view toString(VisualMarker marker);

} // namespace dispatcher