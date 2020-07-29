using namespace std;

#include <fstream>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/json.hpp"
#include "../include/User.h"
#include <vector>
#include <algorithm>
#include <iostream>

using json = nlohmann::json;

Session::Session(const string &configFilePath) : content(), actionsLog(), userMap(), activeUser(nullptr), cmd() { /////

// read the json file to create the content vector

    ifstream i(configFilePath);
    json j;
    i >> j;

    vector <json> movies = j.at("movies").get < vector < json >> ();
    int id = 0;
    for (auto &movie : movies) {
        string movie_name = movie.at("name").get<string>();
        int movie_length = movie.at("length").get<int>();
        vector <string> movie_tags = movie.at("tags").get < vector < string >> ();

        //push the movie item into the vector content
        Movie *toPush = new Movie(++id, movie_name, movie_length, movie_tags);
        this->content.push_back(toPush);
    }
    vector <json> tv_series = j.at("tv_series").get < vector < json >> ();
    for (auto &tv_serie : tv_series) {
        string series_name = tv_serie.at("name").get<string>();
        int episode_length = tv_serie.at("episode_length").get<int>();
        vector<int> seasons = tv_serie.at("seasons").get < vector < int >> ();
        vector <string> series_tags = tv_serie.at("tags").get < vector < string >> ();

        //push each episode of the series into the vector content
        size_t s_num = 1;
        int s_num2 = 1;
        for (int x : seasons) {
            for (int i = 1; i <= x; i++) {
                Episode *toPush = new Episode(++id, series_name, episode_length, s_num2, i, series_tags);
                this->content.push_back(toPush);
                if (s_num == seasons.size() && x == i)
                    toPush->setNextEpisodeId(-1);

            }
            s_num++;
            s_num2++;
        }

    }
    //create default user
    BaseAction *ba = new CreateUser("default", "len");
    ba->act(*this);
    delete (ba);
    ba = new ChangeActiveUser("default");
    ba->act(*this);
    delete (ba);

}


void Session::start() {
    cout << "SPLFLIX is now on!" << endl;

    vector <string> words;

    do {
        getline(cin, cmd);
        words = make_vector(cmd);
        BaseAction *action;
        if (words[0] == "createuser") {
            action = new CreateUser(words[1], words[2]);
        } else if (words[0] == "changeuser") {
            action = new ChangeActiveUser(words[1]);
        } else if (words[0] == "deleteuser") {
            action = new DeleteUser(words[1]);
        } else if (words[0] == "dupuser") {
            action = new DuplicateUser(words[1], words[2]);
        } else if (words[0] == "content") {
            action = new PrintContentList();
        } else if (words[0] == "watchhist") {
            action = new PrintWatchHistory();
        } else if (words[0] == "watch") {
            action = new Watch(words[1]);
        } else if (words[0] == "log") {
            action = new PrintActionsLog();
        } else if (words[0] == "exit") {
            action = new Exit();
        } else {
            continue;
        }
        actionsLog.push_back(action);
        action->act(*this);

        if (words[0] != "watch") {
        }

    } while (words[0] != "exit");

}

vector<string> Session::make_vector(string s) {
    string word;
    vector <string> v;
    for (char i : s) {
        if (i != ' ')
            word = word + i;

        else {
            v.push_back(word);
            word = "";
        }
    }
    if (!word.empty())
        v.push_back(word);
    return v;
}


string Session::makeUser(string userName, string algo) {
    //user name is invalid
    if (userMap.count(userName) != 0)
        return "userError";
    if (algo != "len" && algo != "rer" && algo != "gen")
        return "algoError";

    User *user = nullptr;
    if (algo == "len")
        user = new LengthRecommenderUser(userName);
    else if (algo == "rer")
        user = new RerunRecommenderUser(userName);
    else if (algo == "gen")
        user = new GenreRecommenderUser(userName);
    if (user)
        userMap[user->getName()] = user;
    return "";
}

string Session::changeUser(string userName) {
    if (userMap.count(userName) != 0) { //user name is valid
        activeUser = userMap.find(userName)->second;
        return "";
    } else return "error";
}

string Session::deleteUser(string userName) {
    if (userMap.count(userName) != 0) { //user name is valid
        delete userMap.find(userName)->second;//delete the user from the heap
        userMap.erase(userName);//delete the user from the userMap
        return "";
    } else return "error";
}

string Session::duplicateUser(string originalUser, string newUser) {
    //original username does not exist
    if (userMap.count(originalUser) == 0)
        return "originalUser";
    //new username is already in use
    if (userMap.count(newUser) != 0)
        return "newUser";
    //copy the new user and inserts the new user to the users map
    User *u = (userMap.find(originalUser)->second)->clone(newUser);
    userMap.insert({newUser, u});

    return "";
}

void Session::printContent() {
    for (auto x : content)
        cout << x->getId() << ". " << x->toString() << " " << x->getLength() << " minuts [" << x->tagsString() << "]"
             << endl;
}

User *Session::getActiveUser() const {
    return activeUser;
}

void Session::printLog() {
    for (size_t i = 0; i < actionsLog.size(); i++) {//iterate action log
        size_t pos = actionsLog.size() - 1 - i;
        if (actionsLog[pos]->getStatus() == ERROR)
            cout << actionsLog[pos]->toString() << ": " << actionsLog[pos]->printMsg() << endl;
        else
            cout << actionsLog[pos]->toString() << endl;
    }
}


void Session::watch(string id) {
    int index = stoi(id) - 1;//string ==> index
    Watchable *newContent = content.at(index);
    //actionsLog.push_back(action);

    cout << "Watching " + newContent->toString() << endl; //print the action
    activeUser->pushHistory(newContent);//push the content that the user watch to the history

    Watchable *recommendation = newContent->getNextWatchable(*this);//save the recommend content
    cout << "We recommend watching " + recommendation->toString() + ", continue watching? [y/n]" << endl;
    getline(cin, cmd);
    if (cmd == "y") {
        string recId = to_string(recommendation->getId());
        BaseAction *ba = new Watch(recId);
        actionsLog.push_back(ba);
        ba->act(*this);
    }
}

Watchable *Session::getContentById(long id) {
    return content.at(id - 1);
}

Watchable *Session::getLengthRec(float average) {
    float min = HUGE_VAL;
    Watchable *output;
    for (auto x : content) {
        if (abs(average - x->getLength()) < min && !(activeUser->Contains(x))) {
            output = x;
            min = abs(average - x->getLength());
        }
    }
    return output;
}

///destructor
Session::~Session() {
    clear();
}

std::vector<Watchable *> Session::getContent() const {
    return content;
}

///copy constructor
Session::Session(const Session &copySession)
        : content(), actionsLog(), userMap(), activeUser(nullptr), cmd(copySession.cmd) {
    copy(copySession);
}

void Session::copy(const Session &copySession) {
    ///content
    for (auto x: copySession.content) {
        content.push_back(x->clone());
    }
    ///actionLog
    for (auto x: copySession.actionsLog) {
        actionsLog.push_back(x->clone());
    }
    ///User map
    for (auto x: copySession.userMap) {
        userMap.insert({x.first, x.second->clone(x.first)});
    }
    ///active user
    string otherName = copySession.activeUser->getName();
    activeUser = userMap[otherName];

    ///fix history pointers
    for (auto x:userMap) {
        vector < Watchable * > &history = x.second->get_history();
        for (auto &i : history) {
            i = getContentById(i->getId());
        }
    }
}

Session &Session::operator=(const Session &other) {
    if (this != &other) {
        clear();
        copy(other);
    }
    return *this;
}

Session &Session::operator=(Session &&other) {
    if (this != &other) {
        clear();
        content = move(other.content);
        actionsLog = move(other.actionsLog);
        userMap = move(other.userMap);
        activeUser = other.activeUser;
        cmd = other.cmd;
    }
    return *this;
}

void Session::clear() {
    ///deep delete content
    for (auto x: content) {
        delete x;
    }
    content.clear();

    ///deep delete action log
    for (auto x: actionsLog) {
        delete x;
    }
    actionsLog.clear();

    ///deep delete usermap
    for (auto x: userMap) {
        delete x.second;
    }
    userMap.clear();

    ///delete active user
    activeUser = nullptr;
}

Session::Session(Session &&copySession) : content(move(copySession.content)), actionsLog(move(copySession.actionsLog)),
                                          userMap(move(copySession.userMap)),
                                          activeUser(copySession.activeUser),
                                          cmd(copySession.cmd) {
    copySession.activeUser = nullptr;
}


