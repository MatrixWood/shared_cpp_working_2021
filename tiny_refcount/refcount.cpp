#include "refcount.h"
#include <pthread.h>

refcount::refcount() : m_refcount(1)
{}

refcount::~refcount()
{}

long refcount::incref() {
    return ::__sync_fetch_and_add(&m_refcount, 1);
}

long refcount::decref() {
    long refcnt = ::__sync_fetch_and_sub(&m_refcount, 1);
    if ( refcnt == 0 ) {
        delete this;
    }
    return refcnt;
}