class Vertex;
class Face;

class Edge
{
public:
    void setMember(Vertex *_v, Face *_f, Edge *_prev, Edge *_next, Edge *_sym);

    Vertex *v;
    Face *f;
    Edge *prev;
    Edge *next;
    Edge *sym; // 向きが逆の辺
};
