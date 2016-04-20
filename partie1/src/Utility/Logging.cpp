#include <Utility/Logging.hpp>


void
logEvent(std::string module, std::string event)
{
    bool VERBOSE(true);
    if (VERBOSE)
      {
        std::stringstream console;
        printConsole(module + "\t" + event);
      }
}

void
printConsole(std::string string)
{
    std::cerr << std::setw(80) << std::setprecision(3)
        << "\r" << string << "\r" << std::flush;
}

void
appendLog(std::string string)
{
    std::ofstream output;
    output.open("build/execution.log");
    output << string << "\n";
    output.close();
}
