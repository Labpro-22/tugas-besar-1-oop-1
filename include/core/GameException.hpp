#pragma once
#include <string>
#include <exception>
namespace core { class Player; }

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

class InsufficientFundsException : public GameException {
public:
    InsufficientFundsException(int amount, int required) : amount_(amount), required_(required) {};

    string getMessage() const override {
        return "InsufficientFundsException: Dimiliki M" + to_string(amount_) + ", Dibutuhkan M" + to_string(required_) + ", Kekurangan M" + to_string(required_ - amount_);
    }

    int getAmount() const { 
        return amount_; 
    }
    int getRequired() const { 
        return required_; 
    }

private:
    int amount_;
    int required_;
};

class InvalidMoveException : public GameException {
public:
    InvalidMoveException(const string& reason) : reason_(reason) {};

    string getMessage() const override {
        return "InvalidMoveException: " + reason_;
    }

    string getReason() const { 
        return reason_; 
    }

private:
    string reason_;
};

class InvalidConfigException : public GameException {
public:
    InvalidConfigException(const string& configKey, const string& expected) : configKey_(configKey), expected_(expected) {}

    string getMessage() const override {
        return "InvalidConfigException: key='" + configKey_ + "' expected=" + expected_;
    }

    string getConfigKey() const { 
        return configKey_; 
    }
    string getExpected()  const { 
        return expected_; 
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

    string getFilename() const { 
        return filename_; 
    }
    string getOperation() const { 
        return operation_; 
    }

private:
    string filename_;
    string operation_;
};