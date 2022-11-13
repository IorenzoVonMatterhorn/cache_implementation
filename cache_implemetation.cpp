#include <bits/stdc++.h>
#include <cmath>
using namespace std;
typedef long long ll;
typedef vector<long long> vll;

string HexToBin(string hexdec) {
    long int i = 1;
    string s = "";
    while (hexdec[i]) {

        switch (hexdec[i])
        {
        case '0':
            s += "0000";
            break;
        case '1':
            s += "0001";
            break;
        case '2':
            s += "0010";
            break;
        case '3':
            s += "0011";
            break;
        case '4':
            s += "0100";
            break;
        case '5':
            s += "0101";
            break;
        case '6':
            s += "0110";
            break;
        case '7':
            s += "0111";
            break;
        case '8':
            s += "1000";
            break;
        case '9':
            s += "1001";
            break;
        case 'A':
        case 'a':
            s += "1010";
            break;
        case 'B':
        case 'b':
            s += "1011";
            break;
        case 'C':
        case 'c':
            s += "1100";
            break;
        case 'D':
        case 'd':
            s += "1101";
            break;
        case 'E':
        case 'e':
            s += "1110";
            break;
        case 'F':
        case 'f':
            s += "1111";
            break;
        default:
            cout << "\nInvalid hexadecimal digit "
                 << hexdec[i] << endl;
        }
        i++;
    }
    return s;
}


int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);


#ifndef ONLINE_JUDGE
    freopen("error.txt", "w", stderr);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    // cout << "HI\n";

    long long cache_size, ways, block_size, total_indices, sets, size_of_memory;
    cin >> cache_size >> block_size >> ways;             // no of sets= cache size / block size * ways

    sets = (cache_size * 1024) / (block_size * ways);    // cache size to be given in kb and sets=index basically
    total_indices = sets * ways;
    cout << cache_size << " " << block_size << " " << ways << " " << sets << " " << total_indices << '\n';
    size_of_memory = 1 << 10;

    // map<long long, long long> lastUsedIndex;
    ll lastUsedIndex[total_indices];
    // vector<vll> memory(size_of_memory);
    ll memory[size_of_memory][block_size];
    // vector<vll> cache(total_indices);
    ll cache[total_indices][block_size + 2];
    for (int i = 0; i < (size_of_memory) ; i++) {
        long long z = block_size;
        while (z--) {
            memory[i][z] = 0;
            // memory[i].push_back(rand() % (100)); // 2^28
        }
    }

    for (int i = 0; i < total_indices; i++) {
        long long z = block_size + 2;
        lastUsedIndex[i] = 0;
        while (z--) {
            cache[i][z] = 0;
            // cache[i].push_back(0);
        }
    }

    // for (int i = 0; i < (1 << 9); i++) {
    //     long long z = block_size ;
    //     for (int j = 0; j < z; j++) {
    //         cout << memory[i][j] << " ";
    //     }
    //     cout << '\n';
    // }

    // cout << "cache:" << endl;

    // for (int i = 0; i < total_indices; i++) {
    //     for (ll j = 0; j < block_size + 2; j++) {
    //         cout << cache[i][j] << " ";
    //     }
    //     cout << '\n';
    // }

    ll ind = 0, n_hit = 0, n_miss = 0;
    bool was_hit = 0, was_miss = 1;

    while (true) {
        ind++;
        was_hit = 0;
        was_miss = 1;
        // ll return_value;

        string instruction;
        // ll data;
        cin >> instruction;
        if (instruction == "HLT") {
            break;
        }
        // cout << read_or_write << endl;

        // if (read_or_write == "HLT") {
        //     break;
        // }
        // if (read_or_write == "READ") {
        //     cin >> addr;
        // }
        // else {
        //     cin >> addr;
        //     cin >> data;
        //     cout << "data:" << data << endl;
        // }

        string bin_address = HexToBin(instruction);

        bitset<32> address(bin_address); // bitsets for easier access
        // cout << address << '\n';
        ll offset = ((1 << (int)log2(block_size)) - 1) & address.to_ulong();
        ll set_no = ((1 << (int)log2(sets)) - 1) & (address.to_ulong() >> ((int)log2(block_size)));
        ll tag = (address.to_ulong() >> ((int)log2(sets) + (int)log2(block_size)));

        cout << tag << " " << set_no << " " << offset << endl;

        for (int i = set_no * ways; i < set_no * ways + ways; i++) {

            if (cache[i][0] == tag && cache[i][block_size + 1] == 1) {
                cout << "hit " << ind << endl;
                n_hit++;
                // if (read_or_write == "READ") {
                //     return_value = cache[i][offset + 1];
                // }
                // else {
                //     cache[i][offset + 1] = data;b
                // }
                was_hit = 1;
                was_miss = 0;
                lastUsedIndex[i] = ind;
                continue;
            }
        }


        if (was_miss) {
            n_miss++;
            cout << "miss " << ind << endl;
            ll lru_index = set_no * ways;
            ll mini = lastUsedIndex[set_no * ways];
            for (int i = set_no * ways; i < set_no * ways + ways; i++) {
                if (lastUsedIndex[i] < mini) {
                    mini = lastUsedIndex[i];
                    lru_index = i;
                }
            }
            lastUsedIndex[lru_index] = ind;        // setting counter of that index to the newest value

            ll address_of_block_to_be_replaced = (cache[lru_index][0] << (int)log2(sets)) + set_no;
            // ll address_of_block_to_be_replaced = (cache[lru_index][0]);

            // cout << "lru_index " << lru_index << endl;

            // cout << "tag of lru: " << cache[lru_index][0] << " " << set_no << " " << " " << (address.to_ulong() >> (int)log2(block_size)) << endl;
            // cout << address_of_block_to_be_replaced << endl;

            // cout << "fine1" << endl;
            cache[lru_index][0] = tag;
            if (cache[lru_index][block_size + 1] == 1) {
                for (int i = 1; i <= block_size; i++) {
                    memory[address_of_block_to_be_replaced][i - 1] = cache[lru_index][i];
                }
            }
            // cout << "fine2" << endl;
            cache[lru_index][block_size + 1] = 0;
            // cout << "fine3" << endl;
            for (int i = 1; i <= block_size; i++) {
                cache[lru_index][i] = memory[address.to_ulong() >> (int)log2(block_size)][i - 1];
            }
            // cout << "fine4" << endl;
            // if (read_or_write == "READ") {
            //     return_value = cache[lru_index][offset + 1];
            cache[lru_index][block_size + 1] = 1;
            // }
            // else {
            // cout << "fine5" << endl;
            //     cache[lru_index][offset + 1] = data;
            //     cache[lru_index][block_size+1] = 1;
            // }
            // cout << cache[lru_index][offset + 1] << " " << memory[address_of_block_to_be_replaced][offset] << endl;
            // cout << "fine" << endl;
        }

    }

    // for (int i = 0; i < (size_of_memory); i++) {
    //     long long z = block_size;
    //     for (int j = 0; j < z; j++) {
    //         cout << memory[i][z] << " ";
    //     }
    //     cout << '\n';
    // }

    // cout << "cache:" << endl;

    // for (int i = 0; i < total_indices; i++) {
    //     for (ll j = 0; j < block_size + 2; j++) {
    //         cout << cache[i][j] << " ";
    //     }
    //     cout << '\n';
    // }

    cout << "hit rate: " << ((double)n_hit / (ind - 1)) * 100 << "% " << n_miss << " " << n_hit << endl; // hit rate read hits write
}
