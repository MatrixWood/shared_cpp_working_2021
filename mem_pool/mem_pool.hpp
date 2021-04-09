#include <iostream>

#define MAX_MEMORYHEAD_SIZE 12
#define MAGIC_CODE          0x123456  

struct mem_block {
    mem_block* _next;
    mem_block* _prev;
    void*      _brick;
    void Init() {
        _next = nullptr;
        _prev = nullptr;
        _brick = nullptr;
    }
    mem_block() {
        Init;
    }
};

struct mem_list {
    mem_list* _mem_list_next;
    mem_block* _mem_free;
    mem_block* _mem_free_last;
    mem_block* _mem_used;
    mem_block* _mem_used_last;
    int _size;
    void Init() {
        _mem_list_next = nullptr;
        _mem_free = nullptr;
        _mem_free_last = nullptr;
        _mem_used = nullptr;
        _mem_used_last = nullptr;
        _size = 0;
    }
    mem_list() {
        Init();
    }
};

class mem_pool {
public:
    static mem_pool& Instance() {
        if (_mem_pool == nullptr) {
            _mem_pool = (mem_pool*)malloc(sizeof(mem_pool));
            _mem_pool->Init();
        }
        return *_mem_pool;
    }

    ~mem_pool() {
        Close();
    }

    void* GetBuff(size_t buff_size) {
        void* p_buff = nullptr;
        if (_mem_list == nullptr) {
            p_buff = malloc(buff_size + MAX_MEMORYHEAD_SIZE);
            if (p_buff == nullptr) return nullptr;
            _mem_list = (mem_list*)malloc(sizeof(mem_list));
            if (_mem_list == nullptr) {
                free(p_buff);
                return nullptr;
            }
            _mem_list->Init();
            mem_block* p_mem_used = (mem_block*)malloc(sizeof(mem_block));
            if (p_mem_used == nullptr) {
                free(p_buff);
                return nullptr;
            }
            p_mem_used->Init();
            p_mem_used->_brick = p_buff;
            _mem_list->_size = (int)buff_size;
            _mem_list->_mem_used = p_mem_used;
            _mem_list->_mem_free_last = p_mem_used;
            _mem_list_last = _mem_list;

            return SetMemoryHead(p_buff, _mem_list, p_mem_used);
        }
        mem_list* p_curr_list = _mem_list;
        while (p_curr_list != nullptr) {
            if (p_curr_list->_size == (int)buff_size) {
                mem_block* mem_free = p_curr_list->_mem_free;
                if (mem_free == nullptr) {
                    p_buff = malloc(buff_size + MAX_MEMORYHEAD_SIZE);
                    if (p_buff == nullptr) return nullptr;
                    mem_block* p_mem_used = (mem_block*)malloc(sizeof(mem_block));
                    if (p_mem_used == nullptr) {
                        free(p_buff);
                        return nullptr;
                    }
                    p_mem_used->Init();
                    p_mem_used->_brick = p_buff;
                    mem_block* p_mem_used_last = _mem_list->_mem_free_last;
                    if (p_mem_used_last == nullptr) {
                        p_curr_list->_size = (int)buff_size;
                        p_curr_list->_mem_used = p_mem_used;
                        p_curr_list->_mem_used_last = p_mem_used;
                        return SetMemoryHead(p_buff, p_curr_list, p_mem_used);
                    } else {
                        p_mem_used->_prev = p_curr_list->_mem_used_last;
                        p_curr_list->_mem_used_last->_next = p_mem_used;
                        p_curr_list->_mem_used_last = p_mem_used;
                        return SetMemoryHead(p_buff, p_curr_list, p_mem_used);
                    }
                } else {
                    mem_block* p_mem_tmp = mem_free;
                    p_curr_list->_mem_free = mem_free->_next;
                    p_buff = p_mem_tmp->_brick;

                    p_mem_tmp->_prev = p_curr_list->_mem_used_last;
                    mem_free->_next = nullptr;

                    if (p_curr_list->_mem_used_last == nullptr) {
                        p_curr_list->_mem_used_last = p_mem_tmp;
                        p_curr_list->_mem_used = p_mem_tmp;
                    } else {
                        p_curr_list->_mem_used_last->_next = p_mem_tmp;
                        p_curr_list->_mem_used_last = p_mem_tmp;
                    }
                    return GetMemoryHead(p_buff);
                }
            } else {
                p_curr_list = p_curr_list->_mem_list_next;
            }
        }
        
        p_buff = malloc(buff_size + MAX_MEMORYHEAD_SIZE);
        if (p_buff == nullptr) return nullptr;
        mem_list* p_mem_list = (mem_list*)malloc(sizeof(mem_list));
        if (p_mem_list == nullptr) {
            free(p_buff);
            return nullptr;
        }
        p_mem_list->Init();
        mem_block* p_mem_used = (mem_block*)malloc(sizeof(mem_block));
        if (p_mem_used == nullptr) {
            free(p_buff);
            return nullptr;
        }
        p_mem_used->Init();
        p_mem_used->_brick = p_buff;
        p_mem_list->_size = (int)buff_size;
        p_mem_list->_mem_used = p_mem_used;

        _mem_list_last->_mem_list_next = p_mem_list;
        _mem_list_last = p_mem_list;

        return SetMemoryHead(p_buff, p_mem_list, p_mem_used);
    }
    bool DelBuff(size_t buff_size, void* buff) {
        mem_list* p_curr_list = _mem_list;
        while (p_curr_list != nullptr) {
            if (p_curr_list->_size == (int)buff_size) {
                mem_block* p_mem_used = p_curr_list->_mem_used;
                bool first = true;
                while (p_mem_used != nullptr) {
                    if (buff == p_mem_used->_brick) {
                        if (p_mem_used->_brick != nullptr) {
                            if (first == true) {
                                if (p_mem_used->_next != nullptr) {
                                    p_mem_used->_next->_prev = p_mem_used->_prev;
                                }
                                p_curr_list->_mem_used = p_mem_used->_next;
                                if (p_mem_used == p_curr_list->_mem_used_last) {
                                    p_curr_list->_mem_used_last = p_curr_list->_mem_used_last->_prev;
                                }
                                first = false;
                            } else {
                                if (p_mem_used->_next != nullptr) {
                                    p_mem_used->_next->_prev = p_mem_used->_prev;
                                }
                                if (p_mem_used == p_curr_list->_mem_used_last) {
                                    p_curr_list->_mem_used_last = p_curr_list->_mem_used_last->_prev;
                                } else {
                                    p_mem_used->_prev->_next = p_mem_used->_next;
                                }
                            }
                            if (p_curr_list->_mem_free == nullptr) {
                                p_mem_used->_prev = nullptr;
                                p_mem_used->_next = nullptr;
                                p_curr_list->_mem_free = p_mem_used;
                                p_curr_list->_mem_free_last = p_mem_used;
                            } else {
                                p_mem_used->_prev = p_curr_list->_mem_free_last;
                                p_mem_used->_next = nullptr;
                                p_curr_list->_mem_free_last->_next = p_mem_used;
                                p_curr_list->_mem_free_last = p_mem_used;
                            }
                            return true;
                        } else {
                            return false;
                        }
                    }
                    p_mem_used = p_mem_used->_next;
                    first = false;
                }
            }
            p_curr_list = p_curr_list->_mem_list_next;
        }
        return false;
    }
    bool DelBuff(void* buff) {
        mem_block* p_mem_used = nullptr;
        mem_list* p_curr_list = nullptr;
        if (GetHeadMemoryBlock(buff, p_curr_list, p_mem_used) == false) return false;
        if (p_mem_used != nullptr && p_curr_list != nullptr) {
            if (p_curr_list->_mem_used == p_mem_used) {
                p_curr_list->_mem_used = p_mem_used->_next;
            } else {
                p_mem_used->_prev->_next = p_mem_used->_next;
            }
            if (p_mem_used->_next != nullptr) {
                p_mem_used->_next->_prev = p_mem_used->_prev;
            }
            if (p_mem_used == p_curr_list->_mem_used_last) {
                p_curr_list->_mem_used_last = p_curr_list->_mem_used_last->_prev;
            }
            if (p_curr_list->_mem_free == nullptr) {
                p_mem_used->_prev = nullptr;
                p_mem_used->_next = nullptr;
                p_curr_list->_mem_free = p_mem_used;
                p_curr_list->_mem_free_last = p_mem_used;
            } else {
                p_mem_used->_prev = p_curr_list->_mem_free_last;
                p_mem_used->_next = nullptr;
                p_curr_list->_mem_free_last->_next = p_mem_used;
                p_curr_list->_mem_free_last = p_mem_used;
            }
            return true;
        } else {
            return false;
        }
        return false;
    }
    void DisplayMemoryList();

private:
    mem_pool() {
        Init();
    }

    void Close() {
        mem_list* p_curr_list = _mem_list;
        while (p_curr_list != nullptr) {
            mem_block* p_mem_used = p_curr_list->_mem_used;
            while (p_mem_used != nullptr) {
                if (p_mem_used->_brick != nullptr) {
                    free(p_mem_used->_brick);
                    p_mem_used->_brick = nullptr;
                }
                p_mem_used = p_mem_used->_next;
            }
            mem_block* p_mem_free = p_curr_list->_mem_free;
            while (p_mem_free != nullptr) {
                if (p_mem_free->_brick != nullptr) {
                    free(p_mem_free->_brick);
                    p_mem_free->_brick = nullptr;
                }
                p_mem_free = p_mem_free->_next;
            }
            p_curr_list = p_curr_list->_mem_list_next;
        }
    }

    void Init() {
        _mem_list = nullptr;
        _mem_list_last = nullptr;
    }

    void* SetMemoryHead(void* buff, mem_list* list, mem_block* block) {
        if (buff == nullptr) return nullptr;
        unsigned int* _data = (unsigned int*)buff;
        _data[0] = (unsigned int)list;
        _data[1] = (unsigned int)block;
        _data[2] = (unsigned int)MAGIC_CODE;
        return &_data[3];
    }

    void* GetMemoryHead(void* buff) {
        if (buff == nullptr) return nullptr;
        long* _data = (long*)buff;
        return &_data[3];
    }

    bool GetHeadMemoryBlock(void* buff, mem_list*& list, mem_block*& block) {
        char* c_buff = (char*)buff;
        unsigned int* _data = (unsigned int)(c_buff - MAX_MEMORYHEAD_SIZE);
        if (_data[2] != (long)MAGIC_CODE) return false;
        else {
            list = (mem_list*)_data[0];
            block = (mem_block*)_data[1];
            return true;
        }
    }

private:
    static mem_pool* _mem_pool;
    mem_list*        _mem_list;
    mem_list*        _mem_list_last;
};

mem_pool* mem_pool::_mem_pool = nullptr;

inline void* operator new (size_t buff_size) {
    void* buff = mem_pool::Instance().GetBuff(buff_size);
    return buff;
}

inline void operator delete (void* p) {
    mem_pool::Instance().DelBuff(p);
}

inline void operator delete[] (void* p) {
    mem_pool::Instance().DelBuff(p);
}