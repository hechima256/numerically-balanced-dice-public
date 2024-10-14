#include <iostream>

#include "face.h"
#include "edge.h"

void Face::print_opposite_face(){
    if(this->opp == NULL) {
        std::cout << "caution: this polyhedron has no opposite face." << std::endl;
    }else{
        std::cout << this->opp->id << std::endl;
    }
}
void Face::print_neighboring_faces(){
    Edge *start = this->e;
    Edge *e = start;
    do {
        std::cout << e->sym->f->id << " ";
        e = e->next;
    }while(e != start);
    std::cout << std::endl;
}
// 反対面制約(この面と反対面の数字の和を返す), 反対面なしなら-1
int Face::sum_opposite(){
    if(this->opp == NULL) return -1;
    return this->num + this->opp->num;
}
// 均衡面制約(この面に隣接する面の数字の和を返す)
int Face::sum_neighbor_faces(){
    Edge *start = this->e;
    Edge *e = start;
    int sum = 0;
    do {
        sum += e->sym->f->num;
        e = e->next;
    }while(e != start);
    return sum;
}
// 隣接する面の数
int Face::count_neighbor_faces(){
    Edge *start = this->e;
    Edge *e = start;
    int num = 0;
    do {
        num += 1;
        e = e->next;
    }while(e != start);
    return num;
}
