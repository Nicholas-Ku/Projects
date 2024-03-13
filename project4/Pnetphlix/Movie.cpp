#include "Movie.h"

#include <string>
#include <vector>
using namespace std;

Movie::Movie(const string& id, const string& title, const string& release_year,
             const vector<string>& directors, const vector<string>& actors,
             const vector<string>& genres, float rating)
{
    // Replace this line with correct code.
}

string Movie::get_id() const
{
    return "DUMMY";  // Replace this line with correct code.
}

string Movie::get_title() const
{
    return "DUMMY";  // Replace this line with correct code.
}

string Movie::get_release_year() const
{
    return "DUMMY";  // Replace this line with correct code.
}

float Movie::get_rating() const
{
    return -999;  // Replace this line with correct code.
}

vector<string> Movie::get_directors() const
{
    return vector<string>();  // Replace this line with correct code.
}

vector<string> Movie::get_actors() const
{
    return vector<string>();  // Replace this line with correct code.
}

vector<string> Movie::get_genres() const
{
    return vector<string>();  // Replace this line with correct code.
}
