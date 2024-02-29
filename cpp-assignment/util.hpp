#include <iostream>
#include <limits>
#include <stddef.h>

inline void finish(Consumer* consumer) {
    std::cout << "Press enter to continue...";
    std::cin.get();
    std::cin.get();

    system("clear");
    std::cout << "###############################################" << std::endl;
    std::cout << "##                                           ##" << std::endl;
    std::cout << "##        CAR RENTAL MANAGEMENT SYSTEM       ##" << std::endl;
    std::cout << "##                                           ##" << std::endl;
    std::cout << "###############################################" << std::endl;
    std::cout << std::endl;
    if (consumer != nullptr) {
        std::cout << consumer->display_welcome() << std::endl;
    }
}

inline bool is_date_valid(int day, int month, int year) {
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    if (month == 2) {
        if (year % 4 == 0) {
            if (day > 29) {
                return false;
            }
        } else {
            if (day > 28) {
                return false;
            }
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return false;
        }
    } else {
        if (day > 31) {
            return false;
        }
    }

    return true;
}

// credits: https://howardhinnant.github.io/date_algorithms.html
template <class Int>
constexpr Int days_from_civil(Int y, unsigned m, unsigned d) noexcept {
    static_assert(
        std::numeric_limits<unsigned>::digits >= 18,
        "This algorithm has not been ported to a 16 bit unsigned integer");
    static_assert(
        std::numeric_limits<Int>::digits >= 20,
        "This algorithm has not been ported to a 16 bit signed integer");
    y -= m <= 2;
    const Int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400); // [0, 399]
    const unsigned doy =
        (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;        // [0, 365]
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0, 146096]
    return era * 146097 + static_cast<Int>(doe) - 719468;
}
