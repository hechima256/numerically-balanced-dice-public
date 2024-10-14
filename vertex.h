class Edge;

class Vertex
{
public:
    void print_adjacent_faces();
    int sum_adjacent_faces(); // 均衡頂点制約(この頂点周りの面の数字の合計を返す)
    int count_adjacent_faces();

    int id;
    Edge *e;
};
