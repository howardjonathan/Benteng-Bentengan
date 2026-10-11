#include <iostream>
#include <string>
#include <random>
#include <limits>
#include <vector>
#include <algorithm>
#include <array>
#include <variant>
#include <unordered_map>
#include <map>



void benteng_game() {

    struct Deck {
        std::string card_name;
        int value = 0;
        std::string ability = "";
        bool revealed = false;
        bool hasJack = false;
    };

    enum class Suits {
        Jack,
        Queen,
        King
    };

    enum class Turn {
        You,
        Bot_1
    };

    struct Player {
        std::string name;
        int life_value = 5;
        std::vector<Deck> life_deck;
        std::vector<Deck> hand;
    };


    std::vector<Deck> cards = {
        {"Ace(S)", 1}, {"2(S)", 2}, {"3(S)", 3}, {"4(S)", 4}, {"5(S)", 5}, {"6(S)", 6}, {"7(S)", 7}, {"8(S)", 8}, {"9(S)", 9},{"10(S)", 10}, {"Jack(S)", 0, "Defend"},
        {"Queen(S)", 0, "Swap"}, {"King(S)", 0, "Kill"}, {"Ace(C)", 1}, {"2(C)", 2}, {"3(C)", 3}, {"4(C)", 4}, {"5(C)", 5}, {"6(C)", 6}, {"7(C)", 7}, {"8(C)", 8},
        {"9(C)", 9}, {"10(C)", 10}, {"Jack(C)", 0, "Defend"}, {"Queen(C)", 0, "Swap"}, {"King(C)", 0, "Kill"}, {"Ace(H)", 1}, {"2(H)", 2}, {"3(H)", 3}, {"4(H)", 4}, {"5(H)", 5}, {"6(H)", 6},
        {"7(H)", 7}, {"8(H)", 8}, {"9(H)", 9}, {"10(H)", 10}, {"Jack(H)", 0, "Defend"}, {"Queen(H)", 0, "Swap"}, {"King(H)", 0, "Kill"}, {"Ace(D)", 1}, {"2(D)", 2}, {"3(D)", 3},
        {"4(D)", 4}, {"5(D)", 5}, {"6(D)", 6}, {"7(D)", 7}, {"8(D)", 8}, {"9(D)", 9}, {"10(D)", 10}, {"Jack(D)", 0, "Defend"}, {"Queen(D)", 0, "Swap"}, {"King(D)", 0, "Kill"}
    };

    std::vector<Deck> life_cards = {
        {"Ace(S)", 1}, {"2(S)", 2}, {"3(S)", 3}, {"4(S)", 4}, {"5(S)", 5}, {"6(S)", 6}, {"7(S)", 7}, {"8(S)", 8}, {"9(S)", 9}, {"10(S)", 10},
        {"Ace(C)", 1}, {"2(C)", 2}, {"3(C)", 3}, {"4(C)", 4}, {"5(C)", 5}, {"6(C)", 6}, {"7(C)", 7}, {"8(C)", 8}, {"9(C)", 9}, {"10(C)", 10},
        {"Ace(H)", 1}, {"2(H)", 2}, {"3(H)", 3}, {"4(H)", 4}, {"5(H)", 5}, {"6(H)", 6}, {"7(H)", 7}, {"8(H)", 8}, {"9(H)", 9}, {"10(H)", 10},
        {"Ace(D)", 1}, {"2(D)", 2}, {"3(D)", 3}, {"4(D)", 4}, {"5(D)", 5}, {"6(D)", 6}, {"7(D)", 7}, {"8(D)", 8}, {"9(D)", 9}, {"10(D)", 10}
    };

    std::random_device randcard;
    std::mt19937 card_gen(randcard());
    std::shuffle(cards.begin(), cards.end(), card_gen);
    std::shuffle(life_cards.begin(), life_cards.end(), card_gen);
    std::unordered_map<std::string, Player> players;

    players["Bot_1"] = {{"Bot_1"}, {5}, {}, {}};
    players["You"] = {{"You"}, {5}, {}, {}};

    int life_count = 0;
    while (life_count < 5) {
        players["Bot_1"].life_deck.push_back(life_cards.back());
        life_cards.pop_back();

        players["You"].life_deck.push_back(life_cards.back());
        life_cards.pop_back();

        life_count++;
    }

    auto in_life_deck = [&](const Deck& card) {
        bool in_bot_life = std::any_of(
            players["Bot_1"].life_deck.begin(),
            players["Bot_1"].life_deck.end(),
            [&card](const Deck& lifecard) {
            return lifecard.card_name == card.card_name;
        } );

        bool in_your_life = std::any_of(
            players["You"].life_deck.begin(),
            players["You"].life_deck.end(),
            [&card](const Deck& lifecard) {
            return lifecard.card_name == card.card_name;
        } );

        return in_bot_life || in_your_life;
    };

    auto deal_cards = [&](Player& player) {
        while (player.hand.size() < 5 && !cards.empty()) {
            Deck next_card = cards.back();
            cards.pop_back();

            if (!in_life_deck(next_card)) {
                player.hand.push_back(next_card);
            }
        }
    };

    deal_cards(players["Bot_1"]);
    deal_cards(players["You"]);

    std::cout << "Your life cards are:\n";
    for (const Deck& life : players["You"].life_deck) {
        std::cout << life.card_name << std::endl;
    }
    std::cout << "\n";
    std::cout << "Your hand cards are:\n";
    for (const Deck& deck : players["You"].hand) {
        std::cout << deck.card_name << std::endl;
    }
    std::cout << "\n";

    Turn turns = Turn::You;
    while (turns == Turn::You) {

        std::string chosen_card;
        std::cout << "Pick your card\n";
        std::cin >> chosen_card;
        Deck* played_card = nullptr;

        auto check_selfplay = find_if(players["You"].hand.begin(),
        players["You"].hand.end(),
        [&chosen_card](const Deck& card) {
            return card.card_name == chosen_card;
        });

        if (check_selfplay == players["You"].hand.end()) {
            std::cout << "This is not a valid card\n";
        } else {
            played_card = &(*check_selfplay);
            std::cout << "You have played " << played_card->card_name << std::endl;
            std::cout << "Choose a card to attack\n";
        }

        std::string attack_bot;
        std::cin >> attack_bot;
        Deck* attacked_card = nullptr;

        auto check_botdeck = find_if(players["Bot_1"].life_deck.begin(),
        players["Bot_1"].life_deck.end(),
        [&attack_bot](const Deck& card){
            return card.card_name == attack_bot;
        });

        if (check_botdeck == players["Bot_1"].life_deck.end()) {
            std::cout << "This card is invalid\n";
        } else {
            attacked_card = &(*check_botdeck);
            std::cout << "You have attacked " << attacked_card->card_name << std::endl;
        }

        if (played_card != nullptr && attacked_card != nullptr) {
            if (played_card->value >= 1 && played_card->value <=10) {
                    if(attacked_card->hasJack = false) {
                        int add_result = played_card->value + attacked_card->value;

                        if (add_result % 10 == 0) {
                            players["Bot_1"].life_deck.erase(check_botdeck);
                            players["You"].hand.erase(check_selfplay);
                        } else {
                            attacked_card->value = add_result;
                            attacked_card->revealed = true;
                            players["You"].hand.erase(check_selfplay);
                        }
                    } else {
                        attacked_card->hasJack = false;
                        players["You"].hand.erase(check_selfplay);
                    }
            }
        }
        if (!cards.empty()) {
            players["You"].hand.push_back(cards.back());
            cards.pop_back();
        }
        turns = Turn::Bot_1;
    }
}



int main() {
    benteng_game();
    return 0;
}


