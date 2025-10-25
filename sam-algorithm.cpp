#include <bits/stdc++.h>
using namespace std;

struct State {
    int next[128];
    int link;
    int len;
    int first_pos;
    int mn;
    State() {
        for (int i = 0; i < 128; i++) next[i] = -1;
        link = -1;
        len = 0;
        first_pos = -1;
        mn = INT_MAX;
    }
};

struct SuffixAutomaton {
    vector<State> st;
    int last;
    SuffixAutomaton(int maxlen = 1000) {
        st.reserve(2 * maxlen);
        st.push_back(State());
        last = 0;
    }
    void extend(char c, int pos) {
        int cc = (unsigned char)c;
        int cur = (int)st.size();
        st.push_back(State());
        st[cur].len = st[last].len + 1;
        st[cur].first_pos = pos;
        int p = last;
        
        while (p != -1 && st[p].next[cc] == -1) {
            st[p].next[cc] = cur;
            p = st[p].link;
        }
        
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[cc];
            
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = (int)st.size();
                st.push_back(st[q]);
                st[clone].len = st[p].len + 1;
                st[clone].first_pos = st[q].first_pos;
                
                while (p != -1 && st[p].next[cc] == q) {
                    st[p].next[cc] = clone;
                    p = st[p].link;
                }
                
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }
    
    void build(const string &s) {
        for (int i = 0; i < (int)s.size(); i++)
            extend(s[i], i);
            
        int sz = (int)st.size();
        vector<vector<int>> buckets;
        int maxlen = 0;
        
        for (int i = 0; i < sz; i++)
            if (st[i].len > maxlen) maxlen = st[i].len;
            
        buckets.assign(maxlen + 1, vector<int>());
        
        for (int i = 0; i < sz; i++) buckets[st[i].len].push_back(i);
        
        for (int l = maxlen; l >= 1; l--) {
        	
            for (int v : buckets[l]) {
                int p = st[v].link;
                
                if (p != -1)
                    st[p].first_pos = max(st[p].first_pos, st[v].first_pos);
            }
        }
        
        for (int i = 0; i < sz; i++) st[i].mn = st[i].len;
    }
    
    void incorporate_string(const string &t) {
        int sz = (int)st.size();
        vector<int> best(sz, 0);
        int v = 0, l = 0;
        
        for (int i = 0; i < (int)t.size(); i++) {
            int cc = (unsigned char)t[i];
            
            if (st[v].next[cc] != -1) {
                v = st[v].next[cc];
                l++;
            } else {
            	
                while (v != -1 && st[v].next[cc] == -1)
                    v = st[v].link;
                    
                if (v == -1) {
                    v = 0;
                    l = 0;
                    continue;
                }
                
                l = st[v].len + 1;
                v = st[v].next[cc];
            }
            
            best[v] = max(best[v], l);
        }
        
        int maxl = 0;
        
        for (int i = 0; i < sz; i++)
            if (st[i].len > maxl) maxl = st[i].len;
            
        vector<vector<int>> bylen(maxl + 1);
        
        for (int i = 0; i < sz; i++) bylen[st[i].len].push_back(i);
        
        for (int len = maxl; len >= 0; len--) {
            for (int state : bylen[len]) {
                int p = st[state].link;
                
				if (p != -1)
                    best[p] = max(best[p], min(best[state], st[p].len));
            }
        }
        
        for (int i = 0; i < sz; i++) st[i].mn = min(st[i].mn, best[i]);
    }
    
    pair<int, string> longest_common_substring(const string &s0) {
        int best_len = 0;
        int best_pos = -1;
        
        for (int i = 0; i < (int)st.size(); i++) {
            if (st[i].mn > best_len) {
                best_len = st[i].mn;
                best_pos = st[i].first_pos;
            }
        }
        
        if (best_len == 0) return make_pair(0, string());
        return make_pair(best_len, s0.substr(best_pos - best_len + 1, best_len));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int k;
    cout << "Input number of strings: " << flush;
    cin >> k;

    vector<string> arr(k);
    cout << "\n\nInput " << k << " strings:\n" << flush;
    for (int i = 0; i < k; i++) {
        cout << "String " << i + 1 << ": " << flush;
        cin >> arr[i];
    }

    if (k == 0) {
        cout << "0\n\n";
        return 0;
    }

    SuffixAutomaton sam((int)arr[0].size());
    sam.build(arr[0]);
    for (int i = 1; i < k; i++)
        sam.incorporate_string(arr[i]);

    auto res = sam.longest_common_substring(arr[0]);

    cout << "\n\n--- Result ---\n";
    cout << "Length of longest common substring: " << res.first << "\n";
    cout << "Substring: " << (res.first > 0 ? res.second : "(none)") << "\n";

    return 0;
}

