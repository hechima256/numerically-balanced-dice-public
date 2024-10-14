#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

#include "vertex.h"
#include "edge.h"
#include "face.h"
#include "dice.h"


enum restriction {
    NONE= 0,
    O= 1, // 001
    V= 2, // 010
    F= 4, // 100
    NUM_RESTRICTION=8,
};

void print_sum(int satisfy_count[NUM_RESTRICTION]){
    std::cout << "O: " << satisfy_count[O] + satisfy_count[O+V] + satisfy_count[O+F] + satisfy_count[O+V+F];
    std::cout << ", V: " << satisfy_count[V] + satisfy_count[O+V] + satisfy_count[V+F] + satisfy_count[O+V+F];
    std::cout << ", F: " << satisfy_count[F] + satisfy_count[V+F] + satisfy_count[O+F] + satisfy_count[O+V+F];

    std::cout << ", OV: " << satisfy_count[O+V] + satisfy_count[O+V+F];
    std::cout << ", VF: " << satisfy_count[V+F] + satisfy_count[O+V+F];
    std::cout << ", OF: " << satisfy_count[O+F] + satisfy_count[O+V+F];

    std::cout << ", OVF: " << satisfy_count[O+V+F] << std::endl;
    int total=0;
    for(int i=0; i<NUM_RESTRICTION; i++){
        total += satisfy_count[i];
    }
    std::cout << "number of configurations: " << total << std::endl;
}

/////////////////
/* brute_force */
/////////////////
bool next_config(Dice *d, vector<int> &configuration){
    bool conti;
    do{
        // begin()+1なのは、等面多面体の対称性からid:0の面には1を固定して割り当てるため
        conti = std::next_permutation(configuration.begin()+1, configuration.end() );
        for(int i=0; i<d->F.size(); i++) d->F[i]->num = configuration[i];
    }while(!d->unique_config());
    // 回転対称の配置なのでovfの判定の前にスキップ
    return conti;
}
void brute_force(Dice *d){ // 総当たりで配置の列挙を行う
    vector<int> configuration; // 配置の配列
    int s_o, s_v, s_f;
    int satisfy_count[NUM_RESTRICTION] = {0}; // FVObit形式.  5(101): FとO, (1)001: Oのみ.
    configuration.resize(d->F.size());
    for(int i=0; i<configuration.size(); i++){
        configuration[i] = i+1;
    } // 1, 2, ..., nで初期化

    do{
        for(int i=0; i<d->F.size(); i++) d->F[i]->num = configuration[i];
        s_o=d->satisfy_o(); s_v=d->satisfy_v(); s_f=d->satisfy_f();
        satisfy_count[s_o*O + s_v*V + s_f*F]++; // カウント
        if( s_o || s_v || s_f ) {
            d->print_config();
            std::cout << "O: " << s_o << std::endl;
            std::cout << "V: " << s_v << std::endl;
            std::cout << "F: " << s_f << std::endl;
        }
    }while(next_config(d, configuration));
    print_sum(satisfy_count);
}
/////////////////
/* brute_force */
/////////////////


////////////////
/* back_track */
////////////////
bool allocate_num(Dice *d, int id, int num, set<int> &nums){
    if(d->F[id]->num != 0) return false;
    if( nums.erase(num) ){ // 未使用だったなら
        d->F[id]->num = num;
        return true;
    }else{ // すでに他の面で使用されていたら
        return false;
    }
}
bool unallocate_num(Dice *d, int id, set<int> &nums){
    int num;
    if(d->F[id]->num == 0) return false;
    num = d->F[id]->num;
    d->F[id]->num = 0;
    nums.insert(num);
    return true;
}
void get_legals(Dice *d, int id, set<int> &nums, set<int> &legals, int r){
    int n = d->F.size();
    if(r >= F){
        r-= F;
        // legal F
    }
    if(r >= V){
        r-= V;
        // legal V
    }
    if(r >= O){
        r-= O;
        if(d->F[id]->opp->num == 0){ // 反対面がまだ埋まっていないとき
            legals = nums;
            return;
        }
        //埋まっている場合は制約を満たすもののみ
        for(int num: nums){
            if(num + d->F[id]->opp->num == n+1) legals.insert(num);
        }
    }
}
void allocate(Dice *d, int id, set<int> &nums){
    int n = d->F.size();
    set<int> legals;
    if(id==n){
        if(d->unique_config()) d->print_config();
        return;
    }
    get_legals(d, id, nums, legals, O);
    for(auto num: legals){
        allocate_num(d, id, num, nums);
        allocate(d, id+1, nums);
        unallocate_num(d, id, nums);
    }
}
void back_track(Dice *d){
    set<int> nums; // 未割り当ての数字の集合
    for(int i=0; i<d->F.size(); i++){
        d->F[i]->num = 0;
        nums.insert(i+1);
    }
    allocate_num(d, 0, 1, nums);
    allocate(d, 1, nums);
}

int main(){
    Dice d;

    d.create_polyhedron(OCTA);
    // d.print_connection();

    brute_force(&d);
    // back_track(&d);

    return 0;
}
