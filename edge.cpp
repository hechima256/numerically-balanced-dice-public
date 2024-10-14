#include "edge.h"

void Edge::setMember(Vertex *_v, Face *_f, Edge *_prev, Edge *_next, Edge *_sym){
    v = _v; f = _f; prev = _prev; next = _next; sym = _sym;
}
