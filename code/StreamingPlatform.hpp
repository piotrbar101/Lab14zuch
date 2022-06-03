#pragma once

#include <map>
#include <list>
#include <set>
#include <vector>

#include "User.hpp"
#include "Movie.hpp"

struct MovieCompare
{
	bool operator() (const Movie& first, const Movie& second) const;
};

class StreamingPlatform {
private:
	// Etap 1: Dodaj std::set z filmami sortowanymi wg. kolejnosci: tytul, rezyser, rok wydania
	std::set<Movie, MovieCompare> movies;
	std::map<std::string, User> users;

	static void ProcessLogin(std::string& login);

public:
	// Etap 1	
	bool AddMovie(const Movie& movie);
	void PrintMovies(std::ostream& out) const;


	// Etap 2
	int CountMoviesInYears(int startYear, int endYear) const;
	std::map<std::string, std::vector<const Movie*>> GetMoviesByCategories() const;


	User* AddUser(std::string login, std::string firstName, std::string lastName);
	User* GetUser(std::string login);
	void PrintUsers(std::ostream& out) const;


	// Etap 6
	std::list<const Movie*> GetRecommendedMovies(const User& user, int matchesCount) const;
};