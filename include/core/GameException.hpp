#pragma once
#include <string>
#include <exception>

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

class InvalidConfigException : public GameException {
public:
    InvalidConfigException(const std::string& configKey,
                           const std::string& expected)
        : configKey_(configKey), expected_(expected) {}

    std::string getMessage() const override {
        return "InvalidConfigException: key='" + configKey_ +
               "' expected=" + expected_;
    }

private:
    std::string configKey_;
    std::string expected_;
};

class FileIOException : public GameException {
public:
    FileIOException(const std::string& filename,
                    const std::string& operation)
        : filename_(filename), operation_(operation) {}

    std::string getMessage() const override {
        return "FileIOException: cannot " + operation_ +
               " file '" + filename_ + "'";
    }

private:
    std::string filename_;
    std::string operation_;
};
