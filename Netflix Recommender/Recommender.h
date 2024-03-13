#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>

#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "Movie.h"
#include "User.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    ~Recommender() {};
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
      const UserDatabase* m_user_database;
      const MovieDatabase* m_movie_database;

      // Make it static
      static const int SAME_DIRECTOR_POINTS = 20;
      static const int SAME_ACTOR_POINTS = 30;
      static const int SAME_GENRE_POINTS = 1;

      std::unordered_map<std::string, int> create_unordered_map_linking_movies_with_compatibility(const User* current_user) const;
      std::vector<MovieAndRank> create_descending_order_vector_given_unordered_map(const User* current_user, const std::unordered_map<std::string, int> possible_recommend_movies, const int num_recs) const;

      // Cannot simply use a privately defined comparison for sort
      // Sort needs to try calling it and it cannot because it is a privately defined function without any idea of other private components
      // By feeding it a privately defined structure though, we can access private components defined by the Recommender class
      struct Comparison_struct
      {
          const Recommender* recommender;

          Comparison_struct(const Recommender* rec)
              :recommender(rec)
          {

          }

          // Need to make a separate constructor to have access to recommender and the movie database
          // Otherwise could have used a static private function
          bool operator()(const MovieAndRank& a, const MovieAndRank& b) const;
      };

      void add_points_to_movies(const vector<Movie*>& input_vector, const int num_points, unordered_map<string, int> &input_map) const;
};

#endif // RECOMMENDER_INCLUDED
