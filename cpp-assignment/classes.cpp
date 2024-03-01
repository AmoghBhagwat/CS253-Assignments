#include "classes.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "database.hpp"
#include "util.hpp"

Car::Car() {
    this->id = -1;
    this->model = "";
}

Car::Car(int id, std::string model, std::string number, float cost) {
    this->id = id;
    this->model = model;
    this->number = number;
    this->cost = cost;
    this->start_day = -1;
    this->start_month = -1;
    this->start_year = -1;
}

void Consumer::clear_due() {
    Consumer temp = *this;

    temp.fine_due = 0;
    temp.record = 100;

    Database<Consumer> consumer_db(CONSUMER_FILENAME);
    consumer_db.update(*this, temp);
    *this = temp;
}

float Consumer::get_fine_due() {
    Database<Consumer> consumer_db(CONSUMER_FILENAME);
    std::vector<Consumer> consumers = consumer_db.load_all();

    for (Consumer &consumer : consumers) {
        if (consumer.id == this->id) {
            return consumer.fine_due;
        }
    }

    return 0;
}

bool Consumer::operator==(const Consumer &other) const {
    return id == other.id;
}

void Car::create_new_car() {
    Database<Car> db(CAR_FILENAME);
    std::vector<Car> cars = db.load_all();

    std::string model, number;
    float cost;
    int id = cars.size() + 1;

    std::cout << "Enter the model of the car: ";
    std::cin >> model;
    std::cout << "Enter the number of the car: ";
    std::cin >> number;
    std::cout << "Enter the daily cost of renting the car: ";
    std::cin >> cost;

    Car car(id, model, number, cost);
    db.insert(car);

    std::cout << "Car added successfully!" << std::endl;
}

std::vector<Car> Car::get_available_cars() {
    Database<Car> car_db(CAR_FILENAME);
    std::vector<Car> cars = car_db.load_all();
    std::vector<Car> available_cars;

    for (Car &car : cars) {
        if (!car.is_rented()) {
            available_cars.push_back(car);
        }
    }

    return available_cars;
}

std::vector<Car> Car::get_all_cars() {
    Database<Car> car_db(CAR_FILENAME);
    return car_db.load_all();
}

void Car::remove_car() {
    std::vector<Car> cars = Car::get_all_cars();
    if (cars.size() == 0) {
        std::cout << "No cars available to remove" << std::endl;
        return;
    }

    std::cout << "Available cars: " << std::endl;
    std::cout << std::setw(20) << std::left << "S.No" << std::setw(20)
              << std::left << "Model" << std::setw(20) << std::left << "Number"
              << std::setw(20) << std::left << "Cost" << std::setw(20)
              << std::left << "Condition" << std::setw(20) << std::left
              << "Start Date" << std::setw(20) << std::left << "End Date"
              << std::setw(20) << std::left << "Owner" << std::endl;
    for (int i = 0; i < cars.size(); i++) {
        std::cout << std::setw(20) << std::left << i + 1;
        cars[i].display(2);
        std::cout << std::endl;
    }

    std::cout << "Enter the number of the car you want to remove: ";
    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > cars.size()) {
        std::cout << "Invalid choice" << std::endl;
        return;
    }

    if (cars[choice - 1].is_rented()) {
        std::cout << "Car is currently rented, cannot remove!" << std::endl;
        return;
    }

    std::cout << "Are you sure you want to remove the car? (y/n): ";
    char c;
    std::cin >> c;
    if (c != 'y') {
        return;
    }

    Database<Car> car_db(CAR_FILENAME);
    car_db.remove(cars[choice - 1]);

    std::cout << "Car removed successfully!" << std::endl;
}

void Car::update_car() {
    std::vector<Car> cars = Car::get_available_cars();
    if (cars.size() == 0) {
        std::cout << "No cars available to update" << std::endl;
        return;
    }

    std::cout << "Available cars: " << std::endl;
    std::cout << std::setw(20) << std::left << "S.No" << std::setw(20)
              << std::left << "Model" << std::setw(20) << std::left << "Number"
              << std::setw(20) << std::left << "Cost" << std::setw(20)
              << std::left << "Condition" << std::setw(20) << std::left
              << "Start Date" << std::setw(20) << std::left << "End Date"
              << std::setw(20) << std::left << "Owner" << std::endl;
    for (int i = 0; i < cars.size(); i++) {
        std::cout << std::setw(20) << std::left << i + 1;
        cars[i].display(2);
        std::cout << std::endl;
    }

    std::cout << "Enter the number of the car you want to update: ";
    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > cars.size()) {
        std::cout << "Invalid choice" << std::endl;
        return;
    }

    std::cout << "Enter the new model of the car: ";
    std::string model;
    std::cin >> model;
    std::cout << "Enter the new number of the car: ";
    std::string number;
    std::cin >> number;
    std::cout << "Enter the new daily cost of renting the car: ";
    float cost;
    std::cin >> cost;
    std::cout << "Enter the condition of the car (0-100): ";
    int condition;
    std::cin >> condition;

    Car temp = cars[choice - 1];
    temp.model = model;
    temp.number = number;
    temp.cost = cost;
    temp.condition = condition;

    Database<Car> car_db(CAR_FILENAME);
    car_db.update(cars[choice - 1], temp);

    std::cout << "Car updated successfully!" << std::endl;
}

bool Car::operator==(const Car &other) const { return id == other.id; }

const bool Car::is_owner(const int id) { return (this->owner == id); }

const bool Car::is_rented() { return (this->owner != -1); }

bool Car::rent(int owner, int start_day, int start_month, int start_year,
               int end_day, int end_month, int end_year) {
    if (is_rented())
        return false;
    Car temp = *this;
    temp.owner = owner;
    temp.start_day = start_day;
    temp.start_month = start_month;
    temp.start_year = start_year;
    temp.end_day = end_day;
    temp.end_month = end_month;
    temp.end_year = end_year;

    Database<Car> car_db(CAR_FILENAME);
    car_db.update(*this, temp);

    return true;
}

bool Car::return_car(int condition) {
    if (!is_rented())
        return false;

    Car temp = *this;
    temp.owner = -1;
    temp.start_day = -1;
    temp.start_month = -1;
    temp.start_year = -1;
    temp.end_day = -1;
    temp.end_month = -1;
    temp.end_year = -1;
    temp.condition = condition;

    Database<Car> car_db(CAR_FILENAME);
    car_db.update(*this, temp);
    return true;
}

float Car::get_cost() { return this->cost; }

void Car::display(
    int mode) { // mode 0 for booking, 1 for returning, 2 for admin
    if (mode == 0) {
        std::cout << std::setw(20) << std::left << this->model << std::setw(20)
                  << std::left << this->number << std::setw(20) << std::left
                  << this->cost << std::setw(20) << std::left
                  << this->condition;
    }
    if (mode == 1) {
        std::cout << std::setw(20) << std::left << this->model << std::setw(20)
                  << std::left << this->number << std::setw(20) << std::left
                  << this->get_start_date() << std::setw(20) << std::left
                  << this->get_end_date();
    }
    if (mode == 2) {
        std::cout << std::setw(20) << std::left << this->model << std::setw(20)
                  << std::left << this->number << std::setw(20) << std::left
                  << this->cost << std::setw(20) << std::left << this->condition
                  << std::setw(20) << std::left << this->get_start_date()
                  << std::setw(20) << std::left << this->get_end_date()
                  << std::setw(20) << std::left << this->get_owner_name();
    }
}

int Car::get_condition() { return this->condition; }

int Car::get_start_day() { return this->start_day; }

int Car::get_start_month() { return this->start_month; }

int Car::get_start_year() { return this->start_year; }

int Car::get_end_day() { return this->end_day; }

int Car::get_end_month() { return this->end_month; }

int Car::get_end_year() { return this->end_year; }

std::string Car::get_start_date() {
    if (this->start_day == -1)
        return "Not rented";
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << this->start_day << '/'
       << std::setfill('0') << std::setw(2) << this->start_month << '/'
       << std::setfill('0') << std::setw(4) << this->start_year;
    return ss.str();
}

std::string Car::get_end_date() {
    if (this->end_day == -1)
        return "Not rented";
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << this->end_day << '/'
       << std::setfill('0') << std::setw(2) << this->end_month << '/'
       << std::setfill('0') << std::setw(4) << this->end_year;
    return ss.str();
}

std::string Car::get_owner_name() {
    Database<Consumer> consumer_db(CONSUMER_FILENAME);
    std::vector<Consumer> consumers = consumer_db.load_all();

    for (Consumer &consumer : consumers) {
        if (consumer.get_id() == this->owner) {
            return consumer.get_name();
        }
    }

    return "Not rented";
}

int Car::get_days_rented() {
    int start =
        days_from_civil(this->start_year, this->start_month, this->start_day);
    int end = days_from_civil(this->end_year, this->end_month, this->end_day);
    return end - start;
}

std::ostream &operator<<(std::ostream &os, const Car &car) {
    os << car.id << ' ' << car.model << ' ' << car.number << ' ' << car.cost
       << ' ' << car.condition << ' ' << car.owner << ' ' << car.start_day
       << ' ' << car.start_month << ' ' << car.start_year << ' ' << car.end_day
       << ' ' << car.end_month << ' ' << car.end_year;
    return os;
}

std::istream &operator>>(std::istream &is, Car &car) {
    is >> car.id >> car.model >> car.number >> car.cost >> car.condition >>
        car.owner >> car.start_day >> car.start_month >> car.start_year >>
        car.end_day >> car.end_month >> car.end_year;
    return is;
}

Consumer::Consumer() {
    this->id = -1;
    this->name = "";
    this->password = "";
    this->type = -1;
}

Consumer::Consumer(int id, std::string name, std::string password, int type) {
    this->id = id;
    this->name = name;
    this->password = password;
    this->type = type;
}

std::vector<Car> Consumer::get_rented_cars() {
    Database<Car> car_db(CAR_FILENAME);

    const int id = this->id;

    std::vector<Car> rented_cars =
        car_db.search([&](Car &car) { return car.is_owner(id); });

    return rented_cars;
}

void Consumer::create_new_user() {
    Database<Consumer> db(CONSUMER_FILENAME);
    std::vector<Consumer> consumers = db.load_all();

    std::string name;
    std::string password;
    int type;
    int id = consumers.size() + 1;

    std::cout << "Enter your name: ";
    while (true) {
        std::cin >> name;

        bool found = false;
        for (Consumer &consumer : consumers) {
            if (consumer.name == name) {
                std::cout << "Name already taken, please enter another name: ";
                found = true;
                break;
            }
        }
        if (!found)
            break;
    }

    password = get_password();
    std::cout << "Enter 0 if you are an employee, 1 if you are a customer: ";
    std::cin >> type;

    Consumer consumer(id, name, password, type);
    db.insert(consumer);

    std::cout << "Registered successfully!" << std::endl;
}

std::vector<Consumer> Consumer::get_all_users() {
    Database<Consumer> consumer_db(CONSUMER_FILENAME);
    return consumer_db.load_all();
}

void Consumer::remove_user() {
    Database<Consumer> db(CONSUMER_FILENAME);
    std::vector<Consumer> consumers = db.load_all();

    if (consumers.size() == 0) {
        std::cout << "No users available to remove" << std::endl;
        return;
    }

    std::cout << "Available users: " << std::endl;
    std::cout << std::setw(20) << std::left << "S.No" << std::setw(20) << std::left
              << "Name" << std::setw(20) << std::left << "Type" << std::setw(20)
              << std::left << "Record" << std::setw(20) << std::left << "Dues" << std::endl;
    for (int i = 0; i < consumers.size(); i++) {
        std::cout << std::setw(20) << std::left << i + 1;
        consumers[i].display();
    }

    std::cout << "Enter the number of the user you want to remove: ";
    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > consumers.size()) {
        std::cout << "Invalid choice" << std::endl;
        return;
    }

    if (consumers[choice - 1].get_rented_cars().size() > 0) {
        std::cout << "User has rented cars, cannot remove!" << std::endl;
        return;
    }

    std::cout << "Are you sure you want to remove the user? (y/n): ";
    char c;
    std::cin >> c;
    if (c != 'y') {
        return;
    }

    db.remove(consumers[choice - 1]);
    std::cout << "User removed successfully!" << std::endl;
}

void Consumer::update_user() {
    Database<Consumer> db(CONSUMER_FILENAME);
    std::vector<Consumer> consumers = db.load_all();

    if (consumers.size() == 0) {
        std::cout << "No users available to update" << std::endl;
        return;
    }

    std::cout << "Available users: " << std::endl;
    std::cout << std::setw(20) << std::left << "S.No" << std::setw(20) << std::left
              << "Name" << std::setw(20) << std::left << "Type" << std::setw(20)
              << std::left << "Record" << std::setw(20) << std::left << "Dues" << std::endl;
    for (int i = 0; i < consumers.size(); i++) {
        std::cout << std::setw(20) << std::left << i + 1;
        consumers[i].display();
    }

    std::cout << "Enter the number of the user you want to update: ";
    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > consumers.size()) {
        std::cout << "Invalid choice" << std::endl;
        return;
    }

    std::cout << "Enter the new name of the user: ";
    std::string name;
    std::cin >> name;
    std::string password = get_password();
    std::cout << "Enter the new record of the user: ";
    int record;
    std::cin >> record;

    Consumer temp = consumers[choice - 1];
    temp.name = name;
    temp.password = password;
    temp.record = record;

    db.update(consumers[choice - 1], temp);

    std::cout << "User updated successfully!" << std::endl;
}

Consumer Consumer::login(std::string name, std::string password) {
    Database<Consumer> db(CONSUMER_FILENAME);
    std::vector<Consumer> match = db.search([&](const Consumer &consumer) {
        return consumer.name == name && consumer.password == password;
    });

    if (match.size() > 0)
        return match[0];

    throw std::invalid_argument("Invalid username or password");
}

std::ostream &operator<<(std::ostream &os, const Consumer &consumer) {
    os << consumer.id << ' ' << consumer.name << ' ' << consumer.password << ' '
       << consumer.type << ' ' << consumer.record << ' ' << consumer.fine_due;
    return os;
}

std::istream &operator>>(std::istream &is, Consumer &consumer) {
    is >> consumer.id >> consumer.name >> consumer.password >> consumer.type >>
        consumer.record >> consumer.fine_due;
    return is;
}

int Consumer::get_id() { return this->id; }

int Consumer::get_max_capacity() {
    return this->record / 30; // TODO: change this to a better formula
}

void Consumer::update_dues(float dues) {
    Database<Consumer> consumer_db(CONSUMER_FILENAME);
    Consumer temp = consumer_db.search(
        [&](const Consumer &consumer) { return consumer.id == this->id; })[0];
    temp.fine_due += dues;

    consumer_db.update(*this, temp);
    *this = temp;
}

void Consumer::update_record(int change) {
    Database<Consumer> consumer_db(CONSUMER_FILENAME);
    Consumer temp = consumer_db.search(
        [&](const Consumer &consumer) { return consumer.id == this->id; })[0];
    temp.record += change;

    consumer_db.update(*this, temp);

    *this = temp;
}

float Consumer::get_discount() {
    if (this->type == 0) {
        return 0.15;
    } else {
        return 0;
    }
}

int Consumer::get_record() {
    Database<Consumer> consumer_db(CONSUMER_FILENAME);
    Consumer temp = consumer_db.search(
        [&](const Consumer &consumer) { return consumer.id == this->id; })[0];
    *this = temp;

    return this->record;
}

void Consumer::display() {
    std::string type = (this->type == 0) ? "Employee" : "Customer";
    std::cout << std::setw(20) << std::left << this->name << std::setw(20) << std::left << type << std::setw(20) << std::left << this->record << std::setw(20) << std::left << this->fine_due << std::endl;
}

std::string Consumer::get_name() { return this->name; }

std::string Consumer::display_welcome() {
    Database<Consumer> consumer_db(CONSUMER_FILENAME);
    Consumer temp = consumer_db.search(
        [&](const Consumer &consumer) { return consumer.id == this->id; })[0];
    *this = temp;

    std::string text = "\t\tWelcome, " + this->get_name() + '\n';
    text +=
        "\t\tCars rented: " + std::to_string(this->get_rented_cars().size()) +
        '\n';

    std::string type = (this->type == 0) ? "Employee" : "Customer";

    std::string cost;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << this->fine_due;
    cost = ss.str();

    text += "\t\tPayment due: " + cost + '\n';

    text += "\t\t" + type + " Record (0-100): " + std::to_string(this->record) +
            '\n';
    text +=
        "\t\tMax Cars Rentable: " + std::to_string(this->get_max_capacity()) +
        '\n';

    return text;
}
