#include <bits/stdc++.h>
#include <unordered_map>
#include <math.h>
using namespace std;
typedef long long ll;

typedef long double ld;
typedef string ss;
typedef vector<long long> vll;
typedef stack<ll> skll;
typedef pair<char, int> pci;
typedef pair<ll, int> pll;
typedef map<ll, ll> mll;
typedef map<char, ll> mcl;
#define pb push_back
#define all(v) v.begin(), v.end()
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
#define fost(i, a, b, d) for (ll i = a; i < b; i += d)
#define ofst(i, a, b, d) for (ll i = a; i > b; i -= d)
#define forb(i, a, b) for (int i = a; i < b; i++)
#define fon(i, a, b) for (int i = a; i >= b; i--)
#define fo(i, n) for (int i = 0; i < n; i++)
#define itmp(mp) for (auto it = mp.begin(); it != mp.end(); it++)


int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
#ifndef ONLINE_JUDGE
  freopen("inputf.in", "r", stdin);
  freopen("outputf.in", "w", stdout);
#endif
  map<ll, ll> lastUseIndex;
  vector<vll> memory(1 << 14); // 1<<28
  vector<vll> cache(256);

  fo(i,1<<14){
    ll z = 8;
    while (z--)
      memory[i].pb(arc4random()%(100)); // 2^28
    // cout << memory[i][0] << endl;
  }

  fo(i, 256)
  {
    ll z = 9;
    while (z--)
     {
       cache[i].pb(arc4random() % (100));
      //  cout << i << " " << z << endl;
     }
    // cout << i << endl;
  }
  // testing:
  cache[0][0] = 10;
  for (int i = 1; i < 9; i++)
  {
    cache[0][i] = i * 8 - 9 * 2;
  }

  ll ind = 0, n_hit = 0, n_miss = 0;
  bool was_hit = 0;

  while (true)
  {
    ind++;
    was_hit = 0;
    ll ret_value;
    ss s;
    cin >> s;
    if (s == "HLT")
      break;
    bitset<32> address(s);
    ll offset = ((1 << 3) - 1) & address.to_ulong();
    ll set_no = ((1 << 6) - 1) & address.to_ulong() >> 3;
    ll tag = address.to_ulong() >> 9;
    // bitset<32> tagg(tag);
    // bitset<32> offset_bs(offset);
    // bitset<32> set_no_bs(set_no);

    cout << tag << " " << set_no << " " << offset << endl;

    for (int i = set_no * 4; i < set_no * 4 + 4; i++)
    {
      if (cache[i][0] == tag)
      {
        cout << "hit" << ind << endl;
        n_hit++;
        ret_value = cache[i][offset + 1];
        was_hit = 1;
        lastUseIndex[i] = ind; // sets the last hit index
        break;
      }
    }

    // cout << (address.to_ulong()) << " " << ret_value << endl;
    // MISS
    if (!was_hit)
    {
      n_miss++;
      cout << "miss" << ind << endl;
      ll lru_index = set_no * 4;
      ll min = lastUseIndex[set_no * 4];
      for (int i = set_no * 4; i < set_no * 4 + 4; i++)
      {
        if (lastUseIndex[i] < min)
        {
          min = lastUseIndex[i];
          lru_index = i;
        }
      }
      lastUseIndex[lru_index] = ind;
      // calc index of block to be replaced
      // cout
          // << lru_index << " " << min << endl;
      // !! write through , block at lru index to be written into mem !!

      ll address_of_blockToBeReplaced = (cache[lru_index][0] << 9) + (set_no << 3);

      cout << cache[lru_index][0] << " " <<set_no<<" "<< (address_of_blockToBeReplaced>>3 )<< endl;

      for (int i = 1; i <= 8; i++)
      {
        memory[address_of_blockToBeReplaced >> 3][i] = cache[lru_index][i];
      }

      cache[lru_index][0] = tag;
      for (int i = 1; i <= 8; i++)
      {
        cache[lru_index][i] = memory[address.to_ulong() >> 3][i];
      }
      ret_value = cache[lru_index][offset+1];
    }
    // cout << ret_value << " ret " <<endl;
    // cout << memory[address.to_ulong()>>3][offset+1] << endl;

  }
  cout <<"hit rate:"<< (double)n_hit / (ind-1 )<< endl;
  // cout << n_miss << endl;
  // 1 word=4byte(int) , 1 block= 8 word= 256 bit, offest=3 bit, setno=6 bits,address= 32 bits

  return 0;
}

