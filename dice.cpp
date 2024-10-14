#include <iostream>
#include <vector>
#include <algorithm>

#include "dice.h"
#include "vertex.h"
#include "face.h"
#include "edge.h"


void Dice::init(int n_e, int n_v, int n_f){
    if(n_v + n_f != n_e + 2){
        std::cerr << "illegal number of e,v,f" << std::endl;
        exit(1);
    }
    E.resize(n_e*2);
    V.resize(n_v);
    F.resize(n_f);

    for(int i=0; i<E.size(); i++){
        E[i] = new Edge;
    }
    for(int i=0; i<V.size(); i++){
        V[i] = new Vertex;
        V[i]->id = i;
    }
    for(int i=0; i<F.size(); i++){
        F[i] = new Face;
        F[i]->id = i;
    }
}
void Dice::setEdge(int e_i, int v_i, int f_i, int e_prev_i, int e_next_i, int e_sym_i){
    E[e_i]->setMember(V[v_i], F[f_i], E[e_prev_i], E[e_next_i], E[e_sym_i]);
}
void Dice::auto_connect(){
    for(Edge *ep: E){
        ep->f->e = ep;
        ep->v->e = ep;
    }

    for(Face *fp: F){
        if(fp->opp != NULL) fp->opp->opp = fp;
    }
}
int Dice::n_faces(){ // n面体のnを返す
    return F.size();
}
int Dice::m_corners(){ // m角形のmを返す
    return F[0]->count_neighbor_faces();
}

/* public */
bool Dice::satisfy_o(){ // 反対面制約を満たすかどうか
    int n = n_faces();
    if(F[0]->sum_opposite() < 0) return true; // 反対面なしなら命題論理よりtrue
    for(Face *f: F){
        if(f->sum_opposite() != n+1 ) return false;
    }
    return true;
}
bool Dice::satisfy_v(){ // 均衡頂点制約を満たすかどうか
    int n = n_faces();
    int xFv, sizeFv;
    for(Vertex *v: V){
        xFv = v->sum_adjacent_faces();
        sizeFv = v->count_adjacent_faces();
        if( abs(2*xFv - sizeFv*(n+1)) >= 2 ) return false;
    }
    return true;
}
bool Dice::satisfy_f(){ // 均衡面制約を満たすかどうか
    int n = n_faces();
    int m = m_corners();
    int xFf;
    for(Face *f: F){
        xFf = f->sum_neighbor_faces();
        if( abs(2*xFf - m*(n+1)) >= 2 ) return false;
    }
    return true;
}

void Dice::print_connection(){
    for(int i=0; i<this->F.size(); i++){
        std::cout << "Face" << i << "'s neighbor: ";
        this->F[i]->print_neighboring_faces();
    }
    for(int i=0; i<this->F.size(); i++){
        std::cout << "Face" << i << "'s opposite: ";
        this->F[i]->print_opposite_face();
    }
    for(int i=0; i<this->V.size(); i++){
        std::cout << "Vertex" << i << "'s adjacent: ";
        this->V[i]->print_adjacent_faces();
    }
}
void Dice::print_config(){
    std::cout << "configuration: ";
    for(Face *f: F){
        std::cout << f->num << ", ";
    }
    std::cout << std::endl;
}
bool Dice::unique_config(){
    int range;
    int has_opp_face = F[0]->opp != NULL;
    vector<Face*>::iterator min_p, min_in_range_p;

    range = (F.size()-1 -has_opp_face ) / k_fold;
    min_p = std::min_element(F.begin()+1, F.end() -has_opp_face, [](Face* a, Face *b){
        return a->num < b->num;
    });
    min_in_range_p = std::min_element(F.begin()+1, F.begin()+1+range, [](Face* a, Face *b){
        return a->num < b->num;
    });
    return min_p == min_in_range_p;
    /* 回転対称部分の面の数字のうち、最小値が配列の先頭の枠に来ているなら */
    // ユニークな配置(回転対称の配置で最初のもの)
}
void Dice::create_polyhedron(PolyHedron ph){
    switch (ph) {
        case TETRA:
        this->init(6, 4, 4);
        this->k_fold = 3;

        this->setEdge(0, 0, 0, 2, 1, 11);
        this->setEdge(1, 1, 0, 0, 2, 5);
        this->setEdge(2, 3, 0, 1, 0, 6);

        this->setEdge(3, 1, 1, 5, 4, 10);
        this->setEdge(4, 2, 1, 3, 5, 7);
        this->setEdge(5, 3, 1, 4, 3, 1);

        this->setEdge(6, 0, 2, 8, 7, 2);
        this->setEdge(7, 3, 2, 6, 8, 4);
        this->setEdge(8, 2, 2, 7, 6, 9);

        this->setEdge(9, 0, 3, 11, 10, 8);
        this->setEdge(10, 2, 3, 9, 11, 3);
        this->setEdge(11, 1, 3, 10, 9, 0);

        for(int i=0; i<this->F.size(); i++){
            this->F[i]->opp = NULL;
        }
        break; // tetra

        case CUBE:
        this->init(12, 8, 6);
        this->k_fold = 4;

        this->setEdge(0, 3, 0, 3, 1, 6);
        this->setEdge(1, 0, 0, 0, 2, 11);
        this->setEdge(2, 1, 0, 1, 3, 12);
        this->setEdge(3, 2, 0, 2, 0, 17);

        this->setEdge(4, 7, 1, 7, 5, 22);
        this->setEdge(5, 4, 1, 4, 6, 8);
        this->setEdge(6, 0, 1, 5, 7, 0);
        this->setEdge(7, 3, 1, 6, 4, 16);

        this->setEdge(8, 0, 2, 11, 9, 5);
        this->setEdge(9, 4, 2, 8, 10, 21);
        this->setEdge(10, 5, 2, 9, 11, 13);
        this->setEdge(11, 1, 2, 10, 8, 1);

        this->setEdge(12, 2, 3, 15, 13, 2);
        this->setEdge(13, 1, 3, 12, 14, 10);
        this->setEdge(14, 5, 3, 13, 15, 20);
        this->setEdge(15, 6, 3, 14, 12, 18);

        this->setEdge(16, 7, 4, 19, 17, 7);
        this->setEdge(17, 3, 4, 16, 18, 3);
        this->setEdge(18, 2, 4, 17, 19, 15);
        this->setEdge(19, 6, 4, 18, 16, 23);

        this->setEdge(20, 6, 5, 23, 21, 14);
        this->setEdge(21, 5, 5, 20, 22, 9);
        this->setEdge(22, 4, 5, 21, 23, 4);
        this->setEdge(23, 7, 5, 22, 20, 19);

        this->F[0]->opp = this->F[5];
        this->F[1]->opp = this->F[3];
        this->F[2]->opp = this->F[4];
        this->F[3]->opp = this->F[1];
        this->F[4]->opp = this->F[2];
        this->F[5]->opp = this->F[0];
        break; //cube

        case OCTA:
        this->init(12, 6, 8);
        this->k_fold = 3;

        this->setEdge(0, 0, 0, 2, 1, 10);
        this->setEdge(1, 1, 0, 0, 2, 16);
        this->setEdge(2, 2, 0, 1, 0, 4);

        this->setEdge(3, 3, 1, 5, 4, 7);
        this->setEdge(4, 0, 1, 3, 5, 2);
        this->setEdge(5, 2, 1, 4, 3, 18);

        this->setEdge(6, 4, 2, 8, 7, 11);
        this->setEdge(7, 0, 2, 6, 8, 3);
        this->setEdge(8, 3, 2, 7, 6, 22);

        this->setEdge(9, 4, 3, 11, 10, 13);
        this->setEdge(10, 1, 3, 9, 11, 0);
        this->setEdge(11, 0, 3, 10, 9, 6);

        this->setEdge(12, 5, 4, 14, 13, 17);
        this->setEdge(13, 1, 4, 12, 14, 9);
        this->setEdge(14, 4, 4, 13, 12, 21);

        this->setEdge(15, 5, 5, 17, 16, 19);
        this->setEdge(16, 2, 5, 15, 17, 1);
        this->setEdge(17, 1, 5, 16, 15, 12);

        this->setEdge(18, 3, 6, 20, 19, 5);
        this->setEdge(19, 2, 6, 18, 20, 15);
        this->setEdge(20, 5, 6, 19, 18, 23);

        this->setEdge(21, 5, 7, 23, 22, 14);
        this->setEdge(22, 4, 7, 21, 23, 8);
        this->setEdge(23, 3, 7, 22, 21, 20);

        this->F[0]->opp = this->F[7];
        this->F[1]->opp = this->F[4];
        this->F[2]->opp = this->F[5];
        this->F[3]->opp = this->F[6];
        this->F[4]->opp = this->F[1];
        this->F[5]->opp = this->F[2];
        this->F[6]->opp = this->F[3];
        this->F[7]->opp = this->F[0];
        break;

        case DODECA:
        this->init(30, 20, 12);
        this->k_fold = 5;

        this->setEdge(0, 0, 0, 4, 1, 15);
        this->setEdge(1, 1, 0, 0, 2, 25);
        this->setEdge(2, 2, 0, 1, 3, 35);
        this->setEdge(3, 3, 0, 2, 4, 45);
        this->setEdge(4, 4, 0, 3, 0, 5);

        this->setEdge(5, 0, 1, 9, 6, 4);
        this->setEdge(6, 4, 1, 5, 7, 49);
        this->setEdge(7, 13, 1, 6, 8, 52);
        this->setEdge(8, 14, 1, 7, 9, 13);
        this->setEdge(9, 5, 1, 8, 5, 16);

        this->setEdge(10, 15, 2, 14, 11, 58);
        this->setEdge(11, 16, 2, 10, 12, 24);
        this->setEdge(12, 6, 2, 11, 13, 17);
        this->setEdge(13, 5, 2, 12, 14, 8);
        this->setEdge(14, 14, 2, 13, 10, 51);

        this->setEdge(15, 1, 3, 19, 16, 0);
        this->setEdge(16, 0, 3, 15, 17, 9);
        this->setEdge(17, 5, 3, 16, 18, 12);
        this->setEdge(18, 6, 3, 17, 19, 23);
        this->setEdge(19, 7, 3, 18, 15, 26);

        this->setEdge(20, 16, 4, 24, 21, 57);
        this->setEdge(21, 17, 4, 20, 22, 34);
        this->setEdge(22, 8, 4, 21, 23, 27);
        this->setEdge(23, 7, 4, 22, 24, 18);
        this->setEdge(24, 6, 4, 23, 20, 11);

        this->setEdge(25, 2, 5, 29, 26, 1);
        this->setEdge(26, 1, 5, 25, 27, 19);
        this->setEdge(27, 7, 5, 26, 28, 22);
        this->setEdge(28, 8, 5, 27, 29, 33);
        this->setEdge(29, 9, 5, 28, 25, 36);

        this->setEdge(30, 17, 6, 34, 31, 56);
        this->setEdge(31, 18, 6, 30, 32, 44);
        this->setEdge(32, 10, 6, 31, 33, 37);
        this->setEdge(33, 9, 6, 32, 34, 28);
        this->setEdge(34, 8, 6, 33, 30, 21);

        this->setEdge(35, 3, 7, 39, 36, 2);
        this->setEdge(36, 2, 7, 35, 37, 29);
        this->setEdge(37, 9, 7, 36, 38, 32);
        this->setEdge(38, 10, 7, 37, 39, 43);
        this->setEdge(39, 11, 7, 38, 35, 46);

        this->setEdge(40, 18, 8, 44, 41, 55);
        this->setEdge(41, 19, 8, 40, 42, 54);
        this->setEdge(42, 12, 8, 41, 43, 47);
        this->setEdge(43, 11, 8, 42, 44, 38);
        this->setEdge(44, 10, 8, 43, 40, 31);

        this->setEdge(45, 4, 9, 49, 46, 3);
        this->setEdge(46, 3, 9, 45, 47, 39);
        this->setEdge(47, 11, 9, 46, 48, 42);
        this->setEdge(48, 12, 9, 47, 49, 53);
        this->setEdge(49, 13, 9, 48, 45, 6);

        this->setEdge(50, 19, 10, 54, 51, 59);
        this->setEdge(51, 15, 10, 50, 52, 14);
        this->setEdge(52, 14, 10, 51, 53, 7);
        this->setEdge(53, 13, 10, 52, 54, 48);
        this->setEdge(54, 12, 10, 53, 50, 41);

        this->setEdge(55, 19, 11, 59, 56, 40);
        this->setEdge(56, 18, 11, 55, 57, 30);
        this->setEdge(57, 17, 11, 56, 58, 20);
        this->setEdge(58, 16, 11, 57, 59, 10);
        this->setEdge(59, 15, 11, 58, 55, 50);

        this->F[0]->opp = this->F[11];
        this->F[1]->opp = this->F[6];
        this->F[2]->opp = this->F[7];
        this->F[3]->opp = this->F[8];
        this->F[4]->opp = this->F[9];
        this->F[5]->opp = this->F[10];

        this->F[6]->opp = this->F[1];
        this->F[7]->opp = this->F[2];
        this->F[8]->opp = this->F[3];
        this->F[9]->opp = this->F[4];
        this->F[10]->opp = this->F[5];
        this->F[11]->opp = this->F[0];
        break;


        case ICOSA:
        this->init(30, 12, 20);
        this->k_fold = 3;
//(int e_i, int v_i, int f_i, int e_prev_i, int e_next_i, int e_sym_i
        this->setEdge(0, 1, 0, 2, 1, 21);
        this->setEdge(1, 2, 0, 0, 2, 39);
        this->setEdge(2, 0, 0, 1, 0, 3);

        this->setEdge(3, 1, 1, 5, 4, 2);
        this->setEdge(4, 0, 1, 3, 5, 50);
        this->setEdge(5, 3, 1, 4, 3, 6);

        this->setEdge(6, 1, 2, 8, 7, 5);
        this->setEdge(7, 3, 2, 6, 8, 9);
        this->setEdge(8, 5, 2, 7, 6, 12);

        this->setEdge(9, 5, 3, 11, 10, 7);
        this->setEdge(10, 3, 3, 9, 11, 53);
        this->setEdge(11, 4, 3, 10, 9, 19);

        this->setEdge(12, 1, 4, 14, 13, 8);
        this->setEdge(13, 5, 4, 12, 14, 15);
        this->setEdge(14, 6, 4, 13, 12, 22);

        this->setEdge(15, 6, 5, 17, 16, 13);
        this->setEdge(16, 5, 5, 15, 17, 18);
        this->setEdge(17, 7, 5, 16, 15, 28);

        this->setEdge(18, 7, 6, 20, 19, 16);
        this->setEdge(19, 5, 6, 18, 20, 11);
        this->setEdge(20, 4, 6, 19, 18, 58);

        this->setEdge(21, 2, 7, 23, 22, 0);
        this->setEdge(22, 1, 7, 21, 23, 14);
        this->setEdge(23, 6, 7, 22, 21, 24);

        this->setEdge(24, 2, 8, 26, 25, 23);
        this->setEdge(25, 6, 8, 24, 26, 27);
        this->setEdge(26, 8, 8, 25, 24, 30);

        this->setEdge(27, 8, 9, 29, 28, 25);
        this->setEdge(28, 6, 9, 27, 29, 17);
        this->setEdge(29, 7, 9, 28, 27, 37);

        this->setEdge(30, 2, 10, 32, 31, 26);
        this->setEdge(31, 8, 10, 30, 32, 33);
        this->setEdge(32, 9, 10, 31, 30, 40);

        this->setEdge(33, 9, 11, 35, 34, 31);
        this->setEdge(34, 8, 11, 33, 35, 36);
        this->setEdge(35, 10, 11, 34, 33, 46);

        this->setEdge(36, 10, 12, 38, 37, 34);
        this->setEdge(37, 8, 12, 36, 38, 29);
        this->setEdge(38, 7, 12, 37, 36, 57);

        this->setEdge(39, 0, 13, 41, 40, 1);
        this->setEdge(40, 2, 13, 39, 41, 32);
        this->setEdge(41, 9, 13, 40, 39, 42);

        this->setEdge(42, 0, 14, 44, 43, 41);
        this->setEdge(43, 9, 14, 42, 44, 45);
        this->setEdge(44, 11, 14, 43, 42, 48);

        this->setEdge(45, 11, 15, 47, 46, 43);
        this->setEdge(46, 9, 15, 45, 47, 35);
        this->setEdge(47, 10, 15, 46, 45, 55);

        this->setEdge(48, 0, 16, 50, 49, 44);
        this->setEdge(49, 11, 16, 48, 50, 51);
        this->setEdge(50, 3, 16, 49, 48, 4);

        this->setEdge(51, 3, 17, 53, 52, 49);
        this->setEdge(52, 11, 17, 51, 53, 54);
        this->setEdge(53, 4, 17, 52, 51, 10);

        this->setEdge(54, 4, 18, 56, 55, 52);
        this->setEdge(55, 11, 18, 54, 56, 47);
        this->setEdge(56, 10, 18, 55, 54, 59);

        this->setEdge(57, 10, 19, 59, 58, 38);
        this->setEdge(58, 7, 19, 57, 59, 20);
        this->setEdge(59, 4, 19, 58, 57, 56);

        this->F[0]->opp = this->F[19];
        this->F[1]->opp = this->F[12];
        this->F[2]->opp = this->F[11];
        this->F[3]->opp = this->F[10];
        this->F[4]->opp = this->F[15];
        this->F[5]->opp = this->F[14];
        this->F[6]->opp = this->F[13];
        this->F[7]->opp = this->F[18];
        this->F[8]->opp = this->F[17];
        this->F[9]->opp = this->F[16];

        break;


        case TRI_TETRA:
        this->init(18, 8, 12);
        this->k_fold = 1;
//(int e_i, int v_i, int f_i, int e_prev_i, int e_next_i, int e_sym_i
        this->setEdge(0, 0, 0, 2, 1, 17);
        this->setEdge(1, 1, 0, 0, 2, 27);
        this->setEdge(2, 2, 0, 1, 0, 3);

        this->setEdge(3, 0, 1, 5, 4, 2);
        this->setEdge(4, 2, 1, 3, 5, 6);
        this->setEdge(5, 3, 1, 4, 3, 15);

        this->setEdge(6, 3, 2, 8, 7, 4);
        this->setEdge(7, 2, 2, 6, 8, 9);
        this->setEdge(8, 4, 2, 7, 6, 12);

        this->setEdge(9, 4, 3, 11, 10, 7);
        this->setEdge(10, 2, 3, 9, 11, 34);
        this->setEdge(11, 5, 3, 10, 9, 13);

        this->setEdge(12, 3, 4, 14, 13, 8);
        this->setEdge(13, 4, 4, 12, 14, 11);
        this->setEdge(14, 5, 4, 13, 12, 22);

        this->setEdge(15, 0, 5, 17, 16, 5);
        this->setEdge(16, 3, 5, 15, 17, 18);
        this->setEdge(17, 1, 5, 16, 15, 0);

        this->setEdge(18, 1, 6, 20, 19, 16);
        this->setEdge(19, 3, 6, 18, 20, 21);
        this->setEdge(20, 6, 6, 19, 18, 26);

        this->setEdge(21, 6, 7, 23, 22, 19);
        this->setEdge(22, 3, 7, 21, 23, 14);
        this->setEdge(23, 5, 7, 22, 21, 24);

        this->setEdge(24, 6, 8, 26, 25, 23);
        this->setEdge(25, 5, 8, 24, 26, 31);
        this->setEdge(26, 1, 8, 25, 24, 20);

        this->setEdge(27, 2, 9, 29, 28, 1);
        this->setEdge(28, 1, 9, 27, 29, 30);
        this->setEdge(29, 7, 9, 28, 27, 35);

        this->setEdge(30, 7, 10, 32, 31, 28);
        this->setEdge(31, 1, 10, 30, 32, 25);
        this->setEdge(32, 5, 10, 31, 30, 33);

        this->setEdge(33, 7, 11, 35, 34, 32);
        this->setEdge(34, 5, 11, 33, 35, 10);
        this->setEdge(35, 2, 11, 34, 33, 29);

        for(int i=0; i<this->F.size(); i++){
            this->F[i]->opp = NULL;
        }
        break;

        case PENTA_DIPY:
        this->init(15, 7, 10);
        this->k_fold = 1;
//(int e_i, int v_i, int f_i, int e_prev_i, int e_next_i, int e_sym_i
        this->setEdge(0, 0, 0, 2, 1, 5);
        this->setEdge(1, 1, 0, 0, 2, 14);
        this->setEdge(2, 2, 0, 1, 0, 15);

        this->setEdge(3, 0, 1, 5, 4, 20);
        this->setEdge(4, 3, 1, 3, 5, 6);
        this->setEdge(5, 1, 1, 4, 3, 0);

        this->setEdge(6, 1, 2, 8, 7, 4);
        this->setEdge(7, 3, 2, 6, 8, 28);
        this->setEdge(8, 4, 2, 7, 6, 9);

        this->setEdge(9, 1, 3, 11, 10, 8);
        this->setEdge(10, 4, 3, 9, 11, 25);
        this->setEdge(11, 5, 3, 10, 9, 12);

        this->setEdge(12, 1, 4, 14, 13, 11);
        this->setEdge(13, 5, 4, 12, 14, 22);
        this->setEdge(14, 2, 4, 13, 12, 1);

        this->setEdge(15, 0, 5, 17, 16, 2);
        this->setEdge(16, 2, 5, 15, 17, 21);
        this->setEdge(17, 6, 5, 16, 15, 18);

        this->setEdge(18, 0, 6, 20, 19, 17);
        this->setEdge(19, 6, 6, 18, 20, 29);
        this->setEdge(20, 3, 6, 19, 18, 3);

        this->setEdge(21, 6, 7, 23, 22, 16);
        this->setEdge(22, 2, 7, 21, 23, 13);
        this->setEdge(23, 5, 7, 22, 21, 24);

        this->setEdge(24, 6, 8, 26, 25, 23);
        this->setEdge(25, 5, 8, 24, 26, 10);
        this->setEdge(26, 4, 8, 25, 24, 27);

        this->setEdge(27, 6, 9, 29, 28, 26);
        this->setEdge(28, 4, 9, 27, 29, 7);
        this->setEdge(29, 3, 9, 28, 27, 19);

        for(int i=0; i<this->F.size(); i++){
            this->F[i]->opp = NULL;
        }
        break;


        case HEXA_DIPY:
        this->init(18, 8, 12);
        this->k_fold = 1;
//(int e_i, int v_i, int f_i, int e_prev_i, int e_next_i, int e_sym_i
        this->setEdge(0, 0, 0, 2, 1, 3);
        this->setEdge(1, 1, 0, 0, 2, 9);
        this->setEdge(2, 2, 0, 1, 0, 18);

        this->setEdge(3, 1, 1, 5, 4, 0);
        this->setEdge(4, 0, 1, 3, 5, 23);
        this->setEdge(5, 3, 1, 4, 3, 6);

        this->setEdge(6, 1, 2, 8, 7, 5);
        this->setEdge(7, 3, 2, 6, 8, 26);
        this->setEdge(8, 4, 2, 7, 6, 16);

        this->setEdge(9, 2, 3, 11, 10, 1);
        this->setEdge(10, 1, 3, 9, 11, 12);
        this->setEdge(11, 6, 3, 10, 9, 28);

        this->setEdge(12, 6, 4, 14, 13, 10);
        this->setEdge(13, 1, 4, 12, 14, 15);
        this->setEdge(14, 5, 4, 13, 12, 31);

        this->setEdge(15, 5, 5, 17, 16, 13);
        this->setEdge(16, 1, 5, 15, 17, 8);
        this->setEdge(17, 4, 5, 16, 15, 34);

        this->setEdge(18, 0, 6, 20, 19, 2);
        this->setEdge(19, 2, 6, 18, 20, 27);
        this->setEdge(20, 7, 6, 19, 18, 21);

        this->setEdge(21, 0, 7, 23, 22, 20);
        this->setEdge(22, 7, 7, 21, 23, 24);
        this->setEdge(23, 3, 7, 22, 21, 4);

        this->setEdge(24, 3, 8, 26, 25, 22);
        this->setEdge(25, 7, 8, 24, 26, 35);
        this->setEdge(26, 4, 8, 25, 24, 7);

        this->setEdge(27, 7, 9, 29, 28, 19);
        this->setEdge(28, 2, 9, 27, 29, 11);
        this->setEdge(29, 6, 9, 28, 27, 30);

        this->setEdge(30, 7, 10, 32, 31, 29);
        this->setEdge(31, 6, 10, 30, 32, 14);
        this->setEdge(32, 5, 10, 31, 30, 33);

        this->setEdge(33, 7, 11, 35, 34, 32);
        this->setEdge(34, 5, 11, 33, 35, 17);
        this->setEdge(35, 4, 11, 34, 33, 25);

        this->F[0]->opp = this->F[11];
        this->F[1]->opp = this->F[10];
        this->F[2]->opp = this->F[9];
        this->F[3]->opp = this->F[8];
        this->F[4]->opp = this->F[7];
        this->F[5]->opp = this->F[6];
        break;

    }

    this->auto_connect();
}
