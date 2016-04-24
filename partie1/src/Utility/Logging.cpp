#include <Utility/Logging.hpp>

void
logEvent (std::string module, std::string event, bool append)
{
  bool VERBOSE (true);
  if (VERBOSE)
    {
      std::stringstream console;
      printConsole (module + "\t" + event);
    }
  if (append)
    {
      appendLog (module + "\t" + event);
    }
}

void
printConsole (std::string string)
{
  std::clog << std::setw (80) << std::setprecision (3) << "\r" << string << "\r"
      << std::flush;
}

void
appendLog (std::string string)
{
  bool APPEND (true);
  time_t time;
  struct tm* timeinfo;

  std::time (&time);
  timeinfo = localtime (&time);

  char displayTime[80];
  strftime (displayTime, 80, "%F %T %Z", timeinfo);

  if (APPEND)
    {
      std::ofstream output;
      output.open ("build/execution.log", std::ios::app);
      output << displayTime << "\t" << string << "\n";
      output.close ();
    }
}
