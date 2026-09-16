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
}



int main() {
    benteng_game();
    return 0;
}


