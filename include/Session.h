#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;

class Watchable;

class Session {
public:
    Session(const std::string &configFilePath);

    ~Session();

    Session(const Session &copySession);

    Session(Session &&copySession);

    Session &operator=(const Session &other);

    Session &operator=(Session &&other);

    void start();

    std::vector<std::string> make_vector(std::string s);

    std::string makeUser(std::string userName, std::string algo);

    std::string changeUser(std::string userName);

    std::string deleteUser(std::string userName);

    std::string duplicateUser(std::string originalUser, std::string newUser);

    void printContent();

    User *getActiveUser() const;

    void printLog();

    void watch(std::string id);

    Watchable *getContentById(long id);

    Watchable *getLengthRec(float average);

    std::vector<Watchable *> getContent() const;


private:
    std::vector<Watchable *> content;
    std::vector<BaseAction *> actionsLog;
    std::unordered_map<std::string, User *> userMap;
    User *activeUser;
    std::string cmd;

    void clear();

    void copy(const Session &copySession);
};

#endif