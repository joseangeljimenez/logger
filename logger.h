#pragma once
#include <Arduino.h>
// #include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>

namespace anneo {

// logger 1.1.0
// Minimal logging library for embedded devices
// https://github.com/joseangeljimenez/logger
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

// Based on the Log library for Arduino by Thijs Elenbaas
// https://github.com/thijse/Arduino-Log

using printFunction = void (*)(Print*);

const int LOG_LEVEL_OFF = 0;
const int LOG_LEVEL_FATAL = 1;
const int LOG_LEVEL_ERROR = 2;
const int LOG_LEVEL_WARN = 3;
const int LOG_LEVEL_INFO = 4;
const int LOG_LEVEL_DEBUG = 5;
const int LOG_LEVEL_ALL = LOG_LEVEL_DEBUG;

// Available formats
// %s	replace with an string (char*)
// %c	replace with an character
// %d	replace with an integer value
// %D	replace with a double value
// %l	replace with an long value
// %x	replace and convert integer value into hex
// %X	like %x but combine with 0x123AB
// %b	replace and convert integer value into binary
// %B	like %x but combine with 0b10100011
// %t	replace and convert boolean value into "t" or "f"
// %T	like %t but convert into "true" or "false"

class Logger {
   public:
    // Default constructor
    Logger() : _level(LOG_LEVEL_OFF), _showLevel(true), _output(nullptr) {}

    // Setup
    void setup(int level, Print* output, bool showLevel = true);

    // Log level
    inline void level(int value) { _level = constrain(value, LOG_LEVEL_OFF, LOG_LEVEL_ALL); }
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
    void print(const char* format, va_list args);
    void printFormat(const char format, va_list* args);

    template <class T>
    void printLevel(int level, T msg, ...) {
        if (level > _level) {
            return;
        }
        if (_prefix) {
            _prefix(_output);
        }
        if (_showLevel) {
            static const char levels[] = "FEWID";
            _output->print(levels[level - 1]);
            _output->print(": ");
        }
        va_list args;
        va_start(args, msg);
        print(msg, args);
        if (_suffix) {
            _suffix(_output);
        }
        _output->print("\n");
    }

    int _level;
    bool _showLevel;
    Print* _output;

    printFunction _prefix;
    printFunction _suffix;
};

// Singleton
extern Logger logger;

}  // namespace anneo
