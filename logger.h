#pragma once
#include <printf.h>
#include <stdarg.h>

namespace anneo {

// logger 1.2.0
// Minimal logging library for embedded devices
// https://github.com/joseangeljimenez/logger
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

// Depends on the great tiny printf library by Marco Paland
// https://github.com/mpaland/printf

// Based on the Log library for Arduino by Thijs Elenbaas
// https://github.com/thijse/Arduino-Log

using printFunction = void (*)();

const int LOG_LEVEL_OFF = 0;
const int LOG_LEVEL_FATAL = 1;
const int LOG_LEVEL_ERROR = 2;
const int LOG_LEVEL_WARN = 3;
const int LOG_LEVEL_INFO = 4;
const int LOG_LEVEL_DEBUG = 5;
const int LOG_LEVEL_ALL = LOG_LEVEL_DEBUG;

// Helper function
template <class T>
T _constrain(T value, T low, T high) {
    return value < low ? low : (value > high ? high : value);
}

class Logger {
   public:
    // Default constructor
    Logger() : _level(LOG_LEVEL_OFF), _showLevel(true) {}

    // Setup
    inline void setup(int level, bool showLevel = true) {
        this->level(level);
        this->showLevel(showLevel);
    }

    // Log level
    inline void level(int value) { _level = _constrain(value, LOG_LEVEL_OFF, LOG_LEVEL_ALL); }
    inline int level() const { return _level; }

    // Show log level
    inline void showLevel(bool value) { _showLevel = value; }
    inline bool showLevel() const { return _showLevel; }

    // Function to be called before each log command
    inline void prefix(printFunction f) { _prefix = f; }

    // Function to be called after each log command
    inline void suffix(printFunction f) { _suffix = f; }

    // Output a FATAL message
    template <class T, typename... Args>
    void fatal(T msg, Args... args) {
        printLevel(LOG_LEVEL_FATAL, msg, args...);
    }

    // Output an ERROR message
    template <class T, typename... Args>
    void error(T msg, Args... args) {
        printLevel(LOG_LEVEL_ERROR, msg, args...);
    }

    // Output a WARN message
    template <class T, typename... Args>
    void warn(T msg, Args... args) {
        printLevel(LOG_LEVEL_WARN, msg, args...);
    }

    // Output a INFO message
    template <class T, typename... Args>
    void info(T msg, Args... args) {
        printLevel(LOG_LEVEL_INFO, msg, args...);
    }

    // Output a DEBUG message
    template <class T, typename... Args>
    void debug(T msg, Args... args) {
        printLevel(LOG_LEVEL_DEBUG, msg, args...);
    }

   protected:
    template <class T>
    void printLevel(int level, T msg, ...) {
        if (level > _level) {
            return;
        }
        if (_prefix) {
            _prefix();
        }
        if (_showLevel) {
            static const char levels[] = "FEWID";
            printf_("%c: ", levels[level - 1]);
        }
        va_list args;
        va_start(args, msg);
        vprintf_(msg, args);
        if (_suffix) {
            _suffix();
        }
        printf_("\n");
    }

    int _level;
    bool _showLevel;

    printFunction _prefix;
    printFunction _suffix;
};

}  // namespace anneo
