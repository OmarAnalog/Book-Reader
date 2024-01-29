#include <iostream>
#include <chrono>
#include<iomanip>
#include<ctime>
#include<sstream>
#include <vector>

using namespace std;
using namespace chrono;

string GetCurrentTimeDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

int ReadInt(int low, int high) {
    cout << "\nEnter number in range " << low << " - " << high << ": ";
    int value;

    cin >> value;

    if (low <= value && value <= high)
        return value;

    cout << "ERROR: invalid number...Try again\n";
    return ReadInt(low, high);
}

class Book {
private :
    string Title, Author;
    int number_of_pages, ISBN;
    vector<string> pages;
public:
    Book(const string &title, const string &author, int number_of_pages, int isbn) : Title(title), Author(author),
                                                                                     number_of_pages(number_of_pages),
                                                                                     ISBN(isbn) {}

    const string &getTitle() const {
        return Title;
    }

    int getPages() const {
        return number_of_pages;
    }

    void add_page(const string &data) {
        pages.push_back(data);
    }

    const string &getpage(const int &idx) const {
        return pages[idx];
    }
};

class Readingsessions {
private:
    string last_enter;
    int page;
    Book book;
public:
    Readingsessions(Book book) : book(book), page(0), last_enter(GetCurrentTimeDate()) {}

    const string &getTime() const {
        return last_enter;
    }

    void setTime(const string &time) {
        Readingsessions::last_enter = time;
    }

    int getPage() const {
        return page;
    }

    void setPage(int page) {
        Readingsessions::page = page;
    }

    const Book &getCur() const {
        return book;
    }

    void setCur(const Book &book) {
        Readingsessions::book = book;
    }

    void increase_page() {
        if (page < book.getPages() - 1)
            page++;
    }

    void decrease_page() {
        if (page > 0)
            page--;
    }

};

class User {
private:
    string username, name, email;
    int password;
    bool is_admin = 0;
    vector<Readingsessions> sessions;
public:
    User() {}

    User(string username, int password, string name, string email, bool admin = 0) : username(username),
                                                                                     password(password),
                                                                                     name(std::move(name)),
                                                                                     email(email), is_admin(admin) {}

    const string &getUsername() const {
        return username;
    }

    bool isAdmin() const {
        return is_admin;
    }

    int getPassword() const {
        return password;
    }

    const string &getName() const {
        return name;
    }

    const string &getEmail() const {
        return email;
    }

    void setUsername(const string &username) {
        User::username = username;
    }

    void setName(const string &name) {
        User::name = name;
    }

    void setEmail(const string &email) {
        User::email = email;
    }

    void setPassword(int password) {
        User::password = password;
    }

    void setIsAdmin(bool isAdmin) {
        is_admin = isAdmin;
    }

    void AddReadingSession(Readingsessions session) {
        sessions.push_back(session);
    }

    bool Print_Sessions() {
        if (sessions.empty()) {
            cout << "There is No current sessions\n";
            return 0;
        }
        for (int idx = 0; idx < sessions.size(); idx++) {
            cout << idx + 1 << ": " <<sessions[idx].getCur().getTitle()<<" : "<< sessions[idx].getPage() + 1 << "/" << sessions[idx].getCur().getPages() << " - "
                 << sessions[idx].getTime() << '\n';
        }
        return 1;
    }

    Readingsessions &get_session(int idx) {
        return sessions[idx];
    }

    int numberofsessions() {
        return (int) sessions.size();
    }

};

class Database {
    vector<Book> Books;
    vector<User> database;
public:
    Database() {
        database.push_back(User("root", 123, "omar", "omar@yahoo.com", 1));
    }

    void add_books() {
        string title, author;
        int ISBN, pages;
        cout << "Enter ISBN: ";
        cin >> ISBN;
        cout << "Enter Title: ";
        cin >> title;
        cout << "Enter Author's Name: ";
        cin >> author;
        cout << "Enter How Many Pages: ";
        cin >> pages;
        Books.emplace_back(Book(title, author, pages, ISBN));
        for (int cur = 1; cur <= pages; cur++) {
            cout << "Enter page # " << cur << ": ";
            string content;
            cin >> content;
            Books.back().add_page(content);
        }
    }

    bool print_books() {
        if (Books.empty()) {
            cout << "There are no available Books at the moment\n";
            return 0;
        }
        cout << "Our current book collection : \n";
        for (int idx = 0; idx < Books.size(); idx++) {
            cout << "\t\t" << idx + 1 << ' ' << Books[idx].getTitle() << '\n';
        }
        return 1;
    }

    User &add_user() {
        User obj;
        string un;
        cout << "Enter User name (no spaces): ";
        cin >> un;
        obj.setUsername(un);
        int password;
        cout << "Enter Password (no spaces): ";
        cin >> password;
        obj.setPassword(password);
        string name;
        cout << "Enter name (no spaces): ";
        cin >> name;
        obj.setName(name);
        string email;
        cout << "Enter email (no spaces): ";
        cin >> email;
        obj.setEmail(email);
        database.emplace_back(obj);
        return database.back();
    }//

    pair<User, bool> get_user(const string &UN, int &password) {
        for (int idx = 0; idx < (int) database.size(); idx++) {
            if (database[idx].getUsername() == UN and database[idx].getPassword() == password)
                return {database[idx], 1};
        }
        return {User("", -1, "", ""), 0};
    }

    int number_of_books() {
        return (int) Books.size();
    }

    Book get_book(int idx) {
        return Books[idx];
    }

};

class user_manager {
    Database db;
    User current;
public:
    int login(const string &UN, int &password) {
        pair<User, bool> find_user = db.get_user(UN, password);
        if (!find_user.second) {
            cout << "User name or password may be incorrect\n";
            return -1;
        }
        current = find_user.first;
        cout << "Hello " << current.getUsername() << " | ";
        cout << (current.isAdmin() ? "Admin" : "User") << " View\n";
        return current.isAdmin();
    }

    void signup() {
        current = db.add_user();
        cout << "Hello " << current.getUsername() << " | ";
        cout << "User" << " View\n";
    }

    void print_user() {
        cout << "Name : " << current.getName() << '\n';
        cout << "Email : " << current.getEmail() << '\n';
        cout << "User name : " << current.getUsername() << '\n';
    }

    void list_select_from_readings() {
        bool found = current.Print_Sessions();
        if (!found)
            return;
        cout << "\nWhich session to open?:\n";
        int choosing = ReadInt(1, current.numberofsessions());
        Readingsessions &read = current.get_session(choosing - 1);
        read.setTime(GetCurrentTimeDate());
        reading(read);
    }

    void list_select_from_available() {
        bool found = db.print_books();
        if (!found) {
            return;
        }
        cout << "\nWhich book to read?:\n";
        int choosing = ReadInt(1, db.number_of_books());
        Readingsessions read(db.get_book(choosing - 1));
        reading(read);
        current.AddReadingSession(read);
    }

    void reading(Readingsessions &session) {
        int choice = -1;
        while (choice != 3) {
            cout << "Current Page: " << session.getPage() + 1 << "/" << session.getCur().getPages() << '\n';
            cout << session.getCur().getpage(session.getPage()) << ' ' << session.getCur().getTitle() << "\n\n";
            cout << "Menu:\n";
            cout << "\t\t\t1: Next Page\n\t\t\t2: Previous Page\n\t\t\t 3: Stop Reading\n";
            choice = ReadInt(1, 3);
            if (choice == 1)
                session.increase_page();
            else if (choice == 2)
                session.decrease_page();
        }
    }

    void add_books() {
        db.add_books();
    }
};

class Dashboard {
    user_manager user;
public:
    void run() {
        while (1) {
            cout << "Menu :\n";
            cout << "\t\t\t 1: Login\n\t\t\t2: Sign Up\n\n";
            int choice = ReadInt(1, 2);
            if (choice == 1) {
                cout << "Enter user name (no spaces): ";
                string un;
                cin >> un;
                cout << "Enter Password (no spaces): ";
                int pass;
                cin >> pass;
                int success = user.login(un, pass);
                if (success == -1)
                    run();
                else if (success == 1)
                    admin_view();
                else
                    user_view();
            } else {
                user.signup();
                user_view();
            }
        }
    }

    void user_view() {
        int choice = -1;
        while (choice != 4) {
            cout << "Menu:\n";
            cout
                    << "\t\t\t1: View Profile\n\t\t\t2: List & Select from my Reading History\n\t\t\t3: List & Select from my Available Books\n\t\t\t4: Logout\n";
            choice = ReadInt(1, 4);
            if (choice == 1)
                user.print_user();
            else if (choice == 2)
                user.list_select_from_readings();
            else if (choice == 3)
                user.list_select_from_available();
        }
    }

    void admin_view() {
        int choice = -1;
        while (choice != 3) {
            cout << "Menu:\n";
            cout << "\t\t\t1: View Profile\n\t\t\t2: Add Books\n\t\t\t3: Logout\n";
            choice = ReadInt(1, 3);
            if (choice == 1)
                user.print_user();
            else if (choice == 2)
                user.add_books();
        }
    }
};


int main() {
    Dashboard program;
    program.run();
}
