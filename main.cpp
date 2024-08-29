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
            //cout << link << endl;

            Card_text = parse_file(link);
            for (int i = 0; i < Card_text.size(); i++) {
                card_infos = separate_infos(Card_text[i]);
                cards_info.push_back(card_infos);
            }
    //        for (int j = 0; j < cards_info.size(); j++) {
    //            if (!cards_info[j].empty()) {
    //                cout << cards_info[j][2];
    //                cout << endl;
    //            }
    //        }
        cout << cards_info.size() << endl << endl << endl;
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
            return tmp;
        }
};

vector<vector<string>> compare_infos(vector<vector<string>> box, vector<vector<string>> binder) {
    int found = 0;
    for (int i = 0; i < box.size() - 1; i++) {
        for (int j = 0; j < binder.size(); j++) {
            if (box[i][2] == binder[j][2]) {
                found = 1;
                break;
            }
        }
        if (found != 1) {
            box[i][0][2] = '1';
            box[i][1][1] = '1';
            binder.push_back(box[i]);
        } else {
            found = 0;
        }
    }
    return binder;
}

int main() {
    Cards Box_csv("moxfield_haves_2024-08-29-1530Z.csv");
    Cards Binder_csv("empty.csv");
    vector<vector<string>> new_csv = compare_infos(Box_csv.cards_info, Binder_csv.cards_info);
    ofstream output_file("Binder.csv");
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

}
