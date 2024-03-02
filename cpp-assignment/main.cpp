#include "classes.hpp"
#include "database.hpp"
#include "util.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

Consumer login() {
    std::string name;
    std::string password;
    std::cout << "Enter your name: ";
    std::cin >> name;
    password = get_password();
    try {
        return Consumer::login(name, password);
    } catch (std::invalid_argument &e) {
        throw e;
    }
}

void rent_car(Consumer consumer) {
    int max_rental = consumer.get_max_capacity();
    if (consumer.get_rented_cars().size() >= max_rental) {
        std::cout << "You have reached the maximum rental capacity"
                  << std::endl;
        return;
    }

    std::vector<Car> cars = Car::get_available_cars();
    if (cars.size() == 0) {
        std::cout << "No cars available for rent!" << std::endl;
        return;
    }

    if (consumer.get_discount() > 0) {
        std::cout << "An additional discount of "
                  << consumer.get_discount() * 100
                  << "% will be given because you are an employee!"
                  << std::endl;
    }

    std::cout << "Available cars: " << std::endl;
    std::cout << std::setw(20) << std::left << "S.No" << std::setw(20)
              << std::left << "Model" << std::setw(20) << std::left << "Number"
              << std::setw(20) << std::left << "Cost" << std::setw(20)
              << std::left << "Condition" << std::endl;
    for (int i = 0; i < cars.size(); i++) {
        std::cout << std::setw(20) << std::left << i + 1;
        cars[i].display(0);
        std::cout << std::endl;
    }

    std::cout << "Enter the serial number of the car you want to rent: ";
    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > cars.size()) {
        std::cout << "Invalid choice" << std::endl;
        return;
    }

    int start_day, start_month, start_year;
    do {
        std::cout << "Enter the day of month you want to rent the car: ";
        std::cin >> start_day;
        std::cout << "Enter the month you want to rent the car: ";
        std::cin >> start_month;
        std::cout << "Enter the year you want to rent the car: ";
        std::cin >> start_year;
        if (!is_date_valid(start_day, start_month, start_year)) {
            std::cout << "Invalid date!" << std::endl;
        }
    } while (!is_date_valid(start_day, start_month, start_year));

    int days = -1;
    int end_day, end_month, end_year;
    while (days <= 0) {
        do {
            std::cout << "Enter the day of month you want to return the car: ";
            std::cin >> end_day;
            std::cout << "Enter the month you want to return the car: ";
            std::cin >> end_month;
            std::cout << "Enter the year you want to return the car: ";
            std::cin >> end_year;
            if (!is_date_valid(end_day, end_month, end_year)) {
                std::cout << "Invalid date!" << std::endl;
            }
        } while (!is_date_valid(end_day, end_month, end_year));

        days = days_from_civil(end_year, end_month, end_day) -
               days_from_civil(start_year, start_month, start_day);
        if (days <= 0) {
            std::cout << "Invalid date of return, your return date should be "
                         "after the date of renting"
                      << std::endl;
        }
    }

    std::cout << "The cost of renting the car for " << days << " days is "
              << cars[choice - 1].get_cost() * (1 - consumer.get_discount()) *
                     days
              << std::endl;
    std::cout << "Do you want to continue? (y/n): ";
    char c;
    std::cin >> c;
    if (c != 'y') {
        return;
    }

    consumer.update_dues(cars[choice - 1].get_cost() *
                         (1 - consumer.get_discount()) * days);
    cars[choice - 1].rent(consumer.get_id(), start_day, start_month, start_year,
                          end_day, end_month, end_year);
    std::cout << "Car rented successfully!" << std::endl;
}

void return_car(Consumer consumer) {
    std::vector<Car> cars = consumer.get_rented_cars();
    if (cars.size() == 0) {
        std::cout << "You have not rented any cars!" << std::endl;
        return;
    }

    std::cout << "Rented cars: " << std::endl;
    std::cout << std::setw(20) << std::left << "S.No" << std::setw(20)
              << std::left << "Model" << std::setw(20) << std::left << "Number"
              << std::setw(20) << std::left << "Start Date" << std::setw(20)
              << std::left << "End Date" << std::endl;
    for (int i = 0; i < cars.size(); i++) {
        std::cout << std::setw(20) << std::left << i + 1;
        cars[i].display(1);
        std::cout << std::endl;
    }

    std::cout << "Enter the serial number of the car you want to return: ";
    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > cars.size()) {
        std::cout << "Invalid choice" << std::endl;
        return;
    }

    std::cout << "Do you want to return the car? (y/n): ";
    char c;
    std::cin >> c;
    if (c != 'y') {
        return;
    }

    std::cout << "What is the condition of the car (0-100): ";
    int condition;
    std::cin >> condition;
    if (condition < 0 || condition > 100) {
        std::cout << "Invalid condition" << std::endl;
        return;
    }

    float fine = 0;
    int record_fine = 0;

    int initial_condition = cars[choice - 1].get_condition();
    if (condition < initial_condition) {
        std::cout
            << "The condition of the car is " << initial_condition - condition
            << " worse than when you rented it. You will have to pay a fine of "
            << ((float)initial_condition - (float)condition) *
                   cars[choice - 1].get_cost() * 0.01
            << std::endl;
        fine += ((float)initial_condition - (float)condition) *
                cars[choice - 1].get_cost() * 0.01;
        record_fine -= 15;
    }

    int end_day, end_month, end_year;
    do {
        std::cout << "Enter the day of month of return: ";
        std::cin >> end_day;
        std::cout << "Enter the month of return: ";
        std::cin >> end_month;
        std::cout << "Enter the year of return: ";
        std::cin >> end_year;
        if (!is_date_valid(end_day, end_month, end_year)) {
            std::cout << "Invalid date!" << std::endl;
        }
    } while (!is_date_valid(end_day, end_month, end_year));

    int days = -1;
    while (days < 0) {
        days = days_from_civil(end_year, end_month, end_day) -
               days_from_civil(cars[choice - 1].get_start_year(),
                               cars[choice - 1].get_start_month(),
                               cars[choice - 1].get_start_day());
        if (days < 0) {
            std::cout << "Invalid date of return, your return date should be "
                         "after the date of renting"
                      << std::endl;
            std::cout << "Your start date was "
                      << cars[choice - 1].get_start_day() << "/"
                      << cars[choice - 1].get_start_month() << "/"
                      << cars[choice - 1].get_start_year() << std::endl;

            do {
                std::cout << "Enter the day of month of return: ";
                std::cin >> end_day;
                std::cout << "Enter the month of return: ";
                std::cin >> end_month;
                std::cout << "Enter the year of return: ";
                std::cin >> end_year;
                if (!is_date_valid(end_day, end_month, end_year)) {
                    std::cout << "Invalid date!" << std::endl;
                }
            } while (!is_date_valid(end_day, end_month, end_year));
        }
    }

    int initial_days = cars[choice - 1].get_days_rented();
    if (days > initial_days) {
        std::cout << "You have used the car for " << days - initial_days
                  << " more days than originally planned for. You will have to "
                     "pay a fine of "
                  << ((float)days - (float)initial_days) *
                         cars[choice - 1].get_cost() * 0.01
                  << std::endl;
        fine += ((float)days - (float)initial_days) *
                cars[choice - 1].get_cost() * 0.01;
        record_fine -= 5;
    }

    if (record_fine < 0) {
        std::cout << "You have been fined " << -record_fine
                  << " points for returning the car in bad condition or "
                     "returning it late"
                  << std::endl;
        std::cout << "This will be added to your record" << std::endl;
    }

    std::cout << "Do you want to continue? (y/n): ";
    std::cin >> c;
    if (c != 'y') {
        return;
    }

    consumer.update_dues(fine);
    consumer.update_record(record_fine);
    cars[choice - 1].return_car(condition);
    std::cout << "Car returned successfully!" << std::endl;
}

void clear_dues(Consumer consumer) {
    if (consumer.get_fine_due() == 0) {
        std::cout << "You do not have any dues!" << std::endl;
        return;
    }
    std::cout << "Your due is " << std::fixed << std::setprecision(2)
              << consumer.get_fine_due() << std::endl;
    std::cout << "Do you want to clear your due? (y/n): ";
    char c;
    std::cin >> c;
    if (c != 'y') {
        return;
    }

    consumer.clear_due();
    std::cout << "Due cleared successfully!" << std::endl;
}

void view_rented_cars(Consumer consumer) {
    std::vector<Car> cars = consumer.get_rented_cars();
    if (cars.size() == 0) {
        std::cout << "You have not rented any cars!" << std::endl;
        return;
    }

    std::cout << std::setw(20) << std::left << "S.No" << std::setw(20)
              << std::left << "Model" << std::setw(20) << std::left << "Number"
              << std::setw(20) << std::left << "Start Date" << std::setw(20)
              << std::left << "End Date" << std::endl;
    for (int i = 0; i < cars.size(); i++) {
        std::cout << std::setw(20) << std::left << i + 1;
        cars[i].display(1);
        std::cout << std::endl;
    }
}

void post_user_login(Consumer consumer) {
    while (true) {
        std::cout
            << "Choose:\n0 for renting a car\n1 for returning a car\n2 for "
               "clearing due\n3 for viewing rented cars\n4 for logging out\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 0:
            rent_car(consumer);
            break;
        case 1:
            return_car(consumer);
            break;
        case 2:
            clear_dues(consumer);
            break;
        case 3:
            view_rented_cars(consumer);
            break;
        case 4:
            std::cout << "Logged out successfully!" << std::endl;
            return;
        default:
            std::cout << "Invalid choice" << std::endl;
        }
        finish(&consumer);
    }
}

void post_manager_login() {
    while (true) {
        // std::cout
        //     << "Enter 0 for adding a new car, 1 for removing a car, 2 for "
        //        "updating a car, 3 for viewing all cars, 4 for logging out: ";
        std::cout
            << "Choose:\n0 for adding a new car\n1 for removing a car\n2 for "
               "updating a car\n3 for viewing all cars\n4 for adding a new "
               "user\n5 for removing a user\n6 for updating a user\n7 for "
               "viewing all users\n8 for logging out\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 0:
            Manager::add_car();
            break;
        case 1:
            Manager::remove_car();
            break;
        case 2:
            Manager::update_car();
            break;
        case 3: {
            Manager::display_cars();
            break;
        }
        case 4: {
            Manager::add_user();
            break;
        }
        case 5: {
            Manager::remove_user();
            break;
        }
        case 6: {
            Manager::update_user();
            break;
        }
        case 7: {
            Manager::display_users();
            break;
        }
        case 8:
            std::cout << "Logged out successfully!" << std::endl;
            finish(nullptr);
            return;
        default:
            std::cout << "Invalid choice" << std::endl;
        }
        finish(nullptr);
    }
}

int main() {
    system("clear");
    std::cout << "###############################################" << std::endl;
    std::cout << "##                                           ##" << std::endl;
    std::cout << "##        CAR RENTAL MANAGEMENT SYSTEM       ##" << std::endl;
    std::cout << "##                                           ##" << std::endl;
    std::cout << "###############################################" << std::endl;
    std::cout << std::endl;

    while (true) {
        std::cout << "Choose:\n0 for registering a new user\n1 for user "
                     "login\n2 for manager login\n3 for exit\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input" << std::endl;
            continue;
        }

        switch (choice) {
        case 0:
            Consumer::create_new_user();
            finish(nullptr);
            break;
        case 1: {
            try {
                Consumer consumer = login();
                std::cout << "Logged in successfully!" << std::endl;
                finish(&consumer);
                post_user_login(consumer);
            } catch (std::invalid_argument &e) {
                std::cout << e.what() << std::endl;
            }
            finish(nullptr);
            break;
        }
        case 2: {
            std::string username, password;
            std::cout << "Enter username: ";
            std::cin >> username;
            password = get_password();
            if (username == Manager::get_username() && password == Manager::get_password()) {
                std::cout << "Logged in successfully!" << std::endl;
                finish(nullptr);
                post_manager_login();
            } else {
                std::cout << "Invalid username or password" << std::endl;
                finish(nullptr);
            }
            break;
        }
        case 3:
            std::cout << "Exiting..." << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice" << std::endl;
            finish(nullptr);
        }
    }

    return 0;
}