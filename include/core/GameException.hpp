#pragma once
#include <string>
#include <exception>
namespace core { class Player; }


class GameException : public std::exception {
public:
    virtual std::string getMessage() const = 0;

    const char* what() const noexcept override {
        cachedMessage_ = getMessage();
        return cachedMessage_.c_str();
    }

    virtual ~GameException() = default;

private:
    mutable std::string cachedMessage_;
};

class InsufficientFundsException : public GameException {
public:
    InsufficientFundsException(int amount, int required) : amount_(amount), required_(required) {};

    std::string getMessage() const override {
        return "InsufficientFundsException: Dimiliki M" + std::to_string(amount_) + ", Dibutuhkan M" + std::to_string(required_) + ", Kekurangan M" + std::to_string(required_ - amount_);
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
    InvalidMoveException(const std::string& reason) : reason_(reason) {};

    string getMessage() const override {
        return "InvalidMoveException: " + reason_;
    }

    string getReason() const { 
        return reason_; 
    }

private:
    std::string reason_;
};

class InvalidConfigException : public GameException {
public:
    InvalidConfigException(const std::string& configKey, const std::string& expected) : configKey_(configKey), expected_(expected) {}

    std::string getMessage() const override {
        return "InvalidConfigException: key='" + configKey_ + "' expected=" + expected_;
    }

    std::string getConfigKey() const { 
        return configKey_; 
    }
    std::string getExpected()  const { 
        return expected_; 
    }

private:
    std::string configKey_;
    std::string expected_;
};

class FileIOException : public GameException {
public:
    FileIOException(const std::string& filename, const std::string& operation) : filename_(filename), operation_(operation) {}

    std::string getMessage() const override {
        return "FileIOException: cannot " + operation_ + " file '" + filename_ + "'";
    }

    std::string getFilename() const { 
        return filename_; 
    }
    std::string getOperation() const { 
        return operation_; 
    }

private:
    std::string filename_;
    std::string operation_;
};