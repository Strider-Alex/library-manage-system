/*
Library Mange System
Created by Yang Kefan on 2017/05/02
Support books and persons mangement
IMPORTANT:
Be sure to load library.sql first before you run this program
*/

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>

/*
Include directly the different
headers from cppconn/ and mysql_driver.h + mysql_util.h
(and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"
#include "credentials.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

//safely read an integer
void getint_s(istream& in, int& n) {
	cin >> n;
	cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
}

//handle errors
void err_handler(sql::SQLException &e) {
	int code = e.getErrorCode();
	string msg = "Operation Failed: ";
	//depulicate primary key
	if (code ==1062) {
		msg += "ID already exists.";
	}
	//foregin key not exist
	else if (code == 1452) {
		msg += "PID/BID not exist.";
	}
	else {
		msg += e.what();
	}
	cout << msg << endl;
}

int main(void)
{
	int choice,bid,pid,month_cnt;
	string title, author, name, gender, role;
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::PreparedStatement *prep_stmt;
		sql::ResultSet *res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(MYSQL_SERVER, MYSQL_USERNAME, MYSQL_PASSWORD);
		/* Connect to the MySQL test database */
		con->setSchema(MYSQL_DB);
		while (true) {
			cout << "=====Library Manage System=====" << endl;
			cout << "1. Add Book" << endl;
			cout << "2. Remove Book" << endl;
			cout << "3. List Book" << endl;
			cout << "4. Borrow Book" << endl;
			cout << "5. Return Book" << endl;
			cout << "6. Add Person" << endl;
			cout << "7. Remove Person" << endl;
			cout << "8. List Person" << endl;
			cout << "9. List Borrow Record" << endl;
			cout << "0. Exit" << endl;
			cout << "===============================" << endl;
			cout << "Enter instruction: ";
			getint_s(cin, choice);
			if (choice == 1) {
				try {
					prep_stmt = con->prepareStatement("INSERT INTO book VALUES (?,?,?)");
					cout << "Enter book id: ";
					getint_s(cin, bid);
					prep_stmt->setInt(1, bid);
					cout << "Enter book title: " ;
					getline(cin, title);
					prep_stmt->setString(2, title);
					cout << "Enter book author: ";
					getline(cin,author);
					prep_stmt->setString(3, author);
					prep_stmt->execute(); //execute query
					cout << "OK! Insertion complete." << endl;
					delete prep_stmt; //delete prepared statement
				}
				catch (sql::SQLException &e) {
					err_handler(e);
				}
			}
			else if (choice == 2) {
				try {
					prep_stmt = con->prepareStatement("DELETE FROM book WHERE bid = ?");
					cout << "Enter book id: ";
					getint_s(cin, bid);
					prep_stmt->setInt(1, bid);
					prep_stmt->execute(); //execute query
					cout << "OK! Deletion complete." << endl;
					delete prep_stmt; //delete prepared statement
				}
				catch (sql::SQLException &e) {
					err_handler(e);
				}
			}
			else if (choice == 3) {
				try {
					stmt = con->createStatement();
					res = stmt->executeQuery("SELECT * FROM book");
					cout << "Get " << res->rowsCount() << " row(s) in total." << endl;
					cout << left <<  setw(11) << "BID" << setw(20) << "TITLE" << setw(20) << "AUTHOR" << endl;
					while (res->next()) {
						cout << left << setw(11) << res->getInt("bid") <<  setw(20) << res->getString("title") <<  setw(20) <<res->getString("author") << endl;
					}
					delete stmt;
					delete res;
				}
				catch (sql::SQLException &e) {
					err_handler(e);
				}
			}
			else if (choice == 4) {
				try {
					prep_stmt = con->prepareStatement("INSERT INTO borrows VALUES (?,?,false,NOW(),DATE_ADD(NOW(),interval ? month))");
					cout << "Enter person id: ";
					getint_s(cin, pid);
					prep_stmt->setInt(1, pid);
					cout << "Enter book id: ";
					getint_s(cin, bid);
					prep_stmt->setInt(2, bid);
					cout << "Enter duration(/month): ";
					getint_s(cin, month_cnt);
					prep_stmt->setInt(3, month_cnt);
					prep_stmt->execute(); //execute query
					cout << "OK! Insertion complete." << endl;
					delete prep_stmt; //delete prepared statement
				}
				catch (sql::SQLException &e) {
					err_handler(e);
				}
			}
			else if (choice == 5) {
				try {
					prep_stmt = con->prepareStatement("UPDATE borrows SET has_return = true WHERE pid = ? and bid = ? and has_return = false");
					cout << "Enter person id: ";
					getint_s(cin, pid);
					prep_stmt->setInt(1, pid);
					cout << "Enter book id: ";
					getint_s(cin, bid);
					prep_stmt->setInt(2, bid);
					prep_stmt->execute(); //execute query
					cout << "OK! Update complete." << endl;
					delete prep_stmt; //delete prepared statement
				}
				catch (sql::SQLException &e) {
					err_handler(e);
				}
			}
			else if (choice == 6) {
				try {
					prep_stmt = con->prepareStatement("INSERT INTO person VALUES (?,?,?,?)");
					cout << "Enter person id: ";
					getint_s(cin, pid);
					prep_stmt->setInt(1, pid);
					cout << "Enter person name: ";
					getline(cin, name);
					prep_stmt->setString(2, name);
					cout << "Enter person gender(male/female): ";
					getline(cin, gender);
					prep_stmt->setString(3, gender);
					cout << "Enter person role(student/teacher): ";
					getline(cin, role);
					prep_stmt->setString(4, role);
					prep_stmt->execute(); //execute query
					cout << "OK! Insertion complete." << endl;
					delete prep_stmt; //delete prepared statement
				}
				catch (sql::SQLException &e) {
					err_handler(e);
				}
			}
			else if (choice == 7) {
				try {
					prep_stmt = con->prepareStatement("DELETE FROM person WHERE pid = ?");
					cout << "Enter person id: ";
					getint_s(cin, pid);
					prep_stmt->setInt(1, pid);
					prep_stmt->execute(); //execute query
					cout << "OK! Deletion complete." << endl;
					delete prep_stmt; //delete prepared statement
				}
				catch (sql::SQLException &e) {
					err_handler(e);
				}
			}
			else if (choice == 8) {
				try {
					stmt = con->createStatement();
					res = stmt->executeQuery("SELECT * FROM person");
					cout << "Get " << res->rowsCount() << " row(s) in total." << endl;
					cout << left << setw(11) << "PID" << setw(20) << "NAME" << setw(10) << "GENDER" << setw(10) << "ROLE" << endl;
					while (res->next()) {
						cout << left << setw(11) << res->getInt("pid") << setw(20) << res->getString("name") << setw(10) << res->getString("gender") << setw(10) << res->getString("role") << endl;
					}
					delete stmt;
					delete res;
				}
				catch (sql::SQLException &e) {
					err_handler(e);
				}
			}
			else if (choice == 9) {
				try {
					stmt = con->createStatement();
					res = stmt->executeQuery("SELECT * FROM borrows ORDER BY has_return");
					cout << "Get " << res->rowsCount() << " row(s) in total." << endl;
					cout << left << setw(11) << "PID" << setw(11) << "BID" << setw(15) << "HAS_RETURN" << setw(20) << "START_TIME" << setw(20) << "END_TIME" << endl;
					while (res->next()) {
						cout << left << setw(11) << res->getInt("pid") << setw(11) << res->getInt("bid") << setw(15) << res->getString("has_return") << setw(20) << res->getString("start_time") << setw(20) << res->getString("end_time") << endl;
					}
					delete stmt;
					delete res;
				}
				catch (sql::SQLException &e) {
					err_handler(e);
				}
			}
			else if (choice == 0) {
				break;
			}
			else {
				cout << "Invalid instruction!" << endl;
			}
		}
		//delete res;
		delete stmt;
		delete con;
		//delete prep_stmt;
	}
	catch (sql::SQLException &e) {
		err_handler(e);
	}

	cout << endl;

	return EXIT_SUCCESS;
}