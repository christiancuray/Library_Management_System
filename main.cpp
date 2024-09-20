/*
**********************
Christian Daryl Curay
Date: April 10 2024
Purpose : Final Project
**********************
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <iomanip>
#include <occi.h>
#include <ctime>
#include <cstring>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

/* Library Management System Functions */
int findByID(Statement* stmt, ResultSet* rs, Connection* conn);
string findByName(Statement* stmt, ResultSet* rs, Connection* conn);
void displayReport(Statement* stmt, ResultSet* rs, Connection* conn);
void addBookToLibrary(Statement* stmt, ResultSet* rs, Connection* conn);
void bookCheckout(Statement* stmt, ResultSet* rs, Connection* conn);
void bookReturn(Statement* stmt, ResultSet* rs, Connection* conn);
void addGenre(Statement* stmt, ResultSet* rs, Connection* conn);
void addPublisher(Statement* stmt, ResultSet* rs, Connection* conn);
void addLibraryBranch(Statement* stmt, ResultSet* rs, Connection* conn);
void addAdmin(Statement* stmt, ResultSet* rs, Connection* conn);
void addStaffMember(Statement* stmt, ResultSet* rs, Connection* conn);
void updateLibraryMemberInfo(Statement* stmt, ResultSet* rs, Connection* conn);
void createTables(Statement* stmt, Connection* conn);
void insertDataIntoTables(Statement* stmt, ResultSet* rs, Connection* conn);

int main()
{
	/* OCCI Variables */
	Environment* env = nullptr;
	Connection* conn = nullptr;
	Statement* stmt = nullptr;
	ResultSet* rs = nullptr;

	/* login info for connecting to database */
	string user = "dbs311_243nff11";
	string pass = "23788489";
	string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

	conn = NULL;
	try {
		//trying to connect to database...
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(user, pass, constr);

	}
	catch (SQLException& sqlExcp) {
		// catch if error occurs
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}

	//if unsuccessful connection, exit from the program
	if (!conn) {
		return 1;
	}

	try {
		int input = -1;
		bool exit = false;

		createTables(stmt, conn);
		insertDataIntoTables(stmt, rs, conn);

		do {
			cout << "\n--------------------------------------" << endl;
			cout << "Library Management System Menu:\n\n"
				<< "1) Find by ID\n"
				<< "2) Find by Name\n"
				<< "3) Display Report\n"
				<< "4) Add Book to Library\n"
				<< "5) Book Checkout\n"
				<< "6) Book Return\n"
				<< "7) Add Genre\n"
				<< "8) Add Publisher\n"
				<< "9) Add Library Branch\n"
				<< "10) Add Admin\n"
				<< "11) Add Staff member\n"
				<< "12) Update/Add/Remove Library member\n"
				<< "13) Exit" << endl;
			cout << "Enter number: ";
			cin >> input;
			cout << "--------------------------------------\n" << endl;
			

			switch (input) {
			case 1:
				findByID(stmt, rs, conn);
				break;
			case 2:
				findByName(stmt, rs, conn);
				break;
			case 3:
				displayReport(stmt, rs, conn);
				break;
			case 4:
				addBookToLibrary(stmt, rs, conn);
				break;
			case 5:
				bookCheckout(stmt, rs, conn);
				break;
			case 6:
				bookReturn(stmt, rs, conn);
				break;
			case 7:
				addGenre(stmt, rs, conn);
				break;
			case 8:
				addPublisher(stmt, rs, conn);
				break;
			case 9:
				addLibraryBranch(stmt, rs, conn);
				break;
			case 10:
				addAdmin(stmt, rs, conn);
				break;
			case 11:
				addStaffMember(stmt, rs, conn);
				break;
			case 12:
				updateLibraryMemberInfo(stmt, rs, conn);
				break;
			case 13:
				std::cout << "Thank you. Program is exiting now..." << std::endl;
				exit = true;
				break;
			default:
				std::cout << "The number you entered is not an option." << std::endl;
			}

		} while (!exit);
		
	}
	catch (SQLException& sqlExcp) {
		cout << "Error: ";
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}

	//terminates the connection and environment
	env->terminateConnection(conn);
	Environment::terminateEnvironment(env);

	return 0;
}
// Transform Entities into Tables
void createTables(Statement* stmt, Connection* conn) {
	stmt = nullptr;
	try {
		stmt = conn->createStatement();

		//Create Genre table
		stmt->execute("CREATE TABLE Genre ( "
			"genre_ID INT PRIMARY KEY NOT NULL, "
			"genre_name VARCHAR(255) NOT NULL"
			")");
		cout << ">> Genre table created successfully." << endl;

		// Create Publisher table
		stmt->execute("CREATE TABLE Publisher ( "
			"publisher_ID INT PRIMARY KEY NOT NULL, "
			"name VARCHAR(255) NOT NULL, "
			"contact VARCHAR(255) NOT NULL"
			")");
		cout << ">> Publisher table created successfully." << endl;

		// Create Members table
		stmt->execute("CREATE TABLE Members ( "
			"user_ID INT PRIMARY KEY NOT NULL, "
			"name VARCHAR(255) NOT NULL, "
			"contact_information VARCHAR(255) NOT NULL, "
			"borrowing_privileges NUMBER(1) NOT NULL"
			")");
		cout << ">> Members table created successfully." << endl;

		// Create Staff table
		stmt->execute("CREATE TABLE Staff ( "
			"staff_ID INT PRIMARY KEY NOT NULL, "
			"name VARCHAR(255) NOT NULL, "
			"contact_information VARCHAR(255) NOT NULL, "
			"administrative_privileges NUMBER(1) NOT NULL"
			")");
		cout << ">> Staff table created successfully." << endl;

		// Create Admin table
		stmt->execute("CREATE TABLE Admin ( "
			"admin_ID INT PRIMARY KEY NOT NULL, "
			"name VARCHAR(255) NOT NULL, "
			"contact VARCHAR(255) NOT NULL"
			")");
		cout << ">> Admin table created successfully." << endl;

		// Create Library_branch table
		stmt->execute("CREATE TABLE Library_Branch ( "
			"library_ID INT PRIMARY KEY NOT NULL, "
			"address VARCHAR(255) NOT NULL, "
			"contact_information VARCHAR(255) NOT NULL, "
			"admin INT NOT NULL, "
			"FOREIGN KEY (admin) REFERENCES Admin(admin_ID)"
			")");
		cout << ">> Library_Branch table created successfully." << endl;

		// Create Books table
		stmt->execute("CREATE TABLE Books ( "
			"book_ID INT PRIMARY KEY NOT NULL, "
			"title VARCHAR(255) NOT NULL, "
			"ISBN VARCHAR(255) NOT NULL, "
			"author VARCHAR(255) NOT NULL, "
			"genre_ID INT NOT NULL, "
			"publication_date DATE NOT NULL, "
			"availability_status NUMBER(1) DEFAULT 1 NOT NULL, "
			"library_ID INT NOT NULL, "
			"publisher_ID INT NOT NULL, "
			"FOREIGN KEY (genre_ID) REFERENCES Genre(genre_ID), "
			"FOREIGN KEY (library_ID) REFERENCES Library_Branch(library_ID), "
			"FOREIGN KEY (publisher_ID) REFERENCES Publisher(publisher_ID)"
			")");
		cout << ">> Books table created successfully." << endl;

		// Create Transactions table
		stmt->execute("CREATE TABLE Transactions ( "
			"trans_ID INT PRIMARY KEY NOT NULL, "
			"book_ID INT NOT NULL, "
			"user_ID INT NOT NULL, "
			"loan_date DATE NOT NULL, "
			"return_date DATE NOT NULL, "
			"status VARCHAR(255) NOT NULL, "
			"staff_ID INT NOT NULL, "
			"FOREIGN KEY (book_ID) REFERENCES Books(book_ID), "
			"FOREIGN KEY (user_ID) REFERENCES Members(user_ID), "
			"FOREIGN KEY (staff_ID) REFERENCES Staff(staff_ID)"
			")");
		cout << ">> Transactions table created successfully." << endl << endl;

		// commit all commands into database
		conn->commit();
		cout << "*** All tables created successfully. ***" << endl;

	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
		// if failure occurs, rollback to last commmit
		if (conn != nullptr) {
			conn->rollback();
		}

	}
}

// Data Insertion
void insertDataIntoTables(Statement* stmt, ResultSet* rs, Connection* conn) {
	try {
		stmt = conn->createStatement();
		cout << ">> Data inserting..." << endl;
		// populate data into tables
		// Genre data
		stmt->executeUpdate("INSERT INTO Genre VALUES (100, 'Adventure')");
		stmt->executeUpdate("INSERT INTO Genre VALUES (200, 'Action')");
		stmt->executeUpdate("INSERT INTO Genre VALUES (300, 'Non-Fiction')");
		stmt->executeUpdate("INSERT INTO Genre VALUES (400, 'Mystery')");
		stmt->executeUpdate("INSERT INTO Genre VALUES (500, 'Literary Fiction')");
		stmt->executeUpdate("INSERT INTO Genre VALUES (600, 'Science Fiction')");
		stmt->executeUpdate("INSERT INTO Genre VALUES (700, 'Romance')");

		// Publisher data
		stmt->executeUpdate("INSERT INTO Publisher VALUES (1, 'Grand Central Publishing', 'grand_celtral@yahoo.com')");
		stmt->executeUpdate("INSERT INTO Publisher VALUES (2, 'Penguin Classics', 'penguin123@company.com')");
		stmt->executeUpdate("INSERT INTO Publisher VALUES (3, 'HarperCollins', 'Harpercollins@personal@work')");
		stmt->executeUpdate("INSERT INTO Publisher VALUES (4, 'Ace Books', 'ace.writer@yahoo.ca')");
		stmt->executeUpdate("INSERT INTO Publisher VALUES (5, 'Presidio Press', 'press.works@company.ca')");
		stmt->executeUpdate("INSERT INTO Publisher VALUES (6, 'Alfred A. Knopf', 'alfred.reading@publishing.co')");
		stmt->executeUpdate("INSERT INTO Publisher VALUES (7, 'Random House', 'random.house@publishing.co')");

		// Members data
		stmt->executeUpdate("INSERT INTO Members VALUES (123, 'John Doe', 'john.doe@example.com', 1)");
		stmt->executeUpdate("INSERT INTO Members VALUES (345, 'Jane Smith', 'jane.smith@example.com', 0)");
		stmt->executeUpdate("INSERT INTO Members VALUES (265, 'Michael Jordan', 'michael.jordan@example.com', 1)");
		stmt->executeUpdate("INSERT INTO Members VALUES (523, 'Lebron James', 'lebron.james@example.com', 1)");
		stmt->executeUpdate("INSERT INTO Members VALUES (986, 'Kobe Bryant', 'kobe.bryant08@example.com', 1)");

		// Staff data
		stmt->executeUpdate("INSERT INTO Staff VALUES (111, 'John Eisley', 'john.eisley@example.com', 1)");
		stmt->executeUpdate("INSERT INTO Staff VALUES (222, 'Yuan Johnson', 'Yuan.johnson@example.com', 1)");
		stmt->executeUpdate("INSERT INTO Staff VALUES (333, 'Michael Magtangob', 'michael.magtangob@example.com', 0)");
		stmt->executeUpdate("INSERT INTO Staff VALUES (444, 'Jareem Brown', 'jareem.brown@example.com', 1)");
		stmt->executeUpdate("INSERT INTO Staff VALUES (555, 'Joshua Wilson', 'joshua.wilson@example.com', 1)");

		// Admin data
		stmt->executeUpdate("INSERT INTO Admin VALUES (8, 'Seneca College', 'group8@senecacollege.ca')");
		stmt->executeUpdate("INSERT INTO Admin VALUES (3, 'Toronto Libraries', 'toronto.lib.ca')");

		// Library_Branch data
		stmt->executeUpdate("INSERT INTO Library_Branch VALUES (647, '16 New Ville St.', 'libraryNewhamn@example.com',8)");
		stmt->executeUpdate("INSERT INTO Library_Branch VALUES (187, 'Toronto West', 'torontowest@example.com',3)");

		// Books data
		stmt->executeUpdate("INSERT INTO Books VALUES (1000120, 'The Notebook', 'ISBN1', 'Nicholas Sparks', 700, TO_DATE('2022-01-01', 'YYYY-MM-DD'), 1, 647, 1)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000121, 'Pride and Prejudice', 'ISBN2', 'Jane Austen', 500, TO_DATE('2022-01-02', 'YYYY-MM-DD'), 1, 647, 2)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000122, 'To Kill a Mockingbird', 'ISBN3', 'Harper Lee', 500, TO_DATE('2022-01-03', 'YYYY-MM-DD'), 1, 647, 3)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000123, 'Neuromancer', 'ISBN4', 'William Gibson', 600, TO_DATE('2022-01-04', 'YYYY-MM-DD'), 1, 647, 4)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000124, 'The Left Hand of Darkness', 'ISBN5', 'Ursula K. Le Guin', 600, TO_DATE('2022-01-05', 'YYYY-MM-DD'), 1, 647, 4)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000125, 'The Adventures of Sherlock Holmes', 'ISBN6', 'Arthur Conan Doyle', 400, TO_DATE('2022-01-06', 'YYYY-MM-DD'), 1, 647, 2)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000126, 'The Guns of August', 'ISBN7', 'Barbara W. Tuchman', 400, TO_DATE('2022-01-07', 'YYYY-MM-DD'), 1, 187, 5)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000127, 'Jurassic Park', 'ISBN8', 'Michael Crichton', 600, TO_DATE('2022-01-08', 'YYYY-MM-DD'), 1, 647, 6)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000128, 'The Adventures of Tom Sawyer', 'ISBN9', 'Mark Twain', 700, TO_DATE('2022-01-09', 'YYYY-MM-DD'), 1, 647, 2)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000129, 'Educated', 'ISBN10', 'Tara Westover', 300, TO_DATE('2022-01-10', 'YYYY-MM-DD'), 1, 647, 7)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000131, 'Treasure Island', 'ISBN11', 'Robert Louis Stevenson', 100, TO_DATE('2022-01-11', 'YYYY-MM-DD'), 0, 647, 2)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000132, 'The Immortal Life of Henrietta Lacks', 'ISBN12', 'Rebecca Skloot', 300, TO_DATE('2022-01-12', 'YYYY-MM-DD'), 0, 187, 7)");
		stmt->executeUpdate("INSERT INTO Books VALUES (1000133, 'Die Hard', 'ISBN13', 'Roderick Thorp', 200, TO_DATE('2022-01-13', 'YYYY-MM-DD'), 0, 187, 3)");

		// Transactions data
		stmt->executeUpdate("INSERT INTO Transactions VALUES (20245, 1000120, 123, TO_DATE('2024-04-10', 'YYYY-MM-DD'), TO_DATE('2024-04-17', 'YYYY-MM-DD'), 'returned', 111)");
		stmt->executeUpdate("INSERT INTO Transactions VALUES (20243, 1000121, 523, TO_DATE('2024-04-09', 'YYYY-MM-DD'), TO_DATE('2024-04-16', 'YYYY-MM-DD'), 'returned', 222)");
		stmt->executeUpdate("INSERT INTO Transactions VALUES (20241, 1000122, 265, TO_DATE('2024-04-08', 'YYYY-MM-DD'), TO_DATE('2024-04-15', 'YYYY-MM-DD'), 'borrowed', 444)");
		stmt->executeUpdate("INSERT INTO Transactions VALUES (20231, 1000123, 523, TO_DATE('2024-04-07', 'YYYY-MM-DD'), TO_DATE('2024-04-14', 'YYYY-MM-DD'), 'borrowed', 444)");
		stmt->executeUpdate("INSERT INTO Transactions VALUES (20225, 1000124, 986, TO_DATE('2024-04-06', 'YYYY-MM-DD'), TO_DATE('2024-04-13', 'YYYY-MM-DD'), 'returned', 555)");
		stmt->executeUpdate("INSERT INTO Transactions VALUES (20227, 1000125, 265, TO_DATE('2024-04-05', 'YYYY-MM-DD'), TO_DATE('2024-04-13', 'YYYY-MM-DD'), 'borrowed', 111)");
		stmt->executeUpdate("INSERT INTO Transactions VALUES (20229, 1000126, 986, TO_DATE('2024-04-04', 'YYYY-MM-DD'), TO_DATE('2024-04-11', 'YYYY-MM-DD'), 'borrowed', 222)");
		stmt->executeUpdate("INSERT INTO Transactions VALUES (20233, 1000127, 123, TO_DATE('2024-04-03', 'YYYY-MM-DD'), TO_DATE('2024-04-10', 'YYYY-MM-DD'), 'returned', 111)");

		conn->commit();
		cout << ">> Data inserted successfully!" << endl;
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
	}

}

// FIND BY ID ( OPTION 1 )
int  findByID(Statement* stmt, ResultSet* rs, Connection* conn) {
	int cID;

	cout << "\n*** Find Member by ID ***\n\n";
	cout << "Enter Member ID: ";
	cin >> cID;
	cout << endl;

	stmt = conn->createStatement("SELECT * FROM members WHERE user_id = :1");
	stmt->setInt(1, cID);
	rs = stmt->executeQuery();

	if (!rs->next()) {
		cout << "ResultSet is empty." << endl;
	}
	else {
		cout << "Member Details:"
			<< "\n> Member Number: " << rs->getInt(1)
			<< "\n> Member Name: " << rs->getString(2)
			<< "\n> Contact Information: " << rs->getString(3)
			<< endl;
	}
	return cID;
}

// FIND BY NAME ( OPTION 2 )
string  findByName(Statement* stmt, ResultSet* rs, Connection* conn) {
	std::string name;

	cout << "\n*** Find Member by Name ***\n\n";
	cin.ignore();
	cout << "Enter Member Full Name: ";
	getline(cin, name);
	cout << endl;

	stmt = conn->createStatement("SELECT * FROM members WHERE name = :1");
	stmt->setString(1, name);
	rs = stmt->executeQuery();

	if (!rs->next()) {
		cout << "ResultSet is empty." << endl;
	}
	else {
			cout << "Member Details:"
				<< "\n> Member Number: " << rs->getInt(1)
				<< "\n> Member Name: " << rs->getString(2)
				<< "\n> Contact Information: " << rs->getString(3)
				<< endl;
	}
	return name;
}

// DISPLAY REPORT ( OPTION 3 )
void displayReport(Statement* stmt, ResultSet* rs, Connection* conn) {
	string name;
	int reportChoice;

	cout << "Select which report you want to display:\n"
		<< "1. Book Inventory Report\n"
		<< "2. Transaction History Report\n"
		<< "3. Genre Classification Report\n"
		<< "4. Publisher Engagement Report\n"
		<< "5. Administrative Privileges Audit Report\n"
		<< "6. Borrowing Privileges Status Report\n"
		<< "7. Overdue Books Report\n"
		<< "Enter your choice: ";
	cin >> reportChoice;
	cout << endl;
	 rs = nullptr;

	if (reportChoice == 1) {
		stmt = conn->createStatement("SELECT b.title, b.ISBN, b.author, g.genre_name, p.name AS publisher_name, "
			"b.availability_status FROM Books b JOIN Genre g ON b.genre_ID = g.genre_ID JOIN Publisher p ON b.publisher_ID = p.publisher_ID");
		rs = stmt->executeQuery();
		if (!rs->next()) {
			cout << "No books found." << endl;
		}
		else {
			cout << "----------------- Books Details -----------------" << endl << endl;
			cout << setw(40) << left << "Title"
				<< setw(15) << left << "ISBN"
				<< setw(25) << left << "Author"
				<< setw(20) << left << "Genre"
				<< setw(30) << left << "Publisher"
				<< setw(5) << left << "Status" << endl;

			do {
				cout << setw(40) << left << rs->getString(1)
					<< setw(15) << left <<  rs->getString(2)
					<< setw(25) << left <<  rs->getString(3)
					<< setw(20) << left <<  rs->getString(4)
					<< setw(30) << left <<  rs->getString(5)
					<< setw(5) << left <<  rs->getInt(6)
					<< endl;
			} while (rs->next());
		}
	}
	else if (reportChoice == 2) {
		stmt = conn->createStatement("SELECT b.title, m.name AS user_name, t.loan_date, t.return_date, t.status "
			"FROM Transactions t JOIN Books b ON t.book_ID = b.book_ID JOIN Members m ON t.user_ID = m.user_ID");
		rs = stmt->executeQuery();
		if (!rs->next()) {
			cout << "No transactions found." << endl;
		}
		else {
			cout << "----------------- Transaction Details -----------------" << endl << endl;
			cout << setw(40) << left << "Book Title"
				<< setw(40) << left << "User Name"
				<< setw(15) << left << "Loan Date"
				<< setw(15) << left << "Return Date"
				<< setw(10) << left << "Status" << endl;
			do {
	
				cout << setw(40) << left << rs->getString(1)
					<< setw(40) << left << rs->getString(2)
					<< setw(15) << left << rs->getString(3)
					<< setw(15) << left << rs->getString(4)
					<< setw(10) << left << rs->getString(5)
					<< endl;
			} while (rs->next());
		}
	}
	else if (reportChoice == 3) {
		stmt = conn->createStatement("SELECT g.genre_name, COUNT(*) AS book_count "
			"FROM Books b JOIN Genre g ON b.genre_ID = g.genre_ID GROUP BY g.genre_name");
		rs = stmt->executeQuery();
		if (!rs->next()) {
			cout << "No genre count found." << endl;
		}
		else {
			cout << "----------------- Genre Details -----------------" << endl << endl;
			cout << setw(20) << left << "Genre Name"
				<< setw(5) << left << "Book Count" << endl;
			do {
				cout << setw(20) << left << rs->getString(1)
					<< setw(5) << left << rs->getInt(2)
					<< endl;
			} while (rs->next());
		}
	}
	else if (reportChoice == 4) {
		stmt = conn->createStatement("SELECT p.name AS publisher_name, COUNT(*) AS book_count "
			"FROM Books b JOIN Publisher p ON b.publisher_ID = p.publisher_ID GROUP BY p.name");
		rs = stmt->executeQuery();
		if (!rs->next()) {
			cout << "No publisher count found." << endl;
		}
		else {
			cout << "----------------- Publisher Details -----------------" << endl << endl;
			cout << setw(40) << left << "Publisher Name"
				<< setw(5) << left << "Book Count" << endl;
			do {
				cout << setw(40) << left << rs->getString(1)
					<< setw(5) << left << rs->getInt(2)
					<< endl;
			} while (rs->next());
		}
	}
	else if (reportChoice == 5) {
		stmt = conn->createStatement("SELECT s.name AS staff_name, b.title, s.ADMINISTRATIVE_PRIVILEGES, "
			"t.status FROM Transactions t JOIN Staff s ON t.staff_ID = s.staff_ID JOIN Books b ON t.book_ID = b.book_ID");
		rs = stmt->executeQuery();
		if (!rs->next()) {
			cout << "No staff details found." << endl;
		}
		else {
			cout << "----------------- Staff Details -----------------" << endl << endl;
			cout << setw(40) << left << "Staff Name"
				<< setw(40) << left << "Book Title" 
				<< setw(25) << left << "Administrative Privileges" 
				<< setw(10) << left << "Status" 
				<< endl;
			do {
					cout << setw(40) << left << rs->getString(1)
					<< setw(40) << left << rs->getString(2)
					<< setw(25) << left  << rs->getInt(3)
					<< setw(10) << left << rs->getString(4)
					<< endl;
			} while (rs->next());
		}
	}
	else if (reportChoice == 6) {
		stmt = conn->createStatement("SELECT m.name AS member_name, m.borrowing_privileges FROM Members m");
		rs = stmt->executeQuery();
		if (!rs->next()) {
			cout << "No member details found." << endl;
		}
		else {
			cout << "----------------- Member Details -----------------" << endl << endl;
			cout << setw(40) << left << "Member Name"
				<< setw(5) << left << "Borrowing Privileges" << endl;
			do {
				cout << setw(40) << left << rs->getString(1)
					<< setw(5) << left << rs->getInt(2)
					<< endl;
			} while (rs->next());
		}
	}
	else if (reportChoice == 7) {
		stmt = conn->createStatement("SELECT b.title, m.name AS borrower_name, t.loan_date, "
			"t.RETURN_DATE FROM Transactions t JOIN Books b ON t.book_ID = b.book_ID JOIN Members m "
			"ON t.user_ID = m.user_ID WHERE t.return_date < current_date AND t.STATUS = 'borrowed'");
		rs = stmt->executeQuery();
		if (!rs->next()) {
			cout << "No overdue borrowed books found." << endl;
		}
		else {
			cout << "----------------- Overdue Borrowed Book Details -----------------" << endl << endl;
			cout << setw(40) << left << "Book Title"
				<< setw(40) << left << "Borrower Name"
				<< setw(15) << left << "Loan Date"
				<< setw(15) << left << "Return Date"
				<< endl;
			do {
					cout << setw(40) << left << rs->getString(1)
					<< setw(40) << left << rs->getString(2)
					<< setw(15) << left << rs->getString(3)
					<< setw(15) << left << rs->getString(4)
					<< endl;
			} while (rs->next());
		}
	}
	else {
		cout << "The number you entered is not in the options." << endl;
	}
	
}

// ADD BOOK  ( OPTION 4 )
void addBookToLibrary(Statement* stmt, ResultSet* rs, Connection* conn) {
	
	int bookId, genreId, availability, libId, pubId;
	string title, ISBN, author, pubDate;

	cout << "*** ADD BOOK TO LIBRARY ***\n\n";
	cout << "Enter book ID: ";
	cin >> bookId;
	cin.ignore();
	cout << "Enter title: ";
	getline(cin, title);
	cout << "Enter ISBN: ";
	getline(cin, ISBN);
	cout << "Enter author: ";
	getline(cin, author);
	cout << "Enter genre ID: ";
	cin >> genreId;
	cin.ignore();
	cout << "Enter publication date (YYYY-MM-DD): ";
	getline(cin, pubDate);
	cout << "Enter availability status (0 or 1): ";
	cin >> availability;
	cout << "Enter library ID: ";
	cin >> libId;
	cout << "Enter publisher ID: ";
	cin >> pubId;

	try {
		stmt = conn->createStatement();
		stmt->setSQL("INSERT INTO Books VALUES (:1, :2, :3, :4, :5, TO_DATE(:6, 'YYYY-MM-DD'), :7, :8, :9)");
		stmt->setInt(1, bookId);
		stmt->setString(2, title);
		stmt->setString(3, ISBN);
		stmt->setString(4, author);
		stmt->setInt(5, genreId);
		stmt->setString(6, pubDate);
		stmt->setInt(7, availability);
		stmt->setInt(8, libId);
		stmt->setInt(9, pubId);

		stmt->executeUpdate();
		conn->commit();
		cout << " >> Book checkout successfully!" << endl;
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
	}
}

// BOOK CHECKOUT  ( OPTION 5 )
void bookCheckout(Statement* stmt, ResultSet* rs, Connection* conn) {
	int trans_id, book_id, user_id, staff_id;
	string loan_date, return_date;

	cout << "*** BOOK CHECKOUT ***\n\n";
	cout << "Enter Trasaction Number: ";
	cin >> trans_id;
	cout << "Enter Book ID: ";
	cin >> book_id;
	cout << "Enter Member ID: ";
	cin >> user_id;
	cin.ignore();
	cout << "Enter Loan Date(YYYY-MM-DD): ";
	getline(cin, loan_date, '\n');
	cout << "Enter Return Date(YYYY-MM-DD): ";
	getline(cin, return_date, '\n');
	cout << "Enter Staff ID: ";
	cin >> staff_id;

	try {
		stmt = conn->createStatement();
		stmt->setSQL("INSERT INTO Transactions (trans_ID, book_ID, user_ID, loan_date, return_date, status, staff_ID) "
			"VALUES (:1, :2, :3, TO_DATE(:4, 'YYYY-MM-DD'), TO_DATE(:5, 'YYYY-MM-DD'), 'borrowed', :6)");
		stmt->setInt(1, trans_id);
		stmt->setInt(2, book_id);
		stmt->setInt(3, user_id);
		stmt->setString(4, loan_date);
		stmt->setString(5, return_date);
		stmt->setInt(6, staff_id);

		stmt->executeUpdate();
		conn->commit();
		cout << " >> Book checkout successfully!" << endl;
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
	}
}

// BOOK RETURN  ( OPTION 6 )
void bookReturn(Statement* stmt, ResultSet* rs, Connection* conn){
	int trans_id;
	string return_date;

	cout << "*** BOOK RETURN ***\n\n";
	cout << "Enter Trasaction Number: ";
	cin >> trans_id;
	cin.ignore();
	cout << "Enter Return Date(YYYY-MM-DD): ";
	getline(cin, return_date, '\n');

	try {
		stmt = conn->createStatement();
		stmt->setSQL("UPDATE transactions SET return_date = TO_DATE(:1, 'YYYY-MM-DD'), status = 'returned' WHERE trans_id = :2");
		stmt->setString(1, return_date);
		stmt->setInt(2, trans_id);

		stmt->executeUpdate();
		conn->commit();
		cout << " >> Book returned successfully!" << endl;
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
	}

}

// ADD GENRE  ( OPTION 7 )
void addGenre(Statement* stmt, ResultSet* rs, Connection* conn){
	int id;
	string genre;

	cout << "*** ADD GENRE ***\n\n";
	cout << "Enter genre ID: ";
	cin >> id;
	cin.ignore();
	cout << "Enter genre: ";
	getline(cin, genre);

	try {
		stmt = conn->createStatement();
		stmt->setSQL("INSERT INTO Genre VALUES (:1, :2)");
		stmt->setInt(1, id);
		stmt->setString(2, genre);
		
		stmt->executeUpdate();
		conn->commit();
		cout << " >> Genre added successfully!" << endl;
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
	}
}

// ADD PUBLISHER  ( OPTION 8 )
void addPublisher(Statement* stmt, ResultSet* rs, Connection* conn){
	int id;
	string name, contact;

	cout << "*** ADD Publisher ***\n\n";
	cout << "Enter Publisher ID: ";
	cin >> id;
	cin.ignore();
	cout << "Enter Publisher Name: ";
	getline(cin, name);
	cout << "Enter Contact Information: ";
	getline(cin, contact);

	try {
		stmt = conn->createStatement();
		stmt->setSQL("INSERT INTO Publisher VALUES (:1, :2, :3)");
		stmt->setInt(1, id);
		stmt->setString(2, name);
		stmt->setString(3, contact);

		stmt->executeUpdate();
		conn->commit();
		cout << " >> Genre added successfully!" << endl;
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
	}
}

// ADD LIBRARY BRANCH  ( OPTION 9 )
void addLibraryBranch(Statement* stmt, ResultSet* rs, Connection* conn){
	int libID, adminID;
	string address, contact;

	cout << "*** ADD Library Branch ***\n\n";
	cout << "Enter Library Branch ID: ";
	cin >> libID;
	cin.ignore();
	cout << "Enter Library Branch Address: ";
	getline(cin, address);
	cout << "Enter Library Branch Contact Information: ";
	getline(cin, contact);
	cout << "Enter Admin Number: )";
	cin >> adminID;

	try {
		stmt = conn->createStatement();
		stmt->setSQL("INSERT INTO Library_Branch VALUES (:1, :2, :3, :4)");
		stmt->setInt(1, libID);
		stmt->setString(2, address);
		stmt->setString(3, contact);
		stmt->setInt(4, adminID);

		stmt->executeUpdate();
		conn->commit();
		cout << " >> Library Branch added successfully!" << endl;
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
	}
}

// ADD ADMIN  ( OPTION 10 )
void addAdmin(Statement* stmt, ResultSet* rs, Connection* conn){
	int id, input;
	string name, contact;
	cout << "Select Option:\n"
		<< "1) Add Admin\n"
		<< "2) Remove Admin\n";
	cout << "Enter number: ";
	cin >> input;
	if (input == 1) {
		cout << "*** ADD Admin ***\n\n";
		cout << "Enter Admin ID: ";
		cin >> id;
		cin.ignore();
		cout << "Enter Admin Name: ";
		getline(cin, name);
		cout << "Enter Contact Information: ";
		getline(cin, contact);

		try {
			stmt = conn->createStatement();
			stmt->setSQL("INSERT INTO Admin VALUES (:1, :2, :3)");
			stmt->setInt(1, id);
			stmt->setString(2, name);
			stmt->setString(3, contact);

			stmt->executeUpdate();
			conn->commit();
			cout << " >> Admin added successfully!" << endl;
		}
		catch (SQLException& sqlExcp) {
			cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
		}
	}
	else if (input == 2) {
		cout << "*** Remove Admin ***\n\n";
		cout << "Enter Admin ID: ";
		cin >> id;
		try {
			stmt = conn->createStatement();
			stmt->setSQL("DELETE FROM Admin WHERE admin_ID = :1");
			stmt->setInt(1, id);

			stmt->executeUpdate();
			conn->commit();
			cout << " >> Admin deleted successfully!" << endl;
		}
		catch (SQLException& sqlExcp) {
			cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
		}
	}
	else {
		cout << "Invalid Input!" << endl;
	}
}

// ADD STAFF  ( OPTION 11 )
void addStaffMember(Statement* stmt, ResultSet* rs, Connection* conn){
	int id, previlege;
	string name, contact;

	cout << "*** ADD Staff ***\n\n";
	cout << "Enter Staff ID: ";
	cin >> id;
	cin.ignore();
	cout << "Enter Staff Name: ";
	getline(cin, name);
	cout << "Enter Contact Information: ";
	getline(cin, contact);
	cout << "Enter administrative privileges (0 or 1): ";
	cin >> previlege;

	try {
		stmt = conn->createStatement();
		stmt->setSQL("INSERT INTO Staff VALUES (:1, :2, :3, :4)");
		stmt->setInt(1, id);
		stmt->setString(2, name);
		stmt->setString(3, contact);
		stmt->setInt(4, previlege);

		stmt->executeUpdate();
		conn->commit();
		cout << " >> Staff added successfully!" << endl;
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
	}
	}

// UPDATE LIBRARY MEMBER INFO ( OPTION 12 )
void updateLibraryMemberInfo(Statement* stmt, ResultSet* rs, Connection* conn){
	int id, previlege, input,option;
	string name, contact;
	cout << "Select Option:\n"
		<< "1) Update Existing Member\n"
		<< "2) Add New Member\n"
		<< "3) Remove Member\n";
	cout << "Enter number: ";
	cin >> input;

	if (input == 1) {
		cout << "\n*** Update Member Information ***\n\n";
		cout << "Enter Member ID: ";
		cin >> id;
		cout << "Select one of the following options you wish to update:\n"
			 << "1. Name\n"
			 << "2. Contact Information\n"
			 << "3. Borrowing Privileges\n"
			 << "Enter option: ";
		cin >> option;
		switch (option) {
		case 1: {
			// update name
			cin.ignore();
			string newName;
			cout << "Enter the new name: ";
			getline(cin, newName);

			try {
				stmt = conn->createStatement();
				stmt->setSQL("UPDATE Members SET name = :1 WHERE user_ID = :2");
				stmt->setString(1, newName);
				stmt->setInt(2, id);

				stmt->executeUpdate();
				conn->commit();
				cout << ">> Name updated successfully." << endl;
			}
			catch (SQLException& sqlExcp) {
				cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
			}
			break;
		}
		case 2: {
			// update contact
			cin.ignore();
			string newContactInfo;
			cout << "Enter the new contact information: ";
			getline(cin, newContactInfo);

			try {
				stmt = conn->createStatement();
				stmt->setSQL("UPDATE Members SET contact_information = :1 WHERE user_ID = :2");
				stmt->setString(1, newContactInfo);
				stmt->setInt(2, id);

				stmt->executeUpdate();
				conn->commit();
				cout << ">> Contact information updated successfully." << endl;
			}
			catch (SQLException& sqlExcp) {
				cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
			}
			break;
		}
		case 3: {
			// update borrowing privileges
			int newBorrowingPrivileges;
			cout << "Enter the new borrowing privileges (0 for no, 1 for yes): ";
			cin >> newBorrowingPrivileges;

			try {
				stmt = conn->createStatement();
				stmt->setSQL("UPDATE Members SET borrowing_privileges = :1 WHERE user_ID = :2");
				stmt->setInt(1, newBorrowingPrivileges);
				stmt->setInt(2, id);

				stmt->executeUpdate();
				conn->commit();
				cout << ">> Borrowing privileges updated successfully." << endl;
			}
			catch (SQLException& sqlExcp) {
				cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
			}
			break;
		}
		default:
			cout << "Invalid choice." << endl;
		}
	}
	else if (input == 2) {
		cout << "\n*** ADD Member ***\n\n";
		cout << "Enter Member ID: ";
		cin >> id;
		cin.ignore();
		cout << "Enter Member Name: ";
		getline(cin, name);
		cout << "Enter Contact Information: ";
		getline(cin, contact);
		cout << "Enter borrowing privileges (0 or 1): ";
		cin >> previlege;

		try {
			stmt = conn->createStatement();
			stmt->setSQL("INSERT INTO Members VALUES (:1, :2, :3, :4)");
			stmt->setInt(1, id);
			stmt->setString(2, name);
			stmt->setString(3, contact);
			stmt->setInt(4, previlege);

			stmt->executeUpdate();
			conn->commit();
			cout << " >> Member added successfully!" << endl;
		}
		catch (SQLException& sqlExcp) {
			cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
		}
	}
	else if (input == 3) {
		cout << "\n*** Remove Member ***\n\n";
		cout << "Enter Member ID: ";
		cin >> id;
		try {
			stmt = conn->createStatement();
			stmt->setSQL("DELETE FROM Members WHERE user_ID = :1");
			stmt->setInt(1, id);
		
			stmt->executeUpdate();
			conn->commit();
			cout << " >> Member Removed successfully!" << endl;
		}
		catch (SQLException& sqlExcp) {
			cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
		}
	}
	else {
		cout << "Invalid Input!" << endl;
	}
}
