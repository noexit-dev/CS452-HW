typedef void *Mtq;

Mtq mtq_new();
void mtq_del(Mtq q, DeqMapF f);
void mtq_tail_put(Mtq q, Data d);
Data mtq_head_get(Mtq q);