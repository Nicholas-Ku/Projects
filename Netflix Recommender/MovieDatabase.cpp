#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>

#include<fstream>
#include<iostream>

using namespace std;

MovieDatabase::MovieDatabase()
{
    // Replace this line with correct code.
    m_has_loaded = false;
}

string MovieDatabase::transform_to_lowercase(const string &input) const
{
    string all_lowercase_string;
    for (int i = 0; i < input.size(); i++)
        all_lowercase_string += tolower(input[i]);
    return all_lowercase_string;
}

// Takes O(n) time
void MovieDatabase::turn_line_into_vector(const string &entire_text_line, vector<string> &input_vector) const
{
    string new_object;

    for (int i = 0; i < entire_text_line.length(); i++)
    {
        if (entire_text_line[i] == ',' || i == entire_text_line.length() - 1)
        {
            if (i == entire_text_line.length() - 1)
                new_object += entire_text_line[i];

            new_object = transform_to_lowercase(new_object);
            input_vector.push_back(new_object);
            new_object = "";
            continue;
        }

        new_object += entire_text_line[i];
    }
}

// Traverses through n movies and then inserts its elements into 4 separate multimap trees (ID, actors, directors, genres)
bool MovieDatabase::load(const string& filename)
{
    if (m_has_loaded)
        return false;

    // Declaring an istream variable named infile used to read the file name
    ifstream infile(filename);
    
    if (!infile)
        return false;

    // While it can continue to read from infile
    while (infile)
    {
        string id;
        getline(infile, id);
        
        string uppercase_id;
        for (int i = 0; i < id.size(); i++)
            uppercase_id += toupper(id[i]);
        id = uppercase_id;

        // If it fails to read name, that means there are no entries left
        if (!infile)
            break;

        Movie* existing_movie = get_movie_from_id(id);
        if (existing_movie != nullptr)
        {
            string skip;
            getline(infile, skip); // Skip Title
            getline(infile, skip); // Skip Year
            getline(infile, skip); // Skip Directors
            getline(infile, skip); // Skip Actors
            getline(infile, skip); // Skip Genres
            infile.ignore(arbitrarily_large_number, '\n');
            continue;
        }

        string title;
        getline(infile, title);

        string year;
        getline(infile, year);

        string directors;
        // This gets the entire string of directors
        getline(infile, directors);
        vector <string> v_directors;
        turn_line_into_vector(directors, v_directors); // Takes O(N) time

        string actors;
        getline(infile, actors);
        vector<string> v_actors;
        turn_line_into_vector(actors, v_actors);

        string genres;
        getline(infile, genres);
        vector<string> v_genres;
        turn_line_into_vector(genres, v_genres);

        float rating;
        infile >> rating;
        infile.ignore(arbitrarily_large_number, '\n');

        // Now creating the movie and inserting each of its elements into respective trees
        Movie* movie = new Movie(id, title, year, v_directors, v_actors, v_genres, rating);
        m_movies.push_back(movie);
        m_id_movie_tree.insert(id, movie);
        for (int i = 0; i < v_directors.size(); i++)
            m_director_movie_tree.insert(v_directors[i], movie);
        for (int i = 0; i < v_actors.size(); i++)
            m_actor_movie_tree.insert(v_actors[i], movie);
        for (int i = 0; i < v_genres.size(); i++)
            m_genre_movie_tree.insert(v_genres[i], movie);

        infile.ignore(arbitrarily_large_number, '\n');
    }

    m_has_loaded = true;
    return true;
}

// Takes log m since it simply uses a binary search tree to find movies
Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    auto movie = m_id_movie_tree.find(id);
    if (movie.is_valid())
        return movie.get_value();
    return nullptr;  // Replace this line with correct code.
}


// Takes log(d) * m based time since it first finds all director in a BST (log d time) and then adds all of their movies to a vector (m time)
vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    auto itr = m_director_movie_tree.find(transform_to_lowercase(director));
    vector<Movie*> all_movies_with_director;
    while (itr.is_valid())
    {
        all_movies_with_director.push_back(itr.get_value());
        itr.advance();
    }

    return all_movies_with_director;  // Replace this line with correct code.
}

// Takes log(a) * m for similar reasons as above
vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    auto itr = m_actor_movie_tree.find(transform_to_lowercase(actor));
    vector<Movie*> all_movies_with_actor;
    while (itr.is_valid())
    {
        all_movies_with_actor.push_back(itr.get_value());
        itr.advance();
    }
    return all_movies_with_actor;  // Replace this line with correct code.
}

// Takes log(g) * m for similar reasons as above
vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    auto itr = m_genre_movie_tree.find(transform_to_lowercase(genre));
    vector<Movie*> all_movies_with_genre;
    while (itr.is_valid())
    {
        all_movies_with_genre.push_back(itr.get_value());
        itr.advance();
    }
    return all_movies_with_genre;  // Replace this line with correct code.
}
