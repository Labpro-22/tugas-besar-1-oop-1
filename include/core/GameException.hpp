#pragma once
#include <exception>
#include <string>
namespace core {
class Player;
}

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
  InsufficientFundsException(int amount, int required)
      : amount_(amount), required_(required){};

  std::string getMessage() const override {
    return "InsufficientFundsException: Dimiliki M" + std::to_string(amount_) +
           ", Dibutuhkan M" + std::to_string(required_) + ", Kekurangan M" +
           std::to_string(required_ - amount_);
  }

  int getAmount() const { return amount_; }
  int getRequired() const { return required_; }

 private:
  int amount_;
  int required_;
};

class InvalidMoveException : public GameException {
 public:
  InvalidMoveException(const std::string& reason) : reason_(reason){};

  std::string getMessage() const override {
    return "InvalidMoveException: " + reason_;
  }

  std::string getReason() const { return reason_; }

 private:
  std::string reason_;
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

  std::string getConfigKey() const { return configKey_; }
  std::string getExpected() const { return expected_; }

 private:
  std::string configKey_;
  std::string expected_;
};

class FileIOException : public GameException {
 public:
  FileIOException(const std::string& filename, const std::string& operation)
      : filename_(filename), operation_(operation) {}

  std::string getMessage() const override {
    return "FileIOException: tidak bisa " + operation_ + " file '" + filename_ +
           "'";
  }

  std::string getFilename() const { return filename_; }
  std::string getOperation() const { return operation_; }

 private:
  std::string filename_;
  std::string operation_;
};

class InvalidStateException : public GameException {
 public:
  InvalidStateException(const std::string& action, const std::string& currState)
      : action_(action), currState_(currState) {}
  std::string getMessage() const override {
    return "InvalidStateException: tidak bisa melakukan '" + action_ +
           "' saat game sedang dalam state '" + currState_ + "'";
  }
  std::string getAction() const { return action_; }
  std::string getCurrState() const { return currState_; }

 private:
  std::string action_;
  std::string currState_;
};

class UnauthorizedActionException : public GameException {
 public:
  UnauthorizedActionException(const std::string& actorName,
                              const std::string& ownerName,
                              const std::string& propertyName)
      : actorName_(actorName),
        ownerName_(ownerName),
        propertyName_(propertyName) {}

  std::string getMessage() const override {
    return "UnauthorizedActionException: '" + actorName_ +
           "' tidak bisa build di '" + propertyName_ +
           "' karena property milik '" + ownerName_ + "'";
  }

  std::string getActorName() const { return actorName_; }
  std::string getOwnerName() const { return ownerName_; }
  std::string getPropertyName() const { return propertyName_; }

 private:
  std::string actorName_;
  std::string ownerName_;
  std::string propertyName_;
};

class InvalidPropertyTypeException : public GameException {
 public:
  InvalidPropertyTypeException(const std::string& propertyName,
                               const std::string& propertyType)
      : propertyName_(propertyName), propertyType_(propertyType) {}

  std::string getMessage() const override {
    return "InvalidPropertyTypeException: '" + propertyName_ +
           "' adalah tipe '" + propertyType_ +
           "' yang tidak mendukung pembangunan rumah/hotel";
  }

  std::string getPropertyName() const { return propertyName_; }
  std::string getPropertyType() const { return propertyType_; }

 private:
  std::string propertyName_;
  std::string propertyType_;
};

class GameSetupException : public GameException {
 public:
  explicit GameSetupException(const std::string& reason)
      : reason_(reason), configKey_(""), configValue_("") {}

  GameSetupException(const std::string& reason, const std::string& configKey,
                     const std::string& configValue)
      : reason_(reason), configKey_(configKey), configValue_(configValue) {}

  std::string getMessage() const override {
    std::string msg = "GameSetupException: " + reason_;
    if (!configKey_.empty()) {
      msg += " (key='" + configKey_ + "', value='" + configValue_ + "')";
    }
    return msg;
  }

  std::string getReason() const { return reason_; }
  std::string getConfigKey() const { return configKey_; }
  std::string getConfigValue() const { return configValue_; }

 private:
  std::string reason_;
  std::string configKey_;
  std::string configValue_;
};
