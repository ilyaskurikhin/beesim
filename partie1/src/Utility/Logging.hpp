#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ostream>
#include <fstream>
#include <ctime>

void
logEvent (std::string module, std::string event, bool append = true);

void
printConsole (std::string);

void
appendLog (std::string);

#endif
