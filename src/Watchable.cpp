#include "../include/Watchable.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include "../include/User.h"
#include "../include/Session.h"

using namespace std;


Watchable::Watchable(long id, int length, const vector<string> &tags) : id(id), length(length), tags(tags) {}

long Watchable::getId() const {
    return id;
}

vector<string> Watchable::getTag() const {
    return tags;
}

int Watchable::getLength() const {
    return length;
}

string Watchable::tagsString() const {
    ostringstream vts;
    if (!tags.empty()) {
        copy(tags.begin(), tags.end() - 1,
             ostream_iterator<string>(vts, ", "));
        vts << tags.back();
    }
    return vts.str();
}


Movie::Movie(long id, const string &name, int length, const vector<string> &tags) :
        Watchable(id, length, tags), name(name) {}

string Movie::toString() const {
    return name;
}


Watchable *Movie::getNextWatchable(Session &s) const {
    Watchable *next = s.getActiveUser()->getRecommendation(s);
    return next;
}

Watchable *Movie::clone() {
    return new Movie(*this);
}


Episode::Episode(long id, const string &seriesName, int length, int season, int episode,
                 const vector<string> &tags) :
        Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode), nextEpisodeId(id + 1) {}

string Episode::toString() const {
    string output = seriesName + " S" + to_string(season) + "E"
                    + to_string(episode);
    return output;
}

Watchable *Episode::getNextWatchable(Session &s) const {
    /// if there is no next episode
    if (nextEpisodeId == -1) {
        Watchable *next = s.getActiveUser()->getRecommendation(s);//return content by rec
        return next;
    } else {
        ///if there is next episode
        return s.getContentById(nextEpisodeId);//return next episode
    }

}

void Episode::setNextEpisodeId(long next) {
    nextEpisodeId = next;
}

Watchable *Episode::clone() {
    return new Episode (*this);
}