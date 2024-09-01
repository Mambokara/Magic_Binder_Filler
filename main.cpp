#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Cards {
    public:
        vector<vector<string>> cards_info;
    private:
        vector<string> card_infos;
        vector<string> Card_text;

    public:
        Cards(string link) {
            Card_text = parse_file(link);
            for (int i = 0; i < Card_text.size(); i++) {
                card_infos = separate_infos(Card_text[i]);
                cards_info.push_back(card_infos);
            }
        }
    private:
        vector<string> parse_file(string link) {
            string tmp_infos;
            fstream file(link);
            vector<string> Card;

            getline(file, tmp_infos, '\r');
            while (getline(file, tmp_infos, '\r')) {
                Card.push_back(tmp_infos);
            }
            file.close();
            return Card;
        }

        vector<string> separate_infos(string info){
            vector<string> tmp;
            string tmp_string;

             for (int i = 0; i < info.size(); i++) {
                if (info[i] == ',' and info[i + 1] == '"') {
                    tmp.push_back(tmp_string);
                    tmp_string.clear();
                    continue;
                }
                tmp_string.push_back(info[i]);
            }
            tmp.push_back(tmp_string);
            return tmp;
        }
};

vector<vector<vector<string>>> compare_infos(vector<vector<string>> box, vector<vector<string>> binder) {
    vector<vector<vector<string>>> storage;
    int found = 0;
    string str;
    int tmp = 0;
    for (int i = 0; i < box.size() - 1; i++) {
        for (int j = 0; j < binder.size(); j++) {
            if (box[i][2].compare(binder[j][2]) == 0) {
                found = 1;
                break;
            }
        }
        if (found != 1) {
            binder.push_back(box[i]);
            binder.back()[0][2] = '1';
            binder.back()[1][1] = '1';
            str = box[i][0].substr(2, box[i][0].size() - 3);
            tmp = stoi(str);
            tmp--;
            if (tmp <= 0) {
                box.erase(box.begin() + i);
                i--;
            } else {
                box[i][0] = "\n\"" + to_string(tmp) + "\"";
                box[i][1] = "\"" + to_string(tmp) + "\"";
            }
        } else {
            found = 0;
        }
    }
    storage.push_back(binder);
    storage.push_back(box);
    return storage;
}

void print_in_file(vector<vector<string>> new_csv, string file_name) {
    ofstream output_file(file_name);

    output_file << "\"Count\",\"Tradelist Count\",\"Name\",\"Edition\",\"Condition\",\"Language\",\"Foil\",\"Tags\",\"Last Modified\",\"Collector Number\",\"Alter\",\"Proxy\",\"Purchase Price\"\r";
    for (int i = 0; i < new_csv.size(); i++) {
        for (int j = 0; j < new_csv[i].size(); j++) {
            output_file << new_csv[i][j];
            if (j != new_csv[i].size() - 1) {
                output_file << ",";
            }
        }
        output_file << "\r";
    }
    output_file.close();
}

int main() {
    Cards Box_csv("moxfield_haves_2024-08-29-1530Z.csv");
    Cards Binder_csv("empty.csv");
    vector<vector<vector<string>>> new_csv = compare_infos(Box_csv.cards_info, Binder_csv.cards_info);
    print_in_file(new_csv[0], "Binder.csv");
    print_in_file(new_csv[1], "test.csv");

}
