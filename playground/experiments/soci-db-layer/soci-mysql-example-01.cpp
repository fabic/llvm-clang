/**
 * Example usage of SOCI database abstraction library.
 *
 * Got this from http://soci.sourceforge.net/doc/master/
 *
 * create table phonebook ( id int auto_increment, name varchar(255) not null, phone varchar(64) null, primary key (id) );
 * insert into phonebook (name,phone) values ("John", "+123 45 67 89"), ("Jane", "+33 262 12 34 56");
 *
 * FABIC/2017-12-19
 */

#include "soci/soci.h"
#include "soci/mysql/soci-mysql.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <exception>

using namespace soci;
using namespace std;

bool get_name(string &name)
{
    cout << "Enter name: ";
    cin >> name;
    return !cin.fail();
}

int main()
{
    try
    {
        session sql("mysql://db=pimpl user=fabi password='haiku'");

        sql << R"(CREATE TABLE IF NOT EXISTS phonebook (
                  id INT AUTO_INCREMENT,
                  name VARCHAR(255) NOT NULL,
                  phone VARCHAR(64) NULL,
                  PRIMARY KEY (id)
                ))" ;

        sql << "INSERT INTO phonebook (name,phone) VALUES (\"Tom\", \"+32 999 45 67 89\")" ;
        sql << "INSERT INTO phonebook (name,phone) VALUES (\"Adam\", \"+33 262 12 34 56\")" ;

        int count;
        sql << "SELECT count(*) FROM phonebook", into(count);

        cout << "We have " << count << " entries in the phonebook.\n";

        string name;
        while (get_name(name))
        {
            string phone;
            indicator ind;
            sql << "SELECT phone FROM phonebook WHERE name = :name",
                into(phone, ind), use(name);

            if (ind == i_ok)
            {
                cout << "The phone number is " << phone << '\n';
            }
            else
            {
                cout << "There is no phone for " << name << '\n';
            }
        }
    }
    catch (exception const &e)
    {
        cerr << "Error: " << e.what() << '\n';
    }
}
