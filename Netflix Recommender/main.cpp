#include "UserDatabase.h"
#include "User.h"
#include <iostream>
#include <string>
using namespace std;

//////////////////////////i/////////////////////////////////////////////////////
//
// You are free to do whatever you want with this file, since you won't
// be turning it in.  Presumably, you will make changes to help you test
// your classes.  For example, you might try to implement only some User
// member functions to start out with, and so replace our main routine with
// one that simply creates a User and verifies that the member functions you
// implemented work correctly.
//
//////////////////////////i/////////////////////////////////////////////////////


  // If your program is having trouble finding these files. replace the
  // string literals with full path names to the files.  Also, for test
  // purposes, you may want to create some small, simple user and movie
  // data files to makde debuggiing easier, so you can replace the string
  // literals with the names of those smaller files.

const string USER_DATAFILE  = "users.txt";
const string MOVIE_DATAFILE = "movies.txt";

#include <cassert>
#include "MovieDatabase.h"
#include "Movie.h"
#include "Recommender.h"
#include "treemm.h"
#include "Utility.h"
#include<iostream>
#include<fstream>

void find_matches(const Recommender& r, const MovieDatabase& md, const string& user_email, int num_recommendations)
{
	vector<MovieAndRank> recommendations = r.recommend_movies(user_email, num_recommendations);
	
	if (recommendations.empty())
		cout << "We found no movies to recommend, maybe just watch something with YoonA" << endl;

	else
	{
		for (int i = 0; i < num_recommendations; i++)
		{
			const MovieAndRank& mr = recommendations[i];
			Movie* m = md.get_movie_from_id(mr.movie_id);
			cout << i + 1 << ". " << m->get_title() << " (" << m->get_release_year() << ") " << "Rating: " << m->get_rating() << endl;
			cout << "Compatibility Score: " << mr.compatibility_score << endl;
		}
	}
}

int preliminary_database()
{
	UserDatabase udb;
	if (udb.load(USER_DATAFILE) == false)  // In skeleton, load always return false
	{
		cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
		return 1;
	}
	for (;;)
	{
		User* x = udb.get_user_from_email("AbFow2483@charter.net");
		if (x == nullptr)
			cout << "No user in the database has that email address." << endl;
		else
			cout << "Found " << x->get_full_name() << endl;

		cout << "Enter user email address (or quit): ";
		string email;
		getline(cin, email);
		if (email == "quit")
			return 0;
		User* u = udb.get_user_from_email(email);
		if (u == nullptr)
			cout << "No user in the database has that email address." << endl;
		else
			cout << "Found " << u->get_full_name() << endl;
	}
}

void test_user()
{
	vector<string> Cinestra_watch_history;
	Cinestra_watch_history.push_back("Everything Everywhere All at Once");
	Cinestra_watch_history.push_back("Scott Pilgrim vs. The World");
	Cinestra_watch_history.push_back("Ghost in the Shell");
	User Cinestra("Nicholas Ku", "nicholasku@gmail.com", Cinestra_watch_history);
	
	// utility::print_user(Cinestra);
	assert(Cinestra.get_full_name() == "Nicholas Ku");
	assert(Cinestra.get_email() == "nicholasku@gmail.com");
	assert(Cinestra.get_watch_history()[0] == "Everything Everywhere All at Once");
	assert(Cinestra.get_watch_history()[1] == "Scott Pilgrim vs. The World");
	assert(Cinestra.get_watch_history()[2] == "Ghost in the Shell");
	cout << "User test passed." << endl;
}

void test_movie()
{
	vector<string> directors;
	directors.push_back("Director 1");
	directors.push_back("Director 2");
	vector<string> actors;
	actors.push_back("YoonA Lim");
	actors.push_back("Kazuha Nakamura");
	actors.push_back("Momo Hirai");
	vector<string> genres;
	genres.push_back("Genre 1");
	genres.push_back("Genre 2");

	Movie movie1("99999", "Love Rain", "2012", directors, actors, genres, 4.20);
	//utility::print_movie(movie1);

	assert(movie1.get_id() == "99999");
	assert(movie1.get_title() == "Love Rain");
	assert(movie1.get_actors()[0] == "YoonA Lim");
	assert(movie1.get_actors()[1] == "Kazuha Nakamura");
	assert(movie1.get_actors()[2] == "Momo Hirai");
	assert(movie1.get_genres()[0] == "Genre 1");
	assert(movie1.get_genres()[1] == "Genre 2");
	cout << "Movie test passed." << endl;
}

void test_treemm()
{
	vector<string> Cinestra_watch_history;
	Cinestra_watch_history.push_back("Everything Everywhere All at Once");
	Cinestra_watch_history.push_back("Scott Pilgrim vs. The World");
	Cinestra_watch_history.push_back("Ghost in the Shell");

	string email = "nicholasku@gmail.com";
	User Cinestra1("Cinestra1", email, Cinestra_watch_history);
	User Cinestra2("Cinestra2", email, Cinestra_watch_history);
	User Cinestra3("Cinestra3", email, Cinestra_watch_history);
	User Cinestra4("Cinestra4", email, Cinestra_watch_history);
	User Cinestra5("Cinestra1 copy", email, Cinestra_watch_history);
	User Cinestra6("Cinestra1 copy of a copy", email, Cinestra_watch_history);

	TreeMultimap<int, User> tmm;
	tmm.insert(1, Cinestra1);
	tmm.insert(2, Cinestra2);
	tmm.insert(3, Cinestra3);
	tmm.insert(4, Cinestra4);
	tmm.insert(1, Cinestra5);
	tmm.insert(1, Cinestra6);

	auto itr = tmm.find(1);
	assert(itr.get_value().get_full_name() == "Cinestra1");
	itr.advance();
	assert(itr.get_value().get_full_name() == "Cinestra1 copy");
	itr.advance();
	assert(itr.get_value().get_full_name() == "Cinestra1 copy of a copy");

	auto itr2 = tmm.find(2);
	assert(itr2.get_value().get_full_name() == "Cinestra2");

	auto itr3 = tmm.find(5);
	assert(!itr3.is_valid());
	cout << "Treemm test passed." << endl;
}

void test_user_database()
{
	cout << "Initializing user database test (will take forever)" << endl;
	UserDatabase udb;
	assert(udb.load(USER_DATAFILE));
	cout << "Successfully loaded user database" << endl;

	User* a = udb.get_user_from_email("AbFow2483@charter.net");
	assert(a->get_full_name() == "Abdullah Fowler");
	assert(a->get_email() == "AbFow2483@charter.net");
	assert(a->get_watch_history()[0] == "ID22937");

	User* b = udb.get_user_from_email("LOwen62@mail.com");
	assert(b->get_full_name() == "Lian Owen");

	User* c = udb.get_user_from_email("invalid.gmail.com");
	assert(c == nullptr);

	cout << "User database test passed." << endl;
}

void test_movie_database()
{
	cout << "Initializing movie database test (will take forever)" << endl;
	MovieDatabase mdb;
	assert(!mdb.load("nothing")); // Loading nothing
	assert(mdb.load(MOVIE_DATAFILE));
	cout << "Successfully loaded movie database" << endl;

	Movie* a = mdb.get_movie_from_id("ID10782");
	assert(a->get_id() == "ID10782");
	assert(a->get_title() == "Santosh Subramaniam");
	assert(a->get_release_year() == "2008");
	assert(a->get_directors()[0] == "m. raja");
	assert(a->get_actors()[0] == "jayam ravi");
	assert(a->get_actors()[1] == "genelia d'souza");
	assert(a->get_actors()[2] == "prakash raj");
	assert(a->get_actors()[3] == "sayaji shinde");
	assert(a->get_actors()[4] == "geetha");
	assert(a->get_genres()[0] == "action");
	assert(a->get_genres()[1] == "comedy");
	assert(a->get_genres()[2] == "romance");
	assert(a->get_rating() == 5);

	Movie* b = mdb.get_movie_from_id("ID05902");
	assert(b->get_id() == "ID05902");
	assert(b->get_title() == "Lord of Illusions");
	assert(b->get_release_year() == "1995");
	assert(b->get_directors()[0] == "clive barker");
	assert(b->get_actors()[0] == "scott bakula");
	assert(b->get_actors()[1] == "kevin j. o'connor");
	assert(b->get_actors()[2] == "vincent schiavelli");
	assert(b->get_actors()[3] == "famke janssen");
	assert(b->get_genres()[0] == "horror");
	// assert(b->get_rating() == 2.89224); // This doesn't work, for some reason C++ is giving me 2.89224005 instead of the expected 2.89224

	// Trying another movie with a decimal rating
	Movie* c = mdb.get_movie_from_id("ID39216");
	assert(c->get_id() == "ID39216");
	assert(c->get_title() == "No Safe Spaces");
	assert(c->get_release_year() == "2019");
	assert(c->get_directors()[0] == "justin folk");
	assert(c->get_actors()[0] == "dennis prager");
	assert(c->get_actors()[1] == "adam carolla");
	assert(c->get_actors()[2] == "ben shapiro");
	assert(c->get_actors()[3] == "jordan b. peterson");
	assert(c->get_actors()[4] == "dave rubin");
	assert(c->get_genres()[0] == "documentary");
	// assert(c->get_rating() == 3.44444); // This also doesn't work, the stored data is 3.44444443989 instead of 3.44444

	vector<Movie*> movies_with_same_director = mdb.get_movies_with_director("m. raja");
	assert(movies_with_same_director[0]->get_id() == "ID10782");
	vector<Movie*> movies_with_same_actor = mdb.get_movies_with_actor("jayam ravi");
	assert(movies_with_same_actor[0]->get_id() == "ID10782");
	vector<Movie*> movies_with_same_genre = mdb.get_movies_with_genre("action");
	assert(movies_with_same_genre[0]->get_id() == "ID10782");

	Movie* d = mdb.get_movie_from_id("invalid");
	assert(d == nullptr);
	vector<Movie*> failed_search_for_director = mdb.get_movies_with_director("invalid director");
	assert(failed_search_for_director.empty());
	vector<Movie*> failed_search_for_actor = mdb.get_movies_with_actor("invalid actor");
	assert(failed_search_for_actor.empty());
	vector<Movie*> failed_search_for_genre = mdb.get_movies_with_genre("invalid genre");
	assert(failed_search_for_genre.empty());

	MovieDatabase mdb2;
	const string DUPLICATE_MOVIES = "duplicatemovies.txt";
	assert(mdb2.load(DUPLICATE_MOVIES));
	Movie* e = mdb2.get_movie_from_id("ID001");
	assert(e->get_id() == "ID001");
	assert(e->get_title() == "Movie A");

	cout << "Successfully passed movie database test EXCEPT for the weird floating conversion with the last entry" << endl;

	// Testing Edge Cases

}

void test_recommender()
{
	cout << "Initializing Recommender test" << endl;

	UserDatabase udb;
	assert(udb.load(USER_DATAFILE));
	cout << "Loaded Userdatabase" << endl;
	MovieDatabase mdb;
	assert(mdb.load(MOVIE_DATAFILE));
	cout << "Loaded Moviedatabase" << endl;

	const int num_rec = 3;
	Recommender r(udb, mdb);
	vector<MovieAndRank> Ab_recommendations = r.recommend_movies("AbFow2483@charter.net", num_rec);

	find_matches(r, mdb, "AbFow2483@charter.net", num_rec);

	string first_movie_id = Ab_recommendations[0].movie_id;
	Movie* first_movie = mdb.get_movie_from_id(first_movie_id);
	assert(first_movie->get_title() == "Ghosts on the Loose");
	assert(Ab_recommendations[0].compatibility_score == 120);

	string second_movie_id = Ab_recommendations[1].movie_id;
	Movie* second_movie = mdb.get_movie_from_id(second_movie_id);
	assert(second_movie->get_title() == "Batman vs. Two-Face");
	assert(Ab_recommendations[1].compatibility_score == 110);

	string third_movie_id = Ab_recommendations[2].movie_id;
	Movie* third_movie = mdb.get_movie_from_id(third_movie_id);
	assert(third_movie->get_title() == "Kit Carson");
	assert(Ab_recommendations[2].compatibility_score == 90);

	// find_matches(r, mdb, "AbFow2483@charter.net", num_rec);

	cout << "Passed initial Recommender test" << endl;
}

void test_recommender_edgecases()
{
	const int num_rec = 3;

	const string NO_WATCH_HISTORY = "userstest1.txt";
	const string EMPTY_MOVIES = "moviestest1.txt";

	cout << "Initializing edge case tests for Recommender" << endl;
	UserDatabase user_database_test1;
	assert(user_database_test1.load(NO_WATCH_HISTORY));
	User* a = user_database_test1.get_user_from_email("yoona.lim@example.com");
	assert(a->get_full_name() == "YoonA Lim");
	MovieDatabase movie_database_test1;
	assert(movie_database_test1.load(EMPTY_MOVIES));
	Recommender r_test1(user_database_test1, movie_database_test1);
	vector<MovieAndRank> yoona_recommendations = r_test1.recommend_movies("yoona.lim@example.com", num_rec);
	assert(yoona_recommendations.empty());

	const string USER_WATCHED_ALL_MOVIES = "userstest2.txt";
	const string ALL_WATCHABLE_MOVIES = "moviestest2.txt";
	UserDatabase user_database_test2;
	assert(user_database_test2.load(USER_WATCHED_ALL_MOVIES));
	MovieDatabase movie_database_test2;
	assert(movie_database_test2.load(ALL_WATCHABLE_MOVIES));
	Recommender r_test2(user_database_test2, movie_database_test2);
	vector<MovieAndRank> yoona_recommendations2 = r_test2.recommend_movies("yoona.lim@example.com", num_rec);
	assert(yoona_recommendations2.empty());

	const string SAME_COMPATIBILITY_SCORE = "moviestest3.txt";
	MovieDatabase movie_database_test3;
	assert(movie_database_test3.load(SAME_COMPATIBILITY_SCORE));
	Recommender r_test3(user_database_test2, movie_database_test3);
	vector<MovieAndRank> yoona_recommendations3 = r_test3.recommend_movies("yoona.lim@example.com", num_rec);
	assert(yoona_recommendations3[0].movie_id == "ID006");
	assert(yoona_recommendations3[1].movie_id == "ID007");

	cout << "Finished recommender edge cases";
}

void testing_recommender()
{
	const int num_rec = 3;
	const string USER_WATCHED_ALL_MOVIES = "userstest2.txt";
	UserDatabase user_database_test2;
	assert(user_database_test2.load(USER_WATCHED_ALL_MOVIES));
	const string SAME_COMPATIBILITY_SCORE = "moviestest3.txt";
	MovieDatabase movie_database_test3;
	assert(movie_database_test3.load(SAME_COMPATIBILITY_SCORE));
	Recommender r_test3(user_database_test2, movie_database_test3);
	vector<MovieAndRank> yoona_recommendations3 = r_test3.recommend_movies("yoona.lim@example.com", num_rec);
	assert(yoona_recommendations3[0].movie_id == "ID006");
	assert(yoona_recommendations3[1].movie_id == "ID007");
}

int main()
{
	test_user();
	test_movie();
	test_treemm();
	test_user_database();
	// test_movie_database();
	// test_recommender();
	// test_recommender_edgecases();
	// testing_recommender();
}
