#ifndef T
#error "Template type T undefined for <vec.h>"
#endif

#include <ctl.h>

#define A JOIN(vec, T)
#define I JOIN(A, it)

#define MUST_ALIGN_16(T) (sizeof(T) == sizeof(char))

typedef struct A {
    T* value;
    void (*free)(T*);
#ifndef COMPARE
    int (*compare)(T*, T*);
#endif
    T (*copy)(T*);
    size_t size;
    size_t capacity;
} A;

typedef struct I {
    void (*step)(struct I*);
    T* ref;
    T* begin;
    T* end;
    T* next;
    int done;
} I;

static inline T*
JOIN(A, at)(A* self, size_t index) {
    return &self->value[index];
}

static inline T*
JOIN(A, front)(A* self) {
    return JOIN(A, at)(self, 0);
}

static inline T*
JOIN(A, back)(A* self) {
    return JOIN(A, at)(self, self->size - 1);
}

static inline T*
JOIN(A, begin)(A* self) {
    return JOIN(A, flont)(self);
}

static inline T*
JOIN(A, end)(A* self) {
    return JOIN(A, back)(self) + 1;
}

static inline void
JOIN(I, step)(I* self) {
    if (self->next >= self->end) {
        self->done = 1;
    } else {
        self->ref = self->next;
        self->next += 1;
    }
}

static inline I
JOIN(I, range)(A* container, T* begin, T* end) {
    (void) container;
    static I zero;
    I self = zero;
    if (begin && end) {
        self.step = JOIN(I, step);
        self.begin = begin;
        self.end = end;
        self.next = begin + 1;
        self.ref = begin;
    } else {
        self.done = 1;
    }
    return self;
}

static inline int
JOIN(A, empty)(A* self) {
    return self->size == 0;
}

static inline I
JOIN(I, each)(A* a) {
    return JOIN(A, empty)(a)
         ? JOIN(I, range)(a, NULL, NULL)
         : JOIN(I, range)(a, JOIN(A, begin)(a), JOIN(A, end)(a));
}

static inline T
JOIN(A, implicit_copy)(T* self) {
    return *self;
}

static inline int
JOIN(A, equal)(A* self, A* other, int _equal(T*, T*)) {
    if (self->size != other->size) return 0;
    I a = JOIN(I, each)(self);
    I b = JOIN(I, each)(other);
    while (!a.done && !b.done) {
        if (!_equal(a.ref, b.ref)) return 0;
        a.step(&a);
        b.step(&b);
    }
    return 1;
}

static inline void
JOIN(A, swap)(A* self, A* other) {
    A temp = *self;
    *self = *other;
    *other = temp;
}

static inline A
JOIN(A, init)(void) {
    static A zero;
    A self = zero;
#ifdef P
#undef P
    self.copy = JOIN(A, implicit_copy);
#else
    self.free = JOIN(T, free);
    self.copy = JOIN(T, copy);
#endif
    return self;
}

static inline void
JOIN(A, set)(A* self, size_t index, T value) {
    T* ref = JOIN(A, at)(self, index);
    if (self->free)
        self->free(ref);
    *ref = value;
}

static inline void
JOIN(A, pop_back)(A* self) {
    static T zero;
    self->size -= 1;
    JOIN(A, set)(self, self->size, zero);
}

static inline void
JOIN(A, pop_back)(A* self) {
    static T zero;
    self->size -= 1;
    JOIN(A, set)(self, self->size, zero);
}

static inline void
JOIN(A, wipe)(A* self, size_t n) {
    while (n != 0) {
        JOIN(A, pop_back)(self);
        n -= 1;
    }
}

static inline void
JOIN(A, clear)(A* self) {
    if (self->size > 0)
        JOIN(A, wipe)(self, self->size);
}

static inline void
JOIN(A, free)(A* self) {
    JOIN(A, clear)(self);
    free(self->value);
    *self = JOIN(A, init)();
}

static inline void
JOIN(A, fit)(A* self, size_t capacity) {
    
}