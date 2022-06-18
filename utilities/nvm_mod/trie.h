#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <vector>

#include "my_log.h"

namespace rocksdb {
class Trie{
public:
    std::vector<InternalKey> zones_key;             // 分割区间的 internal key

    Trie(int keys_max_num_, int zones_num_):
        keys_max_num(keys_max_num_),
        zones_num(zones_num_){
        root = new Trie_node();
        zones_key.reserve(zones_num-1);
        keys_num = 0;
        RECORD_LOG("create Trie Success: zones_num:%d keys_max_num: %d\n",
            zones_num, keys_max_num);
    }

    ~Trie(){
        destory(root);
        RECORD_LOG("destory Trie success\n");
    }

    void insert(const Slice &key){
        if (!root && key.size() > 0) return;
        // buffer
        char buf[key.size()];
        // key 的 hex 编码
        strcpy(buf, key.ToString(true).c_str());
        // 遍历的指针
        Trie_node* node = root;
        // 目前的深度
        int depth_ = 0;
        for (std::size_t i = 0; i < strlen(buf); i++) {
            // 只统计部分前缀
            if(++depth_ >= depth){
                break;
            }
            if (begin <= buf[i] && buf[i] <= end){
                if(!node->next[buf[i] - begin])
                    node->next[buf[i] - begin] = new Trie_node();
                node = node->next[buf[i] - begin];
            }
        }
        // 更新词频
        node->status[0]++;
        node->key.DecodeFrom(key);
        keys_num++;
        if(keys_num >= keys_max_num){
            trie_completed = 1;
        }
    }

    int analyse_zones(){
        if (!root) return -1;
        std::queue<Trie_node*> q;
        Trie_node* node = root;
        q.push(node);
        // 获取叶子结点
        for (int depth_ = 0; depth_ < depth-1; depth_++){
            int n = q.size();
            for(int i = 0; i < n; i++){
                node = q.front(); q.pop();
                for(int j = 0; j < range; j++)
                {
                    if(node->next[j] != nullptr){
                        q.push(node->next[j]);
                    }
                }         
            }
        }
        int size = q.size();
        int length = static_cast<int>(round(static_cast<double>(size)/static_cast<double>(zones_num)));
        int start = length;
        for(int i = 0; i < size; i++){
            node = q.front(); q.pop();
            if(i == start){
                zones_key.push_back(node->key);
                start += length; 
                RECORD_LOG("zones_key:%s\n", node->key.DebugString(true).c_str());
            }     
        }
        return zones_key.size();    
    }

    int complete_trie(){ return trie_completed; }

    int get_nums(){ return keys_num; }

private:
    static const int begin = '0';				    // 字符范围上界
	static const int end = 'Z';				        // 字符范围下界
	static const int range = end - begin + 1;	    // 范围，最大的叶子节点数
    static const int depth  = 16;				    // 前缀树最大深度，采样key的前8B
    int keys_max_num = 1000;		                // 采样的 key 数目
    int keys_num;                                   // 目前采样的 key 数目
    int zones_num;                                  // 预分割的分区数
    int trie_completed = 0;                         // 标志位，预分割是否完成
    
    struct Trie_node{
        Trie_node* next[range] = { nullptr };       // 前缀树指针
        int status[1];                              // 统计前缀词频
        InternalKey key;                            // 记录前缀词
        Trie_node() : next{ nullptr }, status{ 0 }{}
    };

    Trie_node* root;

    void destory(Trie_node* node){
        if(!node)
            return ;
        for(int i = 0; i < range; i++)
        {   
            if(node->next[i] != nullptr){
                destory(node->next[i]);
            }          
        }
        delete node;
        node = nullptr;
    }
};
}