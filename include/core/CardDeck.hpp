#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <random>
#include <utility>
#include <vector>

namespace core {

/**
 * @brief Generic deck of cards
 *
 * Models a draw pile with a running `topIndex_`: `draw()` returns the next
 * card without removing it from storage. When the pile is exhausted, `draw()`
 * reshuffles the full pile (discard pile merged) per skill-deck rule 6 in the
 * Nimonspoli specification.
 *
 * This class template is header-only: member templates are defined below so
 * each translation unit that uses `CardDeck<T>` instantiates it at compile
 * time (no separate `.cpp` required for ordinary use).
 */
template <typename T>
class CardDeck {
public:
    CardDeck() = default;

    /**
     * @brief Take ownership of an initial ordered pile.
     * @param cards Cards transferred into this deck; `topIndex_` starts at 0.
     */
    explicit CardDeck(std::vector<std::unique_ptr<T>> cards);

    /**
     * @note Extension (not in spec): deleted copy operations for
     *       owning `std::unique_ptr` storage.
     */
    CardDeck(const CardDeck&) = delete;
    /**
     * @note Extension (not in spec).
     */
    CardDeck& operator=(const CardDeck&) = delete;
    /**
     * @note Extension (not in spec): defaulted move for transferring deck
     *       ownership between containers.
     */
    CardDeck(CardDeck&&) noexcept = default;
    /**
     * @note Extension (not in spec).
     */
    CardDeck& operator=(CardDeck&&) noexcept = default;

    /**
     * @brief Return the next drawable card and advance the read cursor.
     * @return Non-owning pointer to the card at `topIndex_` before advance, or
     *         `nullptr` when there are no cards at all.
     */
    T* draw();

    /**
     * @brief Randomly permute the underlying storage order.
     */
    void shuffle();

    /**
     * @brief Move the draw cursor back to the first card (undrawn state).
     */
    void reset() noexcept;

    /**
     * @brief Whether every card has been drawn at least once this cycle.
     * @return `true` when `topIndex_ >= cards_.size()`.
     */
    bool isEmpty() const noexcept;

    /**
     * @brief Total number of cards held in the deck storage.
     * @return `static_cast<int>(cards_.size())`.
     */
    int size() const noexcept;

    /**
     * @brief Add one card to storage deck (for GameLoader).
     * Card is being added at RIGHT BEFORE topIndex_ so it can't be automatically
     * drawed (card is being held by players, not in draw pile).
     * @note Extension. used by GameLoader::restorePlayers() to
     *       save player held cards' ownership inside the deck.
     */
    void addCard(std::unique_ptr<T> card);

private:
    /**
     * @brief Put all cards back into play and shuffle for a new cycle.
     * @note Extension (not in spec): implements reshuffling discard pile inside `draw()` for readability.
     */
    void reshuffleDiscard();

    std::vector<std::unique_ptr<T>> cards_;  // Extension (not in spec): spec lists `vector<T*>`; deck owns via `unique_ptr`.
    std::size_t topIndex_{0};
};

// ================================================================================
// ======================= CardDeck Template Implementation =======================
// ================================================================================

// Note: implementation is in the header file to avoid writing out CardDeck<SomeT>
//       in the .cpp file for every SomeT used in other files.

template <typename T>
CardDeck<T>::CardDeck(std::vector<std::unique_ptr<T>> cards)
    : cards_(std::move(cards)), topIndex_(0) {}

template <typename T>
T* CardDeck<T>::draw() {
    if (cards_.empty()) {
        return nullptr;
    }
    if (topIndex_ >= cards_.size()) {
        reshuffleDiscard();
    }
    return cards_[topIndex_++].get();
}

template <typename T>
void CardDeck<T>::shuffle() {
    if (cards_.empty()) {
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(cards_.begin(), cards_.end(), gen);
}

template <typename T>
void CardDeck<T>::reset() noexcept {
    topIndex_ = 0;
}

template <typename T>
bool CardDeck<T>::isEmpty() const noexcept {
    return topIndex_ >= cards_.size();
}

template <typename T>
int CardDeck<T>::size() const noexcept {
    return static_cast<int>(cards_.size());
}

template <typename T>
void CardDeck<T>::reshuffleDiscard() {
    reset();
    shuffle();
}

template <typename T>
void CardDeck<T>::addCard(std::unique_ptr<T> card) {
    if (!card) return;
    cards_.insert(cards_.begin() + static_cast<std::ptrdiff_t>(topIndex_),
                  std::move(card));
    ++topIndex_;
}

}
