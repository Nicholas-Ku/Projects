#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"

#include <string>
#include <vector>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    m_user_database = &user_database;
    m_movie_database = &movie_database;
    // Replace this line with correct code.
}

void Recommender::add_points_to_movies(const vector<Movie*> &input_vector, const int num_points, unordered_map<string, int> &input_map) const
{
    for (int i = 0; i < input_vector.size(); i++)
    {
        const string kth_movie_id_with_same_attribute = input_vector[i]->get_id();
        input_map[kth_movie_id_with_same_attribute] = num_points;
    }
}

// Should be O(n * mx log x)
unordered_map<string, int> Recommender::create_unordered_map_linking_movies_with_compatibility(const User *current_user) const
{
    // Unordered map connecting Movie IDs and compatbility scores
    // Not a member variable, this is a LOCAL variable
    unordered_map<string, int> possible_recommend_movies;

    // Grab user from their email
    const vector<string> current_user_watch_history = current_user->get_watch_history();

    // We are iterating through each of the movies that a user has watched
    for (int i = 0; i < current_user_watch_history.size(); i++)
    {
        const Movie* current_movie = m_movie_database->get_movie_from_id(current_user_watch_history[i]);

        if (current_movie == nullptr)
            break;

        // Getting movies with the same director should take md + log d
        vector<Movie*> movies_with_same_director;
        // Try not to use two different functions inside the for loop, define the limit outside so you don't have to go through it each time
        for (int j = 0; j < current_movie->get_directors().size(); j++)
        {
            const string jth_director = (*current_movie).get_directors()[j];
            movies_with_same_director = m_movie_database->get_movies_with_director(jth_director);

            add_points_to_movies(movies_with_same_director, SAME_DIRECTOR_POINTS, possible_recommend_movies);
        }

        // Getting movies with the same actor should take ma + log a
        vector<Movie*> movies_with_same_actor;
        for (int j = 0; j < current_movie->get_actors().size(); j++)
        {
            const string jth_actor = (*current_movie).get_actors()[j];
            movies_with_same_actor = m_movie_database->get_movies_with_actor(jth_actor);

            add_points_to_movies(movies_with_same_actor, SAME_ACTOR_POINTS, possible_recommend_movies);
        }

        // Getting movies with the same genre should take mg + log g
        vector<Movie*> movies_with_same_genre;
        for (int j = 0; j < current_movie->get_genres().size(); j++)
        {
            const string jth_genre = (*current_movie).get_genres()[j];
            movies_with_same_genre = m_movie_database->get_movies_with_actor(jth_genre);

            add_points_to_movies(movies_with_same_genre, SAME_GENRE_POINTS, possible_recommend_movies);
        }

    }

    return possible_recommend_movies;
}

// When you pass an instance of a structure to sort as the third parameter, the algorithm internally invokes the operator() within the structure to compare elements
// Hence we are overloading the () operator
bool Recommender::Comparison_struct::operator()(const MovieAndRank& a, const MovieAndRank& b) const
{
    // First check based on compatibility
    if (a.compatibility_score != b.compatibility_score)
        return a.compatibility_score > b.compatibility_score;

    // Then check based on rating
    if (recommender->m_movie_database->get_movie_from_id(a.movie_id)->get_rating() != recommender->m_movie_database->get_movie_from_id(b.movie_id)->get_rating())
        return recommender->m_movie_database->get_movie_from_id(a.movie_id)->get_rating() > recommender->m_movie_database->get_movie_from_id(b.movie_id)->get_rating();

    // Then check based on alpha sorting
    else
        // Doesn't actually compare by movie title
        return a.movie_id < b.movie_id;
}

/*
std::vector<MovieAndRank> Recommender::create_descending_order_vector_given_unordered_map(const User* &current_user, int movie_count, unordered_map<string, int> possible_recommend_movies) const
{
    vector<MovieAndRank> compiled_MovieAndRank;
    for (auto itr = possible_recommend_movies.begin(); itr != possible_recommend_movies.end(); itr++)
    {
        string itr_id = itr->first;
        int itr_score = itr->second;
        MovieAndRank itr_MovieAndRank(itr_id, itr_score);

        vector<string> temp_watch_history = current_user->get_watch_history();
        bool already_seen_movie = (find(temp_watch_history.begin(), temp_watch_history.end(), itr_id) == temp_watch_history.end());
        if (already_seen_movie)
            continue;

        // Base Case, we will initially start with an empty vector and have to populate it
        else if (compiled_MovieAndRank.empty())
        {
            compiled_MovieAndRank.push_back(itr_MovieAndRank);
            continue;
        }

        for (int i = 0; i < compiled_MovieAndRank.size(); i++)
        {
            // Compare the iterated movie with the ith movie already in the compiled_MovieAndRank
            if (compare_MovieAndRank(itr_MovieAndRank, compiled_MovieAndRank[i]))
            {
                compiled_MovieAndRank.insert(compiled_MovieAndRank.begin() + i, itr_MovieAndRank);
                break;
            }

            else if (i == compiled_MovieAndRank.size() - 1)
            {
                compiled_MovieAndRank.push_back(itr_MovieAndRank);
                break;
            }
        }
    }
}
*/


std::vector<MovieAndRank> Recommender::create_descending_order_vector_given_unordered_map(const User* current_user, const unordered_map<string, int> possible_recommend_movies, const int num_recs) const
{
    vector<MovieAndRank> compiled_movie_and_rank;
    
    const vector<string> watch_history_vector = current_user->get_watch_history();
    unordered_set<string> watch_history_os;
    for (int i = 0; i < watch_history_vector.size(); i++)
        watch_history_os.insert(watch_history_vector[i]);

    for (auto itr = possible_recommend_movies.begin(); itr != possible_recommend_movies.end(); itr++)
    {
        string itr_id = itr->first;
        int itr_score = itr->second;
        const MovieAndRank itr_MovieAndRank(itr_id, itr_score);

        const bool already_seen_movie = (watch_history_os.find(itr_id) != watch_history_os.end());
        // Use an unordered set or define the set outside so the find is faster

        if (already_seen_movie)
            continue;

        else
            compiled_movie_and_rank.push_back(itr_MovieAndRank);
    }

    // Comparison_struct(this) is creating an instance of the Comparison_struct using its constructor that has been initialized with this, which is a pointer to the current Recommender using it
    // When you pass an instance of a structure to sort as the third parameter, the algorithm internally invokes the operator() within the structure to compare elements
    Comparison_struct comparison_object(this);
    sort(compiled_movie_and_rank.begin(), compiled_movie_and_rank.end(), comparison_object);

    vector<MovieAndRank> only_first_x_movies;
    for (int i = 0; i < num_recs; i++)
    {
        if (i >= compiled_movie_and_rank.size())
            break;

        else
        {
            only_first_x_movies.push_back(compiled_movie_and_rank[i]);
        }
    }

    return only_first_x_movies;
}





vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    if (movie_count <= 0)
    {
        return vector<MovieAndRank>();
    }

    User* current_user = m_user_database->get_user_from_email(user_email);
    if (current_user == nullptr)
    {
        return vector<MovieAndRank>();
    }

    // Unordered map connecting Movie IDs and compatbility scores
    // O(n * mx log x)
    unordered_map<string, int> m_possible_recommend_movies = create_unordered_map_linking_movies_with_compatibility(current_user);

    // Descending order vector of MovieAndRank, which is a structure that has both Movie IDs and compatibility scores
    // Should take O(n * log n)
    vector<MovieAndRank> descending_vector = create_descending_order_vector_given_unordered_map(current_user, m_possible_recommend_movies, movie_count);

    return descending_vector;
}
