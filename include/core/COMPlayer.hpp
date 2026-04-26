#pragma once

#include <memory>
#include <string>

#include <core/COMStrategy.hpp>
#include <core/Player.hpp>

namespace core {

/**
 * @brief Automated player that consults a pluggable `COMStrategy`.
 */
class COMPlayer : public Player {
public:
    /**
     * @brief Construct a COM seat with optional AI ownership transfer.
     * @param name Display name used in logs.
     * @param token ASCII marker rendered on the board.
     * @param strategy Optional AI implementation; may be replaced later.
     */
    COMPlayer(std::string name, std::string token,
              std::unique_ptr<COMStrategy> strategy = nullptr);

    ~COMPlayer() override = default;

    void takeTurn(logic::Game& game) override;

    bool isHuman() override;

    /**
     * @brief Replace the active strategy object.
     * @param strategy Newly allocated strategy transferred into this player.
     */
    void setStrategy(std::unique_ptr<COMStrategy> strategy);

    /**
     * @brief Non-owning pointer for callers that only read strategy state.
     * @return Raw pointer to the owned strategy or `nullptr` if unset.
     * @note Extension (not in spec).
     */
    COMStrategy* getStrategy() const noexcept;

    /**
     * @brief Delegates interactive choices to the attached `COMStrategy`.
     * @note Extension (not in spec); overrides `Player::promptChoice`.
     */
    int promptChoice(const std::string& context, int defaultIndex,
                                     int optionCount) override;

private:
    std::unique_ptr<COMStrategy> strategy_;
};

}
