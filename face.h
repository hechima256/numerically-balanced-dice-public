class Edge;

class Face{
public:
    void print_opposite_face();
    void print_neighboring_faces();
    int sum_opposite(); // 反対面制約(この面と反対面の数字の和を返す), 反対面なしなら-1
    int sum_neighbor_faces(); // 均衡面制約(この面に隣接する面の数字の和を返す)
    int count_neighbor_faces(); // 隣接する面の数

    int id;
    int num; // ダイスの数字
    Edge *e;
    Face *opp;
};
