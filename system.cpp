#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <algorithm>

using namespace std;

struct User {
    string username;
    string password;
    vector<string> bookingHistory;
};

struct Movie {
    string title;
    map<string, int> timings; // Maps timing to available seats
    double price;
    map<string, vector<string>> seatMap; // Maps timing to available seats (A1, B2, etc.)
};

vector<User> users;
vector<Movie> movies;

void initializeMovies();
void adminPanel();
void userMenu(User& user);
void displayMovies();
int selectMovie();
string selectTiming(int movieIndex);
vector<string> selectSeats(int availableSeats, string timing, int movieIndex);
void printReceipt(Movie& movie, string timing, vector<string>& seats, double totalCost, User& user);
void viewBookingHistory(User& user);
void registerUser();
User* loginUser();
void addMovie();
void removeMovie();
void viewAllBookings();

int main() {
    initializeMovies(); // Add predefined movies to the system
    char choice;
    cout << "Welcome to the Movie Ticket Booking System!\n";
    while (true) {
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Admin Panel\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case '1': {
                User* user = loginUser();
                if (user) {
                    userMenu(*user);
                }
                break;
            }
            case '2':
                registerUser();
                break;
            case '3':
                adminPanel();
                break;
            case '4':
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

void initializeMovies() {
    Movie movie1;
    movie1.title = "Inception";
    movie1.price = 10.00;
    movie1.timings["10:00"] = 50;
    movie1.timings["14:00"] = 50;
    movie1.timings["18:00"] = 50;
    for (char row = 'A'; row < 'A' + 5; row++) { // Assuming 50 seats, 5 rows with 10 seats each
        for (int num = 1; num <= 10; num++) {
            movie1.seatMap["10:00"].push_back(string(1, row) + to_string(num));
            movie1.seatMap["14:00"].push_back(string(1, row) + to_string(num));
            movie1.seatMap["18:00"].push_back(string(1, row) + to_string(num));
        }
    }
    movies.push_back(movie1);

    Movie movie2;
    movie2.title = "The Dark Knight";
    movie2.price = 12.00;
    movie2.timings["11:00"] = 50;
    movie2.timings["15:00"] = 50;
    movie2.timings["19:00"] = 50;
    for (char row = 'A'; row < 'A' + 5; row++) {
        for (int num = 1; num <= 10; num++) {
            movie2.seatMap["11:00"].push_back(string(1, row) + to_string(num));
            movie2.seatMap["15:00"].push_back(string(1, row) + to_string(num));
            movie2.seatMap["19:00"].push_back(string(1, row) + to_string(num));
        }
    }
    movies.push_back(movie2);

    Movie movie3;
    movie3.title = "Interstellar";
    movie3.price = 15.00;
    movie3.timings["12:00"] = 50;
    movie3.timings["16:00"] = 50;
    movie3.timings["20:00"] = 50;
    for (char row = 'A'; row < 'A' + 5; row++) {
        for (int num = 1; num <= 10; num++) {
            movie3.seatMap["12:00"].push_back(string(1, row) + to_string(num));
            movie3.seatMap["16:00"].push_back(string(1, row) + to_string(num));
            movie3.seatMap["20:00"].push_back(string(1, row) + to_string(num));
        }
    }
    movies.push_back(movie3);

    cout << "Movies have been initialized.\n";
}

void adminPanel() {
    string adminUsername = "admin";
    string adminPassword = "password";
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;
    if (username == adminUsername && password == adminPassword) {
        char choice;
        while (true) {
            cout << "\nAdmin Panel:\n";
            cout << "1. Add Movie\n";
            cout << "2. Remove Movie\n";
            cout << "3. View All Bookings\n";
            cout << "4. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case '1':
                    addMovie();
                    break;
                case '2':
                    removeMovie();
                    break;
                case '3':
                    viewAllBookings();
                    break;
                case '4':
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    } else {
        cout << "Invalid admin credentials!\n";
    }
}

void userMenu(User& user) {
    char choice;
    while (true) {
        cout << "\nUser Menu:\n";
        cout << "1. Book a Ticket\n";
        cout << "2. View Booking History\n";
        cout << "3. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case '1': {
                displayMovies();
                int movieIndex = selectMovie();
                string timing = selectTiming(movieIndex);
                vector<string> seats = selectSeats(movies[movieIndex].timings[timing], timing, movieIndex);
                double totalCost = seats.size() * movies[movieIndex].price;
                printReceipt(movies[movieIndex], timing, seats, totalCost, user);
                break;
            }
            case '2':
                viewBookingHistory(user);
                break;
            case '3':
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

void displayMovies() {
    cout << "Available Movies:\n";
    for (size_t i = 0; i < movies.size(); i++) {
        cout << i + 1 << ". " << movies[i].title << " - $" 
             << fixed << setprecision(2) << movies[i].price << endl;
        cout << "   Timings: ";
        for (const auto& timing : movies[i].timings) {
            cout << timing.first << " (" << timing.second << " seats available) ";
        }
        cout << endl;
    }
}

int selectMovie() {
    int choice;
    cout << "Enter the number of the movie you want to book: ";
    cin >> choice;
    while (choice < 1 || choice > movies.size()) {
        cout << "Invalid choice. Please select a valid movie number: ";
        cin >> choice;
    }
    return choice - 1;
}

string selectTiming(int movieIndex) {
    string timing;
    cout << "Enter the timing (HH:MM) you want to book: ";
    cin >> timing;
    while (movies[movieIndex].timings.find(timing) == movies[movieIndex].timings.end()) {
        cout << "Invalid timing. Please enter a valid timing: ";
        cin >> timing;
    }
    return timing;
}

vector<string> selectSeats(int availableSeats, string timing, int movieIndex) {
    int seats;
    vector<string> selectedSeats;
    string seat;
    cout << "Enter the number of seats you want to book: ";
    cin >> seats;
    while (seats < 1 || seats > availableSeats) {
        cout << "Invalid number of seats. Please enter a valid number: ";
        cin >> seats;
    }
    cout << "Available seats: ";
    for (const string& s : movies[movieIndex].seatMap[timing]) {
        cout << s << " ";
    }
    cout << "\nSelect your seats (e.g., A1 B2): ";
    for (int i = 0; i < seats; i++) {
        cin >> seat;
        while (find(movies[movieIndex].seatMap[timing].begin(), movies[movieIndex].seatMap[timing].end(), seat) == movies[movieIndex].seatMap[timing].end()) {
            cout << "Seat not available. Select a different seat: ";
            cin >> seat;
        }
        selectedSeats.push_back(seat);
        movies[movieIndex].seatMap[timing].erase(remove(movies[movieIndex].seatMap[timing].begin(), movies[movieIndex].seatMap[timing].end(), seat), movies[movieIndex].seatMap[timing].end());
        movies[movieIndex].timings[timing]--; // Reduce available seat count
    }
    return selectedSeats;
}

void printReceipt(Movie& movie, string timing, vector<string>& seats, double totalCost, User& user) {
    cout << "\nBooking Confirmation:\n";
    cout << "Movie: " << movie.title << endl;
    cout << "Timing: " << timing << endl;
    cout << "Seats: ";
    for (const string& seat : seats) {
        cout << seat << " ";
    }
    cout << "\nTotal Cost: $" << fixed << setprecision(2) << totalCost << endl;
    cout << "Thank you for booking with us!\n";

    string booking = movie.title + " | " + timing + " | Seats: ";
    for (const string& seat : seats) {
        booking += seat + " ";
    }
    user.bookingHistory.push_back(booking);
}

void viewBookingHistory(User& user) {
    cout << "\nBooking History:\n";
    for (const string& booking : user.bookingHistory) {
        cout << booking << endl;
    }
}

void registerUser() {
    User newUser;
    cout << "Enter a username: ";
    cin >> newUser.username;
    cout << "Enter a password: ";
    cin >> newUser.password;
    users.push_back(newUser);
    cout << "Registration successful! You can now log in.\n";
}

User* loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    for (User& user : users) {
        if (user.username == username && user.password == password) {
            cout << "Login successful!\n";
            return &user;
        }
    }
    cout << "Invalid username or password!\n";
    return nullptr;
}

void addMovie() {
    Movie newMovie;
    cout << "Enter the movie title: ";
    cin.ignore();
    getline(cin, newMovie.title);
    cout << "Enter the price per ticket: ";
    cin >> newMovie.price;
    string timing;
    int seats;
    char addMore;
    do {
        cout << "Enter timing (HH:MM): ";
        cin >> timing;
        cout << "Enter number of seats: ";
        cin >> seats;
        newMovie.timings[timing] = seats;
        for (char row = 'A'; row < 'A' + (seats / 10); row++) { // Adjust number of rows based on seat count
            for (int num = 1; num <= 10; num++) {
                newMovie.seatMap[timing].push_back(string(1, row) + to_string(num));
            }
        }
        cout << "Do you want to add another timing? (y/n): ";
        cin >> addMore;
    } while (addMore == 'y' || addMore == 'Y');
    movies.push_back(newMovie);
    cout << "Movie added successfully!\n";
}

void removeMovie() {
    displayMovies();
    int movieIndex = selectMovie();
    movies.erase(movies.begin() + movieIndex);
    cout << "Movie removed successfully!\n";
}

void viewAllBookings() {
    cout << "\nAll Bookings:\n";
    for (const User& user : users) {
        cout << "User: " << user.username << endl;
        viewBookingHistory(const_cast<User&>(user)); // const_cast to call a non-const member function
        cout << endl;
    }
}