#include "HashTable.h"
#include <assert.h>
#include <stdio.h>


/** Helper function to print test results */
void print_test_result(const char *test_name, bool result) {
    printf("%s: %s\n", test_name, result ? "PASSED" : "FAILED");
}

/** Test function for HTCreate and HTDestroy */
void test_create_and_destroy() {
    printf("Running test_create_and_destroy...\n");
    HashTable table = HTCreate();
    assert(table != NULL);

    HTDestroy(table);
    printf("test_create_and_destroy: PASSED\n");
}

/** Test function for HTPut and HTGet */
void test_put_and_get() {
    printf("Running test_put_and_get...\n");
    HashTable table = HTCreate();

    assert(HTPut(table, "key1", 10) == true);
    assert(HTGet(table, "key1") == 10);
    assert(HTPut(table, "key2", 20) == true);
    assert(HTGet(table, "key2") == 20);
    assert(HTPut(table, "key1", 5) == true); // Update existing key
    assert(HTGet(table, "key1") == 5);

    // Test inserting a key that is too long
    assert(HTPut(table, "30characterword-30characterword", 5) == false);

    print_test_result("test_put_and_get",
                      (HTGet(table, "key1") == 5) && (HTGet(table, "key2") == 20));

    HTDestroy(table);
}

/** Test function for HTDel */
void test_delete() {
    printf("Running test_delete...\n");
    HashTable table = HTCreate();

    HTPut(table, "key1", 10);
    HTPut(table, "key2", 20);
    assert(HTDel(table, "key1") == true);
    assert(HTGet(table, "key1") == -1); // Key should not exist
    assert(HTGet(table, "key2") == 20); // Other keys should be unaffected

    print_test_result("test_delete",
                      HTGet(table, "key1") == -1 && HTGet(table, "key2") == 20);

    HTDestroy(table);
}

/** Test function for HTSize */
void test_size() {
    printf("Running test_size...\n");
    HashTable table = HTCreate();

    HTPut(table, "key1", 10);
    HTPut(table, "key2", 20);
    HTPut(table, "key3", 30);
    assert(HTSize(table) == 3);

    HTDel(table, "key2");
    assert(HTSize(table) == 2);

    print_test_result("test_size", HTSize(table) == 2);

    HTDestroy(table);
}

/** Test function for HTContains */
void test_contains() {
    printf("Running test_contains...\n");
    HashTable table = HTCreate();

    HTPut(table, "key1", 10);
    HTPut(table, "key2", 20);
    assert(HTContains(table, "key1") == true);
    assert(HTContains(table, "key3") == false);

    print_test_result("test_contains", HTContains(table, "key1") == true &&
                                       HTContains(table, "key3") == false);

    HTDestroy(table);
}

/** Test function for cleanup with 20 deletions */
void test_cleanup_extended() {
    printf("Running test_cleanup_extended...\n");
    HashTable table = HTCreate();

    char key[10];
    for (int i = 0; i < 30; i++) {
        sprintf(key, "key%d", i);
        assert(HTPut(table, key, i * 10) == true);
    }

    // Deleting 20 entries
    for (int i = 0; i < 20; i++) {
        sprintf(key, "key%d", i);
        assert(HTDel(table, key) == true);
    }

    // Checking if the deleted entries are no longer occupied
    for (int i = 0; i < 20; i++) {
        sprintf(key, "key%d", i);
        assert(HTContains(table, key) == false);
    }

    // Checking if the remaining entries are still occupied
    for (int i = 20; i < 30; i++) {
        sprintf(key, "key%d", i);
        assert(HTContains(table, key) == true);
    }

    // Validate cleanup
    for (int i = 0; i < 20; i++) {
        sprintf(key, "key%d", i);
        assert(HTContains(table, key) == false);
    }

    print_test_result("test_cleanup_extended", true);

    HTDestroy(table);
}

/** Test function for HTIncrement */
void test_increment() {
    printf("Running test_HTincrement...\n");

    HashTable table = HTCreate();
    if (table == NULL) {
        printf("HashTable creation failed!\n");
        return;
    }
    HTPut(table, "key1", 10);
    HTPut(table, "key2", 20);
    HTPut(table, "key3", 30);

    // Incrementing values
    HTIncrement(table, "key1");
    HTIncrement(table, "key2");
    HTIncrement(table, "key3");
    HTIncrement(table, "key3");

    // Check if the values have been incremented correctly
    bool test_passed = true;
    test_passed = test_passed && (HTGet(table, "key1") == 11);
    test_passed = test_passed && (HTGet(table, "key2") == 21);
    test_passed = test_passed && (HTGet(table, "key3") == 32);

    // Check increment for a non-existent key (should return false)
    test_passed = test_passed && (HTIncrement(table, "nonexistent") == false);

    print_test_result("test_HTincrement", test_passed);

    HTDestroy(table);
}

/* Run all tests */
int main() {
    test_create_and_destroy();
    test_put_and_get();
    test_delete();
    test_size();
    test_increment();
    test_contains();
    test_cleanup_extended();

    printf("All tests completed.\n");
    return 0;
}
