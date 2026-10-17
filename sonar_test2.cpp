/*
 * sonar_test2.cpp
 * A second set of intentionally flawed code (C++) for testing SonarQube Cloud.
 * Each function or class has ONE deliberate issue, labeled in a comment.
 * DO NOT use this code in a real project.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

/* ---------- VULNERABILITIES ---------- */

/* 1. Hardcoded secret / API key */
const std::string API_KEY = "sk_live_51H8abcDEFghiJKL123456789";

/* 2. Weak random number generator used for a security token */
int generate_token() {
    srand(time(nullptr));
    return rand();
}

/* 3. SQL injection: query built by concatenating user input */
std::string build_query(const std::string &username) {
    return "SELECT * FROM users WHERE name = '" + username + "'";
}

/* 4. Unsafe C string functions: strcat/sprintf with no size checks */
void make_greeting(const char *name) {
    char greeting[16];
    strcpy(greeting, "Hello, ");
    strcat(greeting, name);
    std::printf("%s\n", greeting);
}

/* 5. Path traversal: user-controlled filename opened directly */
void read_file(const char *filename) {
    char path[64];
    std::sprintf(path, "/var/data/%s", filename);
    FILE *f = std::fopen(path, "r");
    if (f) {
        std::fclose(f);
    }
}

/* ---------- BUGS ---------- */

/* 6. new[] released with delete (should be delete[]) */
void wrong_delete() {
    int *arr = new int[10];
    arr[0] = 1;
    delete arr;
}

/* 7. Base class without a virtual destructor, deleted through base pointer */
class Animal {
public:
    ~Animal() { std::cout << "Animal destroyed\n"; }
};

class Dog : public Animal {
    int *data = new int[100];
public:
    ~Dog() { delete[] data; }
};

void polymorphic_delete() {
    Animal *a = new Dog();
    delete a;
}

/* 8. Integer overflow */
int overflow() {
    int big = 2147483647;
    return big + 1;
}

/* 9. Memory leak: early return skips delete */
int early_return_leak(bool fail) {
    int *buf = new int[50];
    if (fail) {
        return -1;
    }
    delete[] buf;
    return 0;
}

/* 10. Returning a pointer to a local variable (dangling pointer) */
int *dangling() {
    int local = 5;
    return &local;
}

/* 11. Class member left uninitialized */
class Counter {
public:
    int value;
    Counter() {}
    int next() { return ++value; }
};

/* 12. switch without break (unintended fall-through) */
const char *describe(int code) {
    const char *msg = "unknown";
    switch (code) {
        case 1:
            msg = "one";
        case 2:
            msg = "two";
            break;
        default:
            break;
    }
    return msg;
}

/* ---------- CODE SMELLS ---------- */

/* 13. Condition that is always true */
bool always_true(unsigned int x) {
    return x >= 0;
}

/* 14. Self-assignment and a useless statement */
void self_assign() {
    int a = 5;
    a = a;
    a == 6;
}

/* 15. Empty catch block that swallows the error */
void swallow() {
    try {
        throw std::runtime_error("boom");
    } catch (...) {
    }
}

/* 16. Function with too many parameters */
int many_params(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + b + c + d + e + f + g + h;
}

int main() {
    make_greeting("ThisNameIsWayTooLong");
    wrong_delete();
    polymorphic_delete();
    return 0;
}
