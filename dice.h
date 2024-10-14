using namespace std;

class Vertex;
class Edge;
class Face;

typedef enum{
    TETRA,   // 正四面体
    CUBE,    // 正六面体(立方体)
    OCTA,    // 正八面体
    DODECA,  // 正十二面体
    ICOSA,   // 正二十面体
    TRI_TETRA, // 三方四面体
    PENTA_DIPY, // 双5角錐
    HEXA_DIPY, // 双6角錐


} PolyHedron;



class Dice { // 凸等面多面体ダイスのクラス
private:
    void init(int n_e, int n_v, int n_f); // 辺数, 頂点数, 面数
    void setEdge(int e_i, int v_i, int f_i, int e_prev_i, int e_next_i, int e_sym_i);
    void auto_connect(); // 反対面以外の情報を自動登録
    int n_faces(); // n面体のnを返す
    int m_corners(); // m角形のmを返す.

public:
    void create_polyhedron(PolyHedron ph);
    void print_connection();
    void print_config();
    bool unique_config(); // 回転対称の配置でないかどうか

    bool satisfy_o(); // 反対面制約を満たすかどうか
    bool satisfy_v(); // 均衡頂点制約を満たすかどうか
    bool satisfy_f();  // 均衡面制約を満たすかどうか



    vector<Edge*> E;
    vector<Vertex*> V;
    vector<Face*> F;
    int k_fold; // k_fold rotational symmetry. (k回転対称)
};
