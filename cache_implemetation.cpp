#include <bits/stdc++.h>
#include <math.h>
using namespace std;
typedef long long ll;
typedef vector<long long> vll;



int main(){

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    #ifndef ONLINE_JUDGE // input and output files
        freopen("input.in", "r", stdin);
        freopen("output.in", "w", stdout);
    #endif

    map<long long, long long> lastUsedIndex;
    vector<vll> memory(1 << 9);
    vector<vll> cache(256);

    for (int i = 0; i < (1<<9) ; i++){
        long long z = 8;
        while (z--){
            memory[i].push_back(rand() % (100)); // 2^28
        }
    }

    for (int i = 0; i < 256; i++){
        long long z = 10;
        while (z--){
            cache[i].push_back(0);
        }
    }

    for (int i = 0; i < (1 << 9); i++){
        long long z = memory[i].size();
        while (z--){
            cout << memory[i][z] << " "; 
        }
        cout << '\n';
    }

    cout << "cache:" << endl;

    for (int i = 0; i < 256; i++){
        for(ll j=0;j<cache[0].size();j++){
            cout << cache[i][j] << " ";
        }
        cout << '\n';
    }

    ll ind = 0, n_hit = 0, n_miss = 0;
    bool was_hit = 0, was_miss = 1;

    while (true){
        ind++;
        was_hit = 0;
        was_miss = 1;
        ll return_value;

        string read_or_write, addr;
        ll data;
        cin >> read_or_write;
        cout << read_or_write << endl;

        if (read_or_write == "HLT"){ 
            break;
        }
        if (read_or_write == "READ"){
            cin >> addr;
        }
        else{
            cin >> addr;
            cin >> data;
            cout << "data:" << data << endl;
        }

        bitset<32> address(addr);                         // bitsets for easier access
        ll offset = ((1 << 3) - 1) & address.to_ulong();
        ll set_no = ((1 << 6) - 1) & (address.to_ulong() >> 3);
        ll tag = (address.to_ulong() >> 9);
        
        cout << "tag::" << tag << " " << set_no << " " << offset << endl;

        for (int i = set_no * 4; i < set_no * 4 + 4; i++){
            if (cache[i][0] == tag && cache[i][9] == 1){
                cout << "hit" << ind << endl;
                n_hit++;
                if (read_or_write == "READ"){
                    return_value = cache[i][offset + 1];
                }
                else{
                    cache[i][offset + 1] = data;
                }
                was_hit = 1;
                was_miss = 1 - was_hit;
                lastUsedIndex[i] = ind; 
                break;
            }
        }


        if (was_miss)
        {
            n_miss++;
            cout << "miss" << ind << endl;
            ll lru_index = set_no * 4;
            ll mini = lastUsedIndex[set_no * 4];
            for (int i = set_no * 4; i < set_no * 4 + 4; i++){
                if (lastUsedIndex[i] < mini){
                    mini = lastUsedIndex[i];
                    lru_index = i;
                }
            }
            lastUsedIndex[lru_index] = ind;

            ll address_of_block_to_be_replaced = (cache[lru_index][0] << 9) + (set_no << 3);

            cout << "tag of lru:" << cache[lru_index][0] << " " << set_no << " " << (address_of_block_to_be_replaced >> 3) << " " << (address.to_ulong() >> 3) << endl;
            cache[lru_index][0] = tag;
            if (cache[lru_index][9] == 1){
                for (int i = 1; i <= 8; i++){
                    memory[address_of_block_to_be_replaced >> 3][i - 1] = cache[lru_index][i];
                }
            }
            cache[lru_index][9] = 0;

            for (int i = 1; i <= 8; i++){
                cache[lru_index][i] = memory[address.to_ulong() >> 3][i - 1];
            }

            if(read_or_write == "READ"){
                return_value = cache[lru_index][offset + 1];
                cache[lru_index][9] = 1;
            }
            else{
                cache[lru_index][offset + 1] = data;
                cache[lru_index][9] = 1;
            }
            cout << cache[lru_index][offset + 1] << " " << memory[address_of_block_to_be_replaced >> 3][offset] << endl;
        }

    }
    
    for (int i = 0; i < (1 << 9); i++){
        long long z = memory[i].size();
        while (z--){
            cout << memory[i][z] << " "; 
        }
        cout << '\n';
    }

    cout << "cache:" << endl;
    for (int i = 0; i < 256; i++){
        long long z = cache[i].size();
        while (z--){
            cout << cache[i][z] << " ";
        }
        cout << '\n';
    }

    cout << "hit rate:" << (double)n_hit/ (ind - 1) << endl;
    
}
