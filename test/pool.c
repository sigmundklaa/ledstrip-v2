
#include "utils/pool.h"
#include "common.h"
#include <assert.h>

struct test
{
    uint32_t a;
    uint8_t b;
};

static pool_byte_ty pool[POOL_REAL_SIZE(100, sizeof(struct test))];
static uint8_t pool_initialized = 0;

int main(void)
{
    p_init(pool, 100, sizeof(struct test));

    struct test *t = p_alloc(pool);
    struct pool *p = (struct pool *)pool;

    assert(p);
    DEBUG(printf("mem: %lu, comp: %lu, bitmap size: %zu\n", p->mem, ((uintptr_t)pool) + sizeof(*p) + 16, _bitmap_tot_sz(100)));
    assert(p->mem == ((uintptr_t)pool) + sizeof(*p) + 16);
    assert(*(pool_bitmap_ty *)(pool + sizeof(*p)) == 0x1);

#if 0
    for (int i = 1; i < 16; i++)
    {
        assert(!(*(pool_bitmap_ty *)(pool + sizeof(*p) + i)));
    }
#endif

    for (int i = 0; i < 99; i++)
    {
        p_alloc(pool);
    }

    assert(!p_alloc(pool));
    p_free(pool, t);
    assert((uintptr_t)p_alloc(pool) == ((uintptr_t)pool) + sizeof(*p) + 16);

    struct test *x = (void *)p->mem;
    for (int i = 0; i < 100; i++)
    {
        p_free(pool, x + i);
    }

    for (int i = 0; i < 12; i++)
    {
        assert(!(*(pool_bitmap_ty *)(pool + sizeof(*p) + i)));
    }

    return 0;
}