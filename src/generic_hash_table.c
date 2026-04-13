/**
 * generic_hash_table.c
 *
 * 纯C99实现
 */

#include "generic_hash_table.h"
#include <string.h>

/* ============ 内部辅助函数 ============ */

/* 计算桶索引：使用位运算（要求bucket_count为2的幂） */
static inline size_t get_bucket_idx(uint64_t hash, size_t bucket_mask)
{
    return (size_t)(hash & bucket_mask);
}

/* ============ 默认哈希实现 ============ */

uint64_t ght_default_hash(const void *data, size_t len)
{
    /* FNV-1a 64位算法 */
    const uint8_t *bytes = (const uint8_t *)data;
    uint64_t hash = 0xcbf29ce484222325ULL; /* FNV偏移基础值 */

    for (size_t i = 0; i < len; i++)
    {
        hash ^= bytes[i];
        hash *= 0x100000001b3ULL; /* FNV质数 */
    }

    return hash;
}

int ght_default_cmp(const void *a, const void *b, size_t size)
{
    return memcmp(a, b, size);
}

uint64_t ght_str_hash(const void *str, size_t unused)
{
    (void)unused;
    const char *s = (const char *)str;
    uint64_t hash = 0xcbf29ce484222325ULL;

    while (*s)
    {
        hash ^= (uint8_t)*s++;
        hash *= 0x100000001b3ULL;
    }

    return hash;
}

int ght_str_cmp(const void *a, const void *b, size_t unused)
{
    (void)unused;
    return strcmp((const char *)a, (const char *)b);
}

uint64_t ght_ptr_hash(const void *ptr, size_t unused)
{
    (void)unused;
    /* 对指针地址进行混合，避免低位过于集中 */
    uintptr_t p = (uintptr_t)ptr;
    return (uint64_t)(p ^ (p >> 4) ^ (p >> 9));
}

int ght_ptr_cmp(const void *a, const void *b, size_t unused)
{
    (void)unused;
    return (a == b) ? 0 : (a < b) ? -1
                                  : 1;
}

/* ============ 核心API实现 ============ */

void ght_init(ght_table_t *table,
              ght_node_t **buckets,
              size_t bucket_count,
              ght_hash_func_t hash_func,
              ght_cmp_func_t cmp_func)
{

    table->buckets = buckets;
    table->bucket_count = bucket_count;
    table->size = 0;

    /* 桶数量必须是2的幂，这里用断言或允许非2的幂用取模？
     * 为了性能，要求2的幂，用位运算取模
     */
    table->hash_func = hash_func ? hash_func : ght_default_hash;
    table->cmp_func = cmp_func ? cmp_func : ght_default_cmp;

    /* 清空桶数组 - 调用者应该已经做了，但再做一遍确保安全 */
    memset(buckets, 0, sizeof(ght_node_t *) * bucket_count);
}

void ght_clear(ght_table_t *table)
{
    if (!table)
        return;

    /* 只是断开所有链接，不释放节点内存 */
    for (size_t i = 0; i < table->bucket_count; i++)
    {
        table->buckets[i] = NULL;
    }
    table->size = 0;
}

ght_node_t *ght_insert(ght_table_t *table,
                       ght_node_t *node,
                       const void *key,
                       const void *value,
                       size_t key_size,
                       size_t value_size)
{

    if (!table || !node || !key)
        return NULL;

    uint64_t hash = table->hash_func(key, key_size);
    size_t idx = get_bucket_idx(hash, table->bucket_count - 1);

    /* 检查是否已存在 */
    ght_node_t **current = &table->buckets[idx];
    while (*current)
    {
        if ((*current)->key_size == key_size &&
            table->cmp_func((*current)->key, key, key_size) == 0)
        {
            /* 键已存在，替换值 */
            ght_node_t *old_node = *current;

            /* 新节点接替旧节点的位置 */
            node->key = (void *)key; /* 去掉const，但调用者不应修改 */
            node->value = (void *)value;
            node->key_size = key_size;
            node->value_size = value_size;
            node->next = old_node->next;

            *current = node;

            /* 返回旧节点给调用者处理 */
            return old_node;
        }
        current = &(*current)->next;
    }

    /* 新键，头插法 */
    node->key = (void *)key;
    node->value = (void *)value;
    node->key_size = key_size;
    node->value_size = value_size;
    node->next = table->buckets[idx];
    table->buckets[idx] = node;

    table->size++;
    return NULL;
}

ght_node_t *ght_find(const ght_table_t *table,
                     const void *key,
                     size_t key_size)
{

    if (!table || !key)
        return NULL;

    uint64_t hash = table->hash_func(key, key_size);
    size_t idx = get_bucket_idx(hash, table->bucket_count - 1);

    ght_node_t *current = table->buckets[idx];
    while (current)
    {
        if (current->key_size == key_size &&
            table->cmp_func(current->key, key, key_size) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

ght_node_t *ght_remove(ght_table_t *table,
                       const void *key,
                       size_t key_size)
{

    if (!table || !key)
        return NULL;

    uint64_t hash = table->hash_func(key, key_size);
    size_t idx = get_bucket_idx(hash, table->bucket_count - 1);

    ght_node_t **current = &table->buckets[idx];
    while (*current)
    {
        if ((*current)->key_size == key_size &&
            table->cmp_func((*current)->key, key, key_size) == 0)
        {

            ght_node_t *target = *current;
            *current = target->next;
            table->size--;

            target->next = NULL; /* 断开链接 */
            return target;
        }
        current = &(*current)->next;
    }

    return NULL;
}

/* ============ 迭代器实现 ============ */

void ght_iter_init(ght_iter_t *iter, const ght_table_t *table)
{
    iter->table = table;
    iter->bucket_idx = 0;
    iter->current = NULL;

    if (!table || table->size == 0)
        return;

    /* 找到第一个非空桶 */
    for (size_t i = 0; i < table->bucket_count; i++)
    {
        if (table->buckets[i])
        {
            iter->bucket_idx = i;
            iter->current = table->buckets[i];
            return;
        }
    }
}

ght_node_t *ght_iter_next(ght_iter_t *iter)
{
    if (!iter || !iter->table)
        return NULL;

    /* 当前桶还有节点 */
    if (iter->current)
    {
        ght_node_t *result = iter->current;
        iter->current = iter->current->next;
        return result;
    }

    /* 当前桶耗尽，找下一个非空桶 */
    iter->bucket_idx++;
    while (iter->bucket_idx < iter->table->bucket_count)
    {
        if (iter->table->buckets[iter->bucket_idx])
        {
            iter->current = iter->table->buckets[iter->bucket_idx]->next;
            return iter->table->buckets[iter->bucket_idx];
        }
        iter->bucket_idx++;
    }

    return NULL;
}