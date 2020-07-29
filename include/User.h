#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>

class Watchable;

class Session;

class User {
public:
    User(const std::string &name);

    virtual ~User() = default;

    User(const User &other) = default;

    User(User &&other) = default;

    User &operator=(const User &other) = default;

    User &operator=(User &&other) = default;

    virtual Watchable* getRecommendation(Session &s) = 0;

    virtual User *clone(const std::string &newname) = 0;

    std::string getName() const;

    std::vector<Watchable *> &get_history();

    void printHistory();

    void pushHistory(Watchable *cont);

    bool Contains(Watchable *cont);

    long getSumOfLengths();

    bool inHistory(Watchable *content);

    std::map<std::string, int> *getTagsMap();

    std::string maxPopularity();

protected:
    std::vector<Watchable *> history;
private:
    const std::string name;
    long sumOfLengths;
    std::map<std::string, int> tagsPopularityMap;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string &name);

    virtual Watchable *getRecommendation(Session &s);

    virtual User *clone(const std::string &newname);

private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string &name);

    virtual Watchable *getRecommendation(Session &s);

    virtual User *clone(const std::string &newname);

private:
    size_t recIndexAtHist;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string &name);

    virtual Watchable *getRecommendation(Session &s);

    virtual User *clone(const std::string &newname);

private:
};

#endif