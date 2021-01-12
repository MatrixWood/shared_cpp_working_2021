#ifndef __REFCOUNT_H__
#define __REFCOUNT_H__

template <class T>
void _swap(T& x, T& y) {
    T tmp = x;
    x = y;
    y = tmp;
}

class refcount {
public:
    refcount();
    virtual ~refcount();
    virtual long incref();
    virtual long decref();
private:
    mutable long m_refcount;
};

template <class T>
class smartptr {
public:
    smartptr() : m_px(0) {}

    smartptr(T* px, bool addref = false) : m_px(px) {
        if ( m_px && addref ) {
            m_px->incref();
        }
    }

    template <class U>
    smartptr(U* px, bool addref = false) : m_px(px) {
        if ( m_px && addref ) {
            m_px->incref();
        }
    }

    ~smartptr() {
        if ( m_px ) {
            m_px->decref();
            m_px = 0;
        }
    }

    smartptr(const smartptr& other) : m_px(other.m_px) {
        if ( m_px ) {
            m_px->incref();
        }
    }

    template <class U>
    smartptr(const smartptr<U>& other) : m_px(other.getptr()) {
        if ( m_px ) {
            m_px->incref();
        }
    }

    smartptr& operator = (const smartptr& other) {
        T* px = 0;
        if ( this != &other ) {
            px = m_px;
            m_px = other.m_px;
            if ( m_px ) {
                m_px->incref();
            }
            if ( px ) {
                px->decref();
            }
            px = 0;
        }
        return *this;
    }

    template <class U>
    smartptr& operator = (const smartptr<U>& other) {
        T* px = 0;
        if ( this != &other ) {
            px = m_px;
            m_px = other.m_px;
            if ( m_px ) {
                m_px->incref();
            }
            if ( px ) {
                px->decref();
            }
            px = 0;
        }
        return *this;
    }

    operator T* () {
        return m_px;
    }

    operator const T* () {
        return m_px;
    }

    typedef T* (smartptr::*safe_bool)();

    operator safe_bool() const {
        return m_px == 0 ? 0 : &smartptr::release;
    }

    bool operator == (const smartptr& other) const {
        return m_px == other.m_px;
    }

    bool operator != (const smartptr& other) const {
        return m_px != other.m_px;
    }

    bool operator == (const T* px) const {
        return m_px == px;
    }

    bool operator != (const T* px) const {
        return m_px != px;
    }

    bool operator < (const smartptr& other) const {
        return m_px < other.m_px;
    }

    void reset() {
        T* px = m_px;
        m_px = 0;
        if ( px ) {
            px->decref();
        }
    }

    void reset(T* px, bool addref = false) {
        T* pxt = m_px;
        m_px = px;
        if ( m_px && addref ) {
            m_px->incref();
        }
        if ( pxt ) {
            pxt->decref();
        }
        pxt = 0;
    }

    T* release() {
        T* px = m_px;
        m_px = 0;
        return px;
    }

    T* getptr(bool addref = false) const {
        if ( addref && m_px ) {
            m_px->incref();
        }
        return m_px;
    }

private:
    T* m_px;
};



#endif