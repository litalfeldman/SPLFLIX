#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    std::string printMsg();
    virtual ~BaseAction() = default;
    virtual BaseAction* clone()=0;
protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    CreateUser(std::string inputname, const std::string &inputalgo);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual ~CreateUser() = default;
    virtual BaseAction* clone();
private:
    std::string name;
    std::string algorithm;
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(const std::string& inputname);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual ~ChangeActiveUser() = default;
    virtual BaseAction* clone();
private:
    std::string name;

};

class DeleteUser : public BaseAction {
public:
    DeleteUser(const std::string& inputname);
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual ~DeleteUser() = default;
    virtual BaseAction* clone();
private:
    std::string name;

};


class DuplicateUser : public BaseAction {
public:

    DuplicateUser(const std::string& originalName,const std::string& newName);
    virtual void act(Session & sess);
    virtual std::string toString() const;
    virtual ~DuplicateUser() = default;
    virtual BaseAction* clone();
private:
    std::string originalName;
    std::string newName;
};

class PrintContentList : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual ~PrintContentList() = default;
    virtual BaseAction* clone();
};

class PrintWatchHistory : public BaseAction {
public:
    virtual void act (Session& sess);
    virtual std::string toString() const;
    virtual ~PrintWatchHistory() = default;
    virtual BaseAction* clone();
};


class Watch : public BaseAction {
public:
    Watch(const std::string& inputId);
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual ~Watch() = default;
    virtual BaseAction* clone();
private:
    std::string id;
};


class PrintActionsLog : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual ~PrintActionsLog() = default;
    virtual BaseAction* clone();
};

class Exit : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual ~Exit() = default;
    virtual BaseAction* clone();
};
#endif