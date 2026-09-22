#pragma once
#include <filesystem>
#include <fstream>
#include <string_view>

namespace dispatcher {

class EventLog {
public:
    explicit EventLog(const std::filesystem::path& path);
    ~EventLog();
    void write(std::string_view message);

private:
    std::ofstream out_;
};

} // namespace dispatcher