#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <fstream>
using namespace std;

UserDatabase::UserDatabase()
{
    m_has_loaded = false;
}

UserDatabase::~UserDatabase()
{
    for (int i = 0; i < m_users.size(); i++)
    {
        delete m_users[i];
        m_users[i] = nullptr;
    }
}

bool UserDatabase::load(const string& filename)
{
    if (m_has_loaded == true)
        return false;

    // infile is created and associated with the specified filename
    ifstream infile(filename);
    
    // If the file opening fails, we will return false
    if (!infile)
    {
        return false;
    }

    // Runs while the input operations on the file are successful
    while (infile)
    {
        /*
        * Format:
        * Person 1's name
        * Person 1's email address
        * Count of number of movies for person 1
        * MovieID #1
        * MovieID #2
        * MovieID #n
        * 
        * Person 2's name
        * Person 2's email address
        * Count of number of movies for person 2
        * MovieID #1
        * MovieID #2
        * MovieID #3
        */

        string name;
        // getline needs a string as its second parameter
        // It takes in the whole line of input and puts it into the string name
        getline(infile, name);
        
        // If fails to read a name, that means there are no entries left
        if (!infile)
            break;

        string email;
        getline(infile, email);
        int number_of_movies;
        // >> Is used to extract characters until it reaches a white space
        infile >> number_of_movies;
        // We now need to skip the white space
        infile.ignore(arbitrarily_large_number, '\n');

        vector<string> watch_history;
        for (int i = 0; i < number_of_movies; i++)
        {
            string movie;
            getline(infile, movie);
            watch_history.push_back(movie);
            // We will transform these entries into lowercase when searching through them via MovieDatabase and Recommender
        }

        User* user = new User(name, email, watch_history);
        m_users.push_back(user);
        m_tree.insert(email, user);

        infile.ignore(arbitrarily_large_number, '\n');
    }

    m_has_loaded = true;
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    auto itr = m_tree.find(email);
    if (itr.is_valid())
        return itr.get_value();
    return nullptr;
}
