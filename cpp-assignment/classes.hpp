#include <fstream>
#include <string>
#include <vector>

const std::string CONSUMER_FILENAME = "consumers.txt";
const std::string CAR_FILENAME = "cars.txt";

class User;

class Car {
    int id;
    std::string model;
    std::string number;
    float cost;
    int owner = -1;
    int condition = 100;

    int start_day = -1;
    int start_month = -1;
    int start_year = -1;
    int end_day = -1;
    int end_month = -1;
    int end_year = -1;

  public:
    Car();
    Car(int id, std::string model, std::string number, float cost);
    static void create_new_car();
    static std::vector<Car> get_available_cars();
    static std::vector<Car> get_all_cars();
    static void remove_car();
    static void update_car();

    const bool is_owner(int id);
    const bool is_rented();

    bool rent(int owner, int start_day, int start_month, int start_year, int end_day, int end_month, int end_year);
    bool return_car(int condition);

    float get_cost();
    std::string display(int mode);
    int get_condition();
    int get_start_day();
    int get_start_month();
    int get_start_year();
    int get_end_day();
    int get_end_month();
    int get_end_year();

    std::string get_start_date();
    std::string get_due_date();
    std::string get_owner_name();
    int get_days_rented();

    bool operator==(const Car &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Car &car);
    friend std::istream &operator>>(std::istream &is, Car &car);
};

class User {
  protected:
    int id;
    std::string name;
    std::string password;
};

class Consumer : User {
    float fine_due = 0;
    int record = 100; // employee record or consumer record
    int type;         // 0 for employees, 1 for consumers

  public:
    Consumer();
    Consumer(int id, std::string name, std::string password, int type);
    static void create_new_user();
    static std::vector<Consumer> get_all_users();
    static void remove_user();
    static void update_user();
    static Consumer login(std::string name, std::string password);

    std::vector<Car> get_rented_cars();
    void clear_due();
    void update_dues(float dues);
    void update_record(int change);

    int get_id();
    float get_fine_due();
    int get_max_capacity();
    float get_discount();
    int get_record();
    std::string get_name();
    std::string display_welcome();

    bool operator==(const Consumer &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Consumer &car);
    friend std::istream &operator>>(std::istream &is, Consumer &car);
};
