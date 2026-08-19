#include "engine/io/path_manager.h"
#include "engine/tools/logger.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
void require(bool condition,const char* message)
{
    if (!condition)
        throw std::runtime_error(message);
}

std::string read_file(const std::filesystem::path& path)
{
    std::ifstream input(path);
    return std::string(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>());
}

void test_console_levels_and_source_location()
{
    auto* logger = engine::tools::Logger::instance();
    logger->shutdown();

    engine::tools::LoggerConfig config;
    config.file_mode = engine::tools::LogFileMode::Disabled;
    config.console_color_mode = engine::tools::ConsoleColorMode::Never;
    require(logger->configure(config),"console config must be accepted");

    std::ostringstream captured;
    std::streambuf* previous = std::clog.rdbuf(captured.rdbuf());
    logger->initialize_console();
    logger->debug("test","debug marker");
    logger->info("test","info marker");
    logger->warn("test","warn marker");
    logger->error("test","error marker");
    logger->terminating("test","terminating marker");
    logger->shutdown();
    std::clog.rdbuf(previous);

    const std::string output = captured.str();
    require(output.find("[DEBUG] [test]") != std::string::npos,"debug level must be logged");
    require(output.find("[INFO] [test]") != std::string::npos,"info level must be logged");
    require(output.find("[WARN] [test]") != std::string::npos,"warn level must be logged");
    require(output.find("[ERROR] [test]") != std::string::npos,"error level must be logged");
    require(output.find("[TERMINATING] [test]") != std::string::npos,"terminating level must be logged");
    require(output.find("logger_tests.cpp:") != std::string::npos,"source location must be logged");
}

void test_level_filtering()
{
    auto* logger = engine::tools::Logger::instance();
    engine::tools::LoggerConfig config;
    config.minimum_level = engine::tools::LogLevel::Warn;
    config.file_mode = engine::tools::LogFileMode::Disabled;
    config.console_color_mode = engine::tools::ConsoleColorMode::Never;
    require(logger->configure(config),"filter config must be accepted");

    std::ostringstream captured;
    std::streambuf* previous = std::clog.rdbuf(captured.rdbuf());
    logger->initialize();
    logger->debug("filter","discarded debug");
    logger->info("filter","discarded info");
    logger->warn("filter","retained warn");
    logger->shutdown();
    std::clog.rdbuf(previous);

    const std::string output = captured.str();
    require(output.find("discarded debug") == std::string::npos,"debug must be filtered");
    require(output.find("discarded info") == std::string::npos,"info must be filtered");
    require(output.find("retained warn") != std::string::npos,"warn must pass filter");
}

void test_file_modes()
{
    auto* path_manager = engine::io::PathManager::instance();
    require(path_manager->init(),"PathManager must initialize");
    require(path_manager->ensure_runtime_dirs(),"runtime directories must exist");

    auto* logger = engine::tools::Logger::instance();
    const std::filesystem::path append_path = path_manager->logs() / "logger-tests-append.log";
    std::error_code error;
    std::filesystem::remove(append_path,error);

    engine::tools::LoggerConfig append_config;
    append_config.file_mode = engine::tools::LogFileMode::Append;
    append_config.append_file_name = append_path.filename().string();
    append_config.console_enabled = false;
    require(logger->configure(append_config),"append config must be accepted");
    logger->initialize();
    logger->info("file","append first");
    require(logger->active_file_path() == append_path,"append path must be active");
    logger->shutdown();

    require(logger->configure(append_config),"append config must be reusable after shutdown");
    logger->initialize();
    logger->warn("file","append second");
    logger->shutdown();
    const std::string append_contents = read_file(append_path);
    require(append_contents.find("append first") != std::string::npos,"append must retain first entry");
    require(append_contents.find("append second") != std::string::npos,"append must write second entry");

    engine::tools::LoggerConfig disabled_config;
    disabled_config.file_mode = engine::tools::LogFileMode::Disabled;
    disabled_config.console_enabled = false;
    require(logger->configure(disabled_config),"disabled config must be accepted");
    logger->initialize();
    require(!logger->active_file_path().has_value(),"disabled mode must not open a file");
    logger->shutdown();

    engine::tools::LoggerConfig new_run_config;
    new_run_config.file_mode = engine::tools::LogFileMode::NewRunFile;
    new_run_config.console_enabled = false;
    require(logger->configure(new_run_config),"new-run config must be accepted");
    logger->initialize();
    const auto first_run = logger->active_file_path();
    require(first_run.has_value(),"new-run mode must open a file");
    logger->error("file","new run first");
    logger->shutdown();

    require(logger->configure(new_run_config),"new-run config must be reusable");
    logger->initialize();
    const auto second_run = logger->active_file_path();
    require(second_run.has_value(),"second new-run mode must open a file");
    require(first_run != second_run,"new-run mode must not reuse an existing path");
    logger->shutdown();

    std::filesystem::remove(append_path,error);
    std::filesystem::remove(*first_run,error);
    std::filesystem::remove(*second_run,error);
}

void test_file_failure_falls_back_to_console()
{
    auto* path_manager = engine::io::PathManager::instance();
    auto* logger = engine::tools::Logger::instance();
    const std::filesystem::path blocked_path = path_manager->logs() / "logger-tests-blocked.log";
    std::error_code error;
    std::filesystem::remove_all(blocked_path,error);
    require(std::filesystem::create_directory(blocked_path),"blocking directory must be created");

    engine::tools::LoggerConfig config;
    config.file_mode = engine::tools::LogFileMode::Append;
    config.append_file_name = blocked_path.filename().string();
    config.console_color_mode = engine::tools::ConsoleColorMode::Never;
    require(logger->configure(config),"fallback config must be accepted");

    std::ostringstream captured;
    std::streambuf* previous = std::clog.rdbuf(captured.rdbuf());
    logger->initialize();
    require(!logger->active_file_path().has_value(),"failed file sink must remain inactive");
    logger->error("fallback","console fallback marker");
    logger->shutdown();
    std::clog.rdbuf(previous);

    require(captured.str().find("console fallback marker") != std::string::npos,
        "console must remain available after file failure");
    std::filesystem::remove_all(blocked_path,error);
}
}

int main()
{
    try
    {
        test_console_levels_and_source_location();
        test_level_filtering();
        test_file_modes();
        test_file_failure_falls_back_to_console();
    }
    catch (const std::exception& exception)
    {
        std::cerr << "logger_tests failed: " << exception.what() << '\n';
        return 1;
    }

    return 0;
}
