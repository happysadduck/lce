# ifndef POOL_H
# define POOL_H

typedef struct Pool Pool;

Pool*prepare_pool(int size,int cnt);
void destroy_pool(Pool*pool);
void*pool_alloc(Pool*pool);
int pool_return(Pool*pool,void*ptr);

# endif
