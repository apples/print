/*******************************************************************************
 * Print - A fully type-safe, templated version of printf().
 * Version: 0.0.1
 * https://github.com/dbralir/print
 *
 * Copyright (c) 2013 Jeramy Harrison <dbralir@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <type_traits>

template <typename S>
class _PrintDetail
{
    S& out;
    std::ios saveState;
public:
    _PrintDetail (S& s)
        : out(s)
        , saveState(nullptr)
    {
        saveState.copyfmt(out);
    }
    
    ~_PrintDetail ()
    {
        out.copyfmt(saveState);
    }
    
    static constexpr char TRIGGER = '%';
    static constexpr char SPANNER = '$';
    
    void _print(const char* format)
    {
        for (; *format != '\0'; ++format)
        {
            if (*format == TRIGGER)
            {
                if (*(++format) != TRIGGER)
                {
                    out << format;
                    throw std::runtime_error("print(): Invalid format string.");
                }
            }
            out << *format;
        }
    }

    template <typename T, typename... Vs>
    void _print(const char* format, T&& item, Vs&&... others)
    {
        for (; *format != '\0'; ++format)
        {
            if (*format == TRIGGER)
            {
                if (*(++format) != TRIGGER)
                {
                    return _print_width(format, std::forward<T>(item), std::forward<Vs>(others)...);
                }
            }
            out << *format;
        }
        throw std::runtime_error("print(): Too many parameters.");
    }

    template <typename T, typename... Vs>
    void _print_width(const char* format, T&& item, Vs&&... others)
    {
        bool chunnel = true;
        while (chunnel)
        {
            switch(*format)
            {
                case '-':
                {
                    out << std::left;
                break;}
                case '+':
                {
                    out << std::showpos;
                break;}
                case '0':
                {
                    out << std::setfill('0');
                break;}
                case 'x':
                case 'X':
                {
                    out << std::hex;
                break;}
                default:
                {
                    chunnel = false;
                    continue;
                }
            }
            ++format;
        }
        if (*format == '*')
        {
            int w = _extract_width(item);
            if (w<0)
            {
                out << std::left;
                w = -w;
            }
            out << std::setw(w);
            return _print_precision(++format, std::forward<Vs>(others)...);
        }
        else
        {
            int w = 0;
            for (; *format >= '0' && *format <= '9'; ++format)
            {
                w = w*10 + (*format-'0');
            }
            if (w!=0) out << std::setw(w);
        }
        return _print_precision(format, std::forward<T>(item), std::forward<Vs>(others)...);
    }
    
    template <typename T, typename... Vs>
    void _print_precision(const char* format, T&& item, Vs&&... others)
    {
        if (*format == '.')
        {
            ++format;
            bool chunnel = true;
            while (chunnel)
            {
                switch(*format)
                {
                    case 'e':
                    case 'E':
                    {
                        out << std::scientific;
                    break;}
                    case 'f':
                    case 'F':
                    {
                        out << std::fixed;
                    break;}
                    default:
                    {
                        chunnel = false;
                        continue;
                    }
                }
                ++format;
            }
            if (*format == '*')
            {
                int w = _extract_precision(item);
                if (w<0)
                {
                    out << std::left;
                    w = -w;
                }
                out << std::setprecision(w);
                return _print_spanner(++format, std::forward<Vs>(others)...);
            }
            else
            {
                int w = 0;
                for (; *format >= '0' && *format <= '9'; ++format)
                {
                    w = w*10 + (*format-'0');
                }
                if (w!=0) out << std::setprecision(w);
            }
        }
        return _print_spanner(format, std::forward<T>(item), std::forward<Vs>(others)...);
    }
    
    template <typename T, typename... Vs>
    void _print_spanner(const char* format, T&& item, Vs&&... others)
    {
        if (*format != SPANNER) throw std::runtime_error("print(): Invalid format spec.");
        out << item;
        out.copyfmt(saveState);
        return _print(++format, std::forward<Vs>(others)...);
    }
    
    void _print_width(const char* format)
    {
        throw std::runtime_error("print(): Missing parameters.");
    }
    
    void _print_precision(const char* format)
    {
        throw std::runtime_error("print(): Missing parameters.");
    }
    
    void _print_spanner(const char* format)
    {
        throw std::runtime_error("print(): Missing parameters.");
    }
    
    template <typename T>
    typename std::enable_if<
        std::is_assignable<int&, T>::value
        , int
    >::type _extract_width(T&& t)
    {
        return int(t);
    }

    template <typename T>
    typename std::enable_if<
        !std::is_assignable<int&, T>::value
        , int
    >::type _extract_width(T&& t)
    {
        throw std::runtime_error("print(): Width value must cast to int.");
    }
    
    template <typename T>
    typename std::enable_if<
        std::is_assignable<int&, T>::value
        , int
    >::type _extract_precision(T&& t)
    {
        return int(t);
    }

    template <typename T>
    typename std::enable_if<
        !std::is_assignable<int&, T>::value
        , int
    >::type _extract_precision(T&& t)
    {
        throw std::runtime_error("print(): Precision value must cast to int.");
    }
};

template <typename... Ts>
void print(const char* format, Ts&&... params)
{
    _PrintDetail<decltype(std::cout)> printer(std::cout);
    return printer._print(format, std::forward<Ts>(params)...);
}

template <typename T, typename... Ts>
void print(T&& out, const char* format, Ts&&... params)
{
    _PrintDetail<T> printer(std::forward<T>(out));
    return printer._print(format, std::forward<Ts>(params)...);
}
