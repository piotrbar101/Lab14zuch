#include "User.hpp"

#include <algorithm>

#include "Movie.hpp"


User::User(std::string firstName, std::string lastName) :
	firstName{ firstName }, lastName{ lastName } 
{
}

std::ostream& operator << (std::ostream& out, const User& user) {
	user.Print(out, true);
	return out;
}



void User::Print(std::ostream& out, bool printMovies) const {
	out << this->firstName << " " << this->lastName;

	if (printMovies) {
		out << ":" << '\n' << "  watched movies:" << '\n';
		
		int idx = 0;
		for (auto movie : this->watchingHistory) {
			out << ++idx << ": ";

			movie->Print(out, true, false);

			// Etap 4: Uzupe³nij o wypisywanie oceny filmu
			auto it = ratings.find(movie);
			if (it != ratings.end()) {
				out << " rated " << it->second << "/5";
			}

			out << '\n';
		}
	}
}

void User::WatchMovie(const Movie& movie)
{
	watchingHistory.push_front(&movie);
}

void User::ClearHistory()
{
	watchingHistory.clear();
}

std::list<const Movie*> User::GetWatchedMovies() const
{
	auto ret(watchingHistory);
	std::reverse(ret.begin(), ret.end());
	return ret;
}

void User::RateLastMovie(int rating)
{
	if (watchingHistory.empty())
		return;

	auto last = watchingHistory.front();
	auto it = ratings.find(last);
	if(it == ratings.end() )
	{
		if(rating >= 0)
		{
			last->AddRating(rating);
			ratings[last] = rating;
		}
	}
	else
	{
		if (rating < 0)
		{
			last->RemoveRating(it->second);
			ratings.erase(it);
		}
		else
		{
			last->UpdateRating(it->second, rating);
			it->second = rating;
		}
	}
}

std::set<const Movie*> User::GetWatchedMoviesSet() const
{
	std::set<const Movie*> ret;
	std::copy(watchingHistory.begin(), watchingHistory.end(), std::inserter(ret, ret.begin()));
	return ret;
}

bool User::operator==(const User& other) const
{
	return this->firstName == other.firstName && this->lastName == other.lastName;
}

