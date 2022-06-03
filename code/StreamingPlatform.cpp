#include "StreamingPlatform.hpp"

#include <algorithm>

void StreamingPlatform::ProcessLogin(std::string& login) {
	std::replace_if(login.begin(), login.end(), [](char c)
		{
			return !std::isalnum(c);
		}, '_');
}

User* StreamingPlatform::AddUser(std::string login, std::string firstName, std::string lastName) {
	ProcessLogin(login);

	const auto result = this->users.insert(std::make_pair(login, User{ firstName, lastName }));
	if (result.second) {
		return &result.first->second;
	}
	return nullptr;
}

User* StreamingPlatform::GetUser(std::string login) {
	ProcessLogin(login);

	auto it = this->users.find(login);
	if (it != this->users.end()) {
		return &it->second;
	}
	return nullptr;
}

void StreamingPlatform::PrintUsers(std::ostream& out) const {
	int idx = 0;
	for (const auto& pair : this->users) {
		out << ++idx << ": " << pair.first << " - " << pair.second << '\n';
	}
}


bool MovieCompare::operator()(const Movie& first, const Movie& second) const
{
	return first.title == second.title ? (first.director == second.director ? first.releaseYear < second.releaseYear : first.director < second.director) : first.title < second.title;
}

bool StreamingPlatform::AddMovie(const Movie& movie)
{
	return movies.insert(movie).second;
}

void StreamingPlatform::PrintMovies(std::ostream& out) const
{
	std::for_each(movies.begin(), movies.end(), [&out](const Movie& movie)
		{
			out << movie << '\n';
		});
}

int StreamingPlatform::CountMoviesInYears(int startYear, int endYear) const
{
	return std::count_if(movies.begin(), movies.end(), [startYear, endYear](const Movie& movie)
		{
			return startYear <= movie.releaseYear && movie.releaseYear <= endYear;
		});
}

std::map<std::string, std::vector<const Movie*>> StreamingPlatform::GetMoviesByCategories() const
{
	std::map<std::string, std::vector<const Movie*>> categorised;
	std::for_each(movies.begin(), movies.end(), [&categorised](const Movie& movie)
		{
			categorised[movie.category].push_back(&movie);
		});

	return categorised;
}

std::list<const Movie*> StreamingPlatform::GetRecommendedMovies(const User& user, int matchesCount) const
{
	std::set<const Movie*> retset;
	std::list<const Movie*> ret;
	auto watched = user.GetWatchedMoviesSet();
	std::for_each(users.begin(), users.end(), [&](const std::pair<std::string, User>& entry)
		{
			if (!(entry.second == user))
			{
				auto watch = entry.second.GetWatchedMoviesSet();
				std::set<const Movie*> intersect;
				std::set_intersection(watched.begin(), watched.end(), watch.begin(), watch.end(), std::inserter(intersect,intersect.begin()));

				if(intersect.size() >= matchesCount)
				{
					std::set<const Movie*> diff;
					std::set_difference(watch.begin(), watch.end(), watched.begin(), watched.end(), std::inserter(diff,diff.begin()));
					std::copy(diff.begin(), diff.end(), std::inserter(retset,retset.begin()));
				}
			}
		});

	std::copy(retset.begin(), retset.end(), std::back_inserter(ret));

	ret.sort([](const auto& first, const auto& second)
		{
			return first->GetRating() > second->GetRating();
		});
	return ret;
}
