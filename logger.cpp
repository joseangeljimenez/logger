#include "logger.h"

namespace anneo {

void Logger::setup(int level, Print* output, bool showLevel) {
    this->level(level);
    _output = output;
    this->showLevel(showLevel);
}

void Logger::print(const char* format, va_list args) {
    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            printFormat(*format, &args);
        } else {
            _output->print(*format);
        }
    }
}

void Logger::printFormat(const char format, va_list* args) {
    if (format == '%') {
        _output->print(format);
    } else if (format == 's') {
        register char* s = (char*)va_arg(*args, int);
        _output->print(s);
    } else if (format == 'd' || format == 'i') {
        _output->print(va_arg(*args, int), DEC);
    } else if (format == 'D' || format == 'F') {
        _output->print(va_arg(*args, double));
    } else if (format == 'x') {
        _output->print(va_arg(*args, int), HEX);
    } else if (format == 'X') {
        _output->print("0x");
        _output->print(va_arg(*args, int), HEX);
    } else if (format == 'b') {
        _output->print(va_arg(*args, int), BIN);
    } else if (format == 'B') {
        _output->print("0b");
        _output->print(va_arg(*args, int), BIN);
    } else if (format == 'l') {
        _output->print(va_arg(*args, long), DEC);
    } else if (format == 'c') {
        _output->print((char)va_arg(*args, int));
    } else if (format == 't') {
        if (va_arg(*args, int) == 1) {
            _output->print("T");
        } else {
            _output->print("F");
        }
    } else if (format == 'T') {
        if (va_arg(*args, int) == 1) {
            _output->print(F("true"));
        } else {
            _output->print(F("false"));
        }
    }
}

// Singleton
Logger logger = Logger();

}  // namespace anneo
