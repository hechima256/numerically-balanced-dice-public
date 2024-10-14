#include <iostream>

#include "vertex.h"
#include "edge.h"
#include "face.h"

void Vertex::print_adjacent_faces(){
    Edge *start = this->e;
    Edge *e = start;
    do{
        std::cout << e->f->id << " ";
        e = e->prev->sym;
    } while(e != start);
    std::cout << std::endl;
}
// 均衡頂点制約(この頂点周りの面の数字の合計を返す)
int Vertex::sum_adjacent_faces(){
    Edge *start = this->e;
    Edge *e = start;
    int sum = 0;
    do{
        sum += e->f->num;
        e = e->prev->sym;
    } while(e != start);
    return sum;
}
// この頂点周りの面の数
int Vertex::count_adjacent_faces(){
    Edge *start = this->e;
    Edge *e = start;
    int num = 0;
    do{
        num += 1;
        e = e->prev->sym;
    } while(e != start);
    return num;
}
