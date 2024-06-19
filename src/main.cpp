#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <array>

using namespace std;

struct Upper_section {
    int aces;
    int twos;
    int threes;
    int fours;
    int fives;
    int sixes;
};
struct Lower_section {
    int threekind;
    int fourkind;
    int fhouse;
    int sm;
    int lg;
    int yahtzee;
    int chances;
};

class GameBoard {
    public:
        Upper_section upper_section;
        Lower_section lower_section;

        GameBoard() {};

        void reset_board() {
            upper_section.aces = -1;
            upper_section.twos = -1;
            upper_section.threes = -1;
            upper_section.fours = -1;
            upper_section.fives = -1;
            upper_section.sixes = -1;
            lower_section.threekind = -1; // all dices
            lower_section.fourkind = -1; // all dices
            lower_section.fhouse = -1;
            lower_section.sm = -1;
            lower_section.lg = -1;
            lower_section.yahtzee = -1;
            lower_section.chances = -1;
        }

        void display() {
            int upper_total = calc_upper_section();
            int lower_total = calc_lower_section();
            int bonus = (upper_total >= 63) ? 35 : 0;
            int grand_total = upper_total + bonus + lower_total;

            cout << "+----------------------++---------------------------+\n";
            cout << "|    UPPER SECTION     |       LOWER SECTION        |\n";
            cout << "+----------------------+----------------------------+\n";
            cout << "| Aces   [1s] |" << center_data(upper_section.aces) << "| 3 of a kind  [3k] |" << center_data(lower_section.threekind) << "|\n";
            cout << "|----------------------|----------------------------|\n";
            cout << "| Twos   [2s] |" << center_data(upper_section.twos) << "| 4 of a kind  [4k] |" << center_data(lower_section.fourkind) << "|\n";
            cout << "|----------------------|----------------------------|\n";
            cout << "| Threes [3s] |" << center_data(upper_section.threes) << "| Full House   [fh] |" << center_data(lower_section.fhouse) << "|\n";
            cout << "|----------------------|----------------------------|\n";
            cout << "| Fours  [4s] |" << center_data(upper_section.fours) << "| Sm. Straight [ss] |" << center_data(lower_section.sm) << "|\n";
            cout << "|----------------------|----------------------------|\n";
            cout << "| Fives  [5s] |" << center_data(upper_section.fives) << "| Lg. Straight [ls] |" << center_data(lower_section.lg) << "|\n";
            cout << "|----------------------|----------------------------|\n";
            cout << "| Sixes  [6s] |" << center_data(upper_section.sixes) << "| YAHTZEE      [yz] |" << center_data(lower_section.yahtzee) << "|\n";
            cout << "|----------------------|----------------------------|\n";
            cout << "| BONUS (63+) |" << center_data(bonus) << "| Chances      [c]  |" << center_data(lower_section.chances) << "|\n";
            cout << "|----------------------|----------------------------|\n";
            cout << "| Total Score |" << center_data(upper_total+bonus) << "| Total Score       |" << center_data(lower_total) << "|\n";
            cout << "+----------------------+----------------------------+\n";
            cout << "                       | Grand Total       |" << center_data(grand_total) << "|\n";
            cout << "                       +-------------------+--------+\n";
        }

        int count_num_dice(int num, vector<int> dices) {
            int total_num = 0;
            for (int& dice : dices) {
                if (num == dice) {
                    total_num = total_num + dice;
                }
            }
            return total_num;
        }

        int add_all_dices(vector<int> dices) {
            int total_num = 0;
            for (int &dice : dices) {
                total_num = total_num + dice;
            }

            return total_num;
        }

        bool check_kind(int how_often, vector<int> dices) {
            int largest_seen = 0;
            sort(dices.begin(), dices.end());
            
            int previous = dices[0];
            int seen_count = 1;
            
            for (size_t i = 1; i < dices.size(); ++i) {
                if (dices[i] == previous) {
                    seen_count = seen_count + 1;
                } else {
                    largest_seen = max(seen_count, largest_seen);
                    seen_count = 1;
                    previous = dices[i];
                }
            }
            largest_seen = max(seen_count, largest_seen);
            
            return largest_seen >= how_often;
        }

        
        void aces(vector<int> dices) {
            upper_section.aces = count_num_dice(1, dices);
        }

        void twos(vector<int> dices) {
            upper_section.twos = count_num_dice(2, dices);
        }

        void threes(vector<int> dices) {
            upper_section.threes = count_num_dice(3, dices);
        }

        void fours(vector<int> dices) {
            upper_section.fours = count_num_dice(4, dices);
        }

        void fives(vector<int> dices) {
            upper_section.fives = count_num_dice(5, dices);
        }

        void sixes(vector<int> dices) {
            upper_section.sixes = count_num_dice(6, dices);
        }

        void three_of_kind(vector<int> dices) {
            lower_section.threekind = (check_kind(3, dices)) ? add_all_dices(dices) : 0;
        }

        void four_of_kind(vector<int> dices) {
            lower_section.fourkind = (check_kind(4, dices)) ? add_all_dices(dices) : 0;
        }

        int get_longest_straight(vector<int> &dices) { // logic issue here
            int longest = 0;
            sort(dices.begin(), dices.end());

            int maxStreak = 1;
            int currentStreak = 1;

            
            for (size_t i = 1; i < dices.size(); ++i) {
                if (dices[i] == dices[i - 1] + 1) {
                    currentStreak++;
                } else if (dices[i] != dices[i - 1]) {
                    maxStreak = max(maxStreak, currentStreak);
                    currentStreak = 1;
                }
            }

            maxStreak = max(maxStreak, currentStreak);

            return maxStreak;
        }

        void fhouse(vector<int> dices) {
            array<int, 6> counts = {0};

            for (int num : dices) {
                counts[num - 1]++;
            }

            bool foundThree = false;
            bool foundTwo = false;

            for (int count : counts) {
                if (count == 3) {
                    foundThree = true;
                } else if (count == 2) {
                    foundTwo = true;
                }
            }

            lower_section.fhouse = (foundTwo && foundThree) ? 25 : 0;
        }

        void sm_straight(vector<int> dices) {
            int straight_count = get_longest_straight(dices);
            lower_section.sm = (straight_count >= 3) ? 30 : 0;
        }

        void lg_straight(vector<int> dices) {
            int straight_count = get_longest_straight(dices);
            lower_section.lg = (straight_count >= 4) ? 40 : 0;
        }

        void chance(vector<int> dices) {
            lower_section.chances = add_all_dices(dices);
        }

        void yahtzee(vector<int> dices) {
            int dice_count = 0;

            int previous_dice;

            for (int &dice : dices) {
                if (previous_dice == dice) {
                    dice_count = dice_count + 1;
                    continue;
                }
                previous_dice = dice;
                dice_count = 1;
            }
            
            lower_section.yahtzee = (dice_count == 5) ? 50 : 0;
        }

    private:
        int calc_upper_section() {
            array<int, 6> items = {upper_section.aces, upper_section.twos,
                                    upper_section.threes, upper_section.fours,
                                    upper_section.fives, upper_section.sixes};
            int total = 0;
            for (int i = 0; i < 6; i++) {
                if (items[i] >= 0) { 
                    total = total + items[i];
                }
            }

            return total;
        }

        int calc_lower_section() {
            array<int, 7> items = {lower_section.threekind, lower_section.fourkind,
                                    lower_section.fhouse, lower_section.sm,
                                    lower_section.lg, lower_section.yahtzee,
                                    lower_section.chances};
            
            int total = 0;
            for (int i = 0; i < 7; i++) {
                if (items[i] >= 0) { 
                    total = total + items[i];
                }
            }

            return total;
        }

        string center_data(int value, int spaces = 8) {
            int value_length = to_string(value).length();
            int num_whitespace = spaces - value_length;
            string data = "";
            
            for (int i = 0; i <= num_whitespace; i++) {
                if (i == (num_whitespace/2)) {
                    data = data + to_string(value);
                } else {
                    data = data + " ";
                }
            }

            return data;
        }
};

int roll_dice(int lower_bound = 1, int upper_bound = 6) {
    return (rand() % upper_bound) + lower_bound;
}

void display_dice(vector<int> &dices) {
    cout << "Your Dices: " << "[ " << dices[0] << ", " << dices[1] << ", " << dices[2] << ", " << dices[3] << ", " << dices[4] << " ]" << endl;
}

vector<int> parse_input(string input) {
    input = input + ", ";
    vector<int> need_rolling_dices;
    string delimiter = ", ";

    size_t pos = 0;
    int token;
    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = stoi(input.substr(0, pos));
        need_rolling_dices.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    return need_rolling_dices;
}

vector<int> handle_dice_event (GameBoard &gameboard) {
    int rolls_left = 2;
    srand(time(0));
    vector<int> dices = {roll_dice(), roll_dice(), roll_dice(), roll_dice(), roll_dice()};
    string event_options;
    cin.ignore();

    for (rolls_left; 0 < rolls_left; rolls_left--) {
        system("cls");
        gameboard.display();
        display_dice(dices);

        cout << "Re-roll dices? Roll left: " << rolls_left << "\n1. Yes. which dices? eg '1 3'. reroll first and third\n0. Submit dices" << endl;
        getline(cin, event_options);

        if (event_options == "0") { break; }

        cout << event_options << endl;
        vector<int> dices_to_roll = parse_input(event_options);
        cin.clear();

        for (int &dice_index : dices_to_roll) {
            dices[dice_index-1] = roll_dice();
        }
    }

    return dices;
}

void handle_score(vector<int> &dices, GameBoard &gameboard) {
    system("cls");
    display_dice(dices);
    gameboard.display();

    string score_options;

    while (true) {
        cout << "Where would you like to score? : ";
        cin >> score_options;

        if (score_options == "1s" && gameboard.upper_section.aces < 0) {
            gameboard.aces(dices);
        } else if (score_options == "2s" && gameboard.upper_section.twos < 0) {
            gameboard.twos(dices);
        } else if (score_options == "3s" && gameboard.upper_section.threes < 0) {
            gameboard.threes(dices);
        } else if (score_options == "4s" && gameboard.upper_section.fours < 0) {
            gameboard.fours(dices);
        } else if (score_options == "5s" && gameboard.upper_section.fives < 0) {
            gameboard.fives(dices);
        } else if (score_options == "6s" && gameboard.upper_section.sixes < 0) {
            gameboard.sixes(dices);
        } else if (score_options == "3k" && gameboard.lower_section.threekind < 0) {
            gameboard.three_of_kind(dices);
        } else if (score_options == "4k" && gameboard.lower_section.fourkind < 0) {
            gameboard.four_of_kind(dices);
        } else if (score_options == "fh" && gameboard.lower_section.fhouse < 0) {
            gameboard.fhouse(dices);
        } else if (score_options == "ss" && gameboard.lower_section.sm < 0) {
            gameboard.sm_straight(dices);
        } else if (score_options == "ls" && gameboard.lower_section.lg < 0) {
            gameboard.lg_straight(dices);
        } else if (score_options == "yz" && gameboard.lower_section.yahtzee < 0) {
            gameboard.yahtzee(dices);
        } else if (score_options == "c" && gameboard.lower_section.chances < 0) {
            gameboard.chance(dices);
        } else {
            cout << "Invaild choices or already scored\n";
            continue;
        }
        break;
    }

}
 
int main() {
    int event_option;
    GameBoard gameboard;
    gameboard.reset_board();

    do {
        system("cls");
        gameboard.display();
        cout << "Choose one.\n1. Roll Dice\n0. Exit\n";
        cin >> event_option;
        
        if (event_option == 1) {
            vector<int> dices = handle_dice_event(gameboard);
            handle_score(dices, gameboard);
        } else {
            event_option = 0;
        }
    } while (event_option != 0 );
    return 0;
}