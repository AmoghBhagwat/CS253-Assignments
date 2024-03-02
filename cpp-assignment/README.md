# Car Rental Management System
This repository contains my submission for the C++ Assignment for the course CS253, Spring 2024.

## Portability Warning
The code is written and tested on a Linux-based operating system and few functionalities are not guaranteed to work on other operating systems like Windows. A few examples where unexpected behaviour occurs are as follows.

- Clearing the screen might not work on Windows. To fix this, replace the line ```system("clear")``` by ```system("cls")``` in the ```finish()``` function in the file ```utils.hpp```
- Password input might not work, since the code needed for hiding the password is platform-dependent.

## Setup Instructions
Clone this repository using the command
```
git clone https://github.com/AmoghBhagwat/CS253-Assignments.git
```
Run the ```start.sh``` file using
```
sudo chmod +x start.sh
./start.sh
```

## Running the Program
The database is pre-populated with 5 employees, 5 customers and 5 cars initially. There is always only 1 manager in the system. The login details of the users provided initially is as follows.

| Name | Password | Type |
| ---- | -------- | ---- |
| Amogh | amogh123 | Customer |
| Srujan | srujan123 | Customer |
| Nevish | nevu123 | Customer |
| Harshit | hars123 | Customer |
| Anaswar | anaswar013 | Customer |
| Raghav | raghav123 | Employee |
| Ananya | ananya123 | Employee |
| Khushi | khushi123 | Employee |
| Srishti | srishti213 | Employee |
| Debraj | debraj123 | Employee |

Manager details - 
- Username - ```admin```
- Password - ```admin```

**NOTE: Entering the password will not display anything. This is the general convention. Press ENTER after entering the password and it will work as intended.**

## Design Decisions / Assumptions
- A car cannot be deleted if it is already rented by a user.
- A user cannot be deleted if they have rented one or more cars.
- The user will have to pay a fine if he/she returns the car in worse condition and/or if the car is returned after the end date. This will also affect the record of the user.
- The maximum number of cars a user can rent is determined by their record (```record / 30``` in this case).
- The record of the user is reset to 100 when all dues are cleared by them.

## Project Structure
- ```main.cpp``` consists of the main program, which includes managing the control flow and taking user input.
- ```classes.hpp``` and ```classes.cpp``` contain the definitions and implementations of the ```Consumer```, ```User``` and ```Car``` classes.
- ```database.hpp``` contains the implementation for handling the database. 
- ```consumers.txt``` and ```cars.txt``` contains the details of consumers and cars respectively. 

I am using a simple text-file based database, where each entry is stored in its row and the attributes are stored separated by spaces.