/**
 * generic_hash_table.h
 *
 * 纯C99通用哈希表 - 链表法解决冲突
 * 不分配堆内存，所有内存管理交给调用者
 */

#ifndef GENERIC_HASH_TABLE_H
#define GENERIC_HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ============ 配置与类型定义 ============ */

/* 哈希函数类型：将任意对象映射为64位哈希值 */
typedef uint64_t (*ght_hash_func_t)(const void *key, size_t key_size);

/* 键比较函数类型：返回0表示相等 */
typedef int (*ght_cmp_func_t)(const void *a, const void *b, size_t size);

/* 节点结构 - 调用者负责分配 */
typedef struct ght_node
{
    void *key;             /* 键指针（不拷贝数据，仅存储指针） */
    void *value;           /* 值指针（不拷贝数据，仅存储指针） */
    size_t key_size;       /* 键大小（用于比较） */
    size_t value_size;     /* 值大小（信息用途） */
    struct ght_node *next; /* 链表下一个节点 */
} ght_node_t;

/* 哈希表结构 - 调用者分配表本身，但桶数组需要单独分配 */
typedef struct
{
    ght_node_t **buckets; /* 桶数组指针（调用者分配） */
    size_t bucket_count;  /* 桶数量（必须是2的幂，用于位运算取模） */
    size_t size;          /* 当前元素数量 */

    /* 函数指针 */
    ght_hash_func_t hash_func;
    ght_cmp_func_t cmp_func;
} ght_table_t;

/* 迭代器结构 */
typedef struct
{
    const ght_table_t *table;
    size_t bucket_idx;
    ght_node_t *current;
} ght_iter_t;

/* ============ 核心API ============ */

/**
 * 初始化哈希表
 * @param table 调用者分配的表结构
 * @param buckets 调用者分配的桶数组（大小为bucket_count，初始化为NULL）
 * @param bucket_count 桶数量（必须为2的幂，如16, 32, 64...）
 * @param hash_func 哈希函数（可为NULL，使用默认FNV-1a）
 * @param cmp_func 比较函数（可为NULL，使用默认memcmp）
 */
void ght_init(ght_table_t *table,
              ght_node_t **buckets,
              size_t bucket_count,
              ght_hash_func_t hash_func,
              ght_cmp_func_t cmp_func);

/**
 * 清空表（不释放节点内存，仅断开链接）
 * 调用者需要自行遍历释放节点
 */
void ght_clear(ght_table_t *table);

/**
 * 插入/更新键值对
 * @param table 哈希表
 * @param node 调用者分配的节点（函数会设置其字段）
 * @param key 键指针
 * @param value 值指针
 * @param key_size 键大小
 * @param value_size 值大小
 * @return 如果键已存在，返回旧节点（需要调用者释放）；否则返回NULL
 */
ght_node_t *ght_insert(ght_table_t *table,
                       ght_node_t *node,
                       const void *key,
                       const void *value,
                       size_t key_size,
                       size_t value_size);

/**
 * 查找键
 * @return 找到的节点，未找到返回NULL
 */
ght_node_t *ght_find(const ght_table_t *table,
                     const void *key,
                     size_t key_size);

/**
 * 删除键
 * @return 被删除的节点（调用者负责释放），未找到返回NULL
 */
ght_node_t *ght_remove(ght_table_t *table,
                       const void *key,
                       size_t key_size);

/**
 * 获取当前元素数量
 */
static inline size_t ght_size(const ght_table_t *table)
{
    return table ? table->size : 0;
}

/**
 * 判断是否为空
 */
static inline bool ght_empty(const ght_table_t *table)
{
    return !table || table->size == 0;
}

/* ============ 迭代器API ============ */

/**
 * 初始化迭代器
 */
void ght_iter_init(ght_iter_t *iter, const ght_table_t *table);

/**
 * 获取下一个节点
 * @return 下一个节点，遍历结束返回NULL
 */
ght_node_t *ght_iter_next(ght_iter_t *iter);

/* ============ 默认哈希/比较函数 ============ */

/* FNV-1a 64位哈希 - 通用字节流哈希 */
uint64_t ght_default_hash(const void *data, size_t len);

/* 默认比较 - 使用memcmp */
int ght_default_cmp(const void *a, const void *b, size_t size);

/* 字符串专用哈希（以\0结尾） */
uint64_t ght_str_hash(const void *str, size_t unused);

/* 字符串专用比较 */
int ght_str_cmp(const void *a, const void *b, size_t unused);

/* 指针专用哈希（直接取地址值） */
uint64_t ght_ptr_hash(const void *ptr, size_t unused);

/* 指针专用比较 */
int ght_ptr_cmp(const void *a, const void *b, size_t unused);

#endif /* GENERIC_HASH_TABLE_H */