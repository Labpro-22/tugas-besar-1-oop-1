#pragma once
#include <string>
#include <exception>
using namespace std;

class GameException : public std::exception {
public:
    virtual string getMessage() const = 0;

    const char* what() const noexcept override {
        cachedMessage_ = getMessage();
        return cachedMessage_.c_str();
    }

    virtual ~GameException() = default;

private:
    mutable string cachedMessage_;
};
 
class InvalidMoveException : public GameException {
public: 
    InvalidMoveException(const string& reason) : reason_(reason) {};
    string getMessage() const override {
        return "InvalidMoveException: " + reason_;
    }

private:
    string reason_;
};

class InvalidConfigException : public GameException {
public:
    InvalidConfigException(const string& configKey, const string& expected)
        : configKey_(configKey), expected_(expected) {}

    string getMessage() const override {
        return "InvalidConfigException: key='" + configKey_ + "' expected=" + expected_;
    }

private:
    string configKey_;
    string expected_;
};

class FileIOException : public GameException {
public:
    FileIOException(const string& filename, const string& operation) : filename_(filename), operation_(operation) {}

    string getMessage() const override {
        return "FileIOException: cannot " + operation_ + " file '" + filename_ + "'";
    }

private:
    string filename_;
    string operation_;
};
