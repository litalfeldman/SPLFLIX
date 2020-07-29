
#include <iostream>
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"

using namespace std;

string User::getName() const {
    return this->name;
}

User::User(const string& userName) :history(), name(userName), sumOfLengths(0) ,tagsPopularityMap(){
}

vector<Watchable *>& User::get_history() {
    return history;
}

LengthRecommenderUser::LengthRecommenderUser(const string &name) : User(name) {
}

User *LengthRecommenderUser::clone(const string &newname) {
    ///create the new user
    LengthRecommenderUser *U = new LengthRecommenderUser(newname);
    ///copy the history of the original user
    for (auto &item: this->history) {
        U->pushHistory(item);
    }
    ///copy the map of the original user
    map<std::string, int> *orgMap = this->getTagsMap();
    for (auto x: *orgMap) {
        U->getTagsMap()->insert(x);
    }
    return U;
}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    float average = getSumOfLengths() / history.size();
    return s.getLengthRec(average);
}


RerunRecommenderUser::RerunRecommenderUser(const string &name) : User(name), recIndexAtHist(0) {
}

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    int n = history.size();

    Watchable *toRecommend = s.getActiveUser()->get_history()[(recIndexAtHist+1) % n];
    recIndexAtHist=recIndexAtHist+1 % n;

    return toRecommend;
}

User *RerunRecommenderUser::clone(const string &newname) {
    ///create the new user
    RerunRecommenderUser *U = new RerunRecommenderUser(newname);
    ///copy the history of the original user
    for (auto &item: this->history) {
        U->pushHistory(item);
    }
    ///copy the map of the original user
    map<std::string, int> *orgMap = this->getTagsMap();
    for (auto x: *orgMap) {
        U->getTagsMap()->insert(x);
    }
    return U;
}


Watchable *GenreRecommenderUser::getRecommendation(Session &s) { ////// TODO: problem!!!! we r finding the first max not the max

    map<std::string, int> *mapIt = getTagsMap();

    while (mapIt->size() > 0) {
        string mostPopularGenre = maxPopularity();

        std::vector<Watchable *> content = s.getContent();
        for (auto cont: content) {
            for (auto tag: cont->getTag()) {
                if (tag == mostPopularGenre && !inHistory(cont)) {
                    return cont;
                }
            }
        }
        mapIt->erase(mostPopularGenre);
    }
    return nullptr;

}

GenreRecommenderUser::GenreRecommenderUser(const string &name) : User(name) {
}

User *GenreRecommenderUser::clone(const string &newname) {
    ///create the new user
    GenreRecommenderUser *U = new GenreRecommenderUser(newname);
    ///copy the history of the original user
    for (auto &item: this->history) {
        U->pushHistory(item);
    }
    ///copy the map of the original user
    map<std::string, int> *orgMap = this->getTagsMap();
    for (auto x: *orgMap) {
        U->getTagsMap()->insert(x);
    }
    return U;
}


void User::printHistory() {
    cout << "Watch history for " << name << endl;
    int id = 1;
    for (auto content : history) {
        cout << id << ". " << content->toString() << endl;
        id = id + 1;
    }
}

void User::pushHistory(Watchable *cont) {
    history.push_back(cont);

    sumOfLengths = sumOfLengths + (cont->getLength());

    /// inserting the content's tags into the tags map
    for (auto tag: cont->getTag()) {
        if (tagsPopularityMap.find(tag) == tagsPopularityMap.end())
            tagsPopularityMap.insert(std::pair<std::string, int>(tag, 1));
        else
            tagsPopularityMap[tag]++;
    }
}

bool User::Contains(Watchable *cont) {
    for (size_t i = 0; (i < history.size()); i++)
        if (history[i]->getId() == cont->getId())
            return true;
    return false;
}

long User::getSumOfLengths() {
    return sumOfLengths;
}

bool User::inHistory(Watchable *content) {
    for (auto watch : history) {
        if (watch->getId() == content->getId())
            return true;
    }
    return false;
}

std::map<std::string, int> *User::getTagsMap() {
    return &tagsPopularityMap;
}

///iterates the tagMap to find the most popular tag
std::string User::maxPopularity() {
    string tag;
    int max=0;

    for (auto x : *getTagsMap()){
        if (x.second > max){
            max = x.second;
            tag = x.first;
        }
    }
    return tag;
}


