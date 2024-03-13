#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>

#include "Movie.h"
#include "treemm.h"

class MovieDatabase
{
  public:
    MovieDatabase();
    ~MovieDatabase() {};
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
      bool m_has_loaded;
      std::vector<Movie*> m_movies;
      TreeMultimap<std::string, Movie*> m_id_movie_tree;
      TreeMultimap<std::string, Movie*> m_director_movie_tree;
      TreeMultimap<std::string, Movie*> m_actor_movie_tree;
      TreeMultimap<std::string, Movie*> m_genre_movie_tree;

      void turn_line_into_vector(const std::string &entire_text_line, std::vector<std::string> &input_vector) const;
      std::string transform_to_lowercase(const std::string &input) const;
      const int arbitrarily_large_number = 10000;
};

#endif // MOVIEDATABASE_INCLUDED
