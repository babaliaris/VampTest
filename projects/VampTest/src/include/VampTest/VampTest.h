#ifndef VAMP_TEST_H
#define VAMP_TEST_H
#include <stdlib.h>
#include <stdio.h>


typedef void (*VampTestFunc)(void *testnode, void *framework);

typedef struct VampTestNode
{
    struct VampTestNode *m_next;
    struct VampTestNode *m_prev;

    char *m_suite_name;
    char *m_name;

    VampTestFunc m_func;
}
VampTestNode;

typedef struct VampTestFramework
{
    VampTestNode *m_head;
    VampTestNode *m_tail;

    unsigned int m_number_of_tests;
    unsigned int m_number_of_successes;
    unsigned int m_number_of_failures;

}
VampTestFramework;



VampTestNode *VampNewTestNode(const char *suite_name, const char *test_name);

VampTestFramework *VampNewTestFramework();

void VampDestroyTestFramework(VampTestFramework *test_framework);

void VampAppendTest(VampTestFramework *test_framework, VampTestNode *test_node);

void VampRunAllTests(VampTestFramework *test_framework);





#endif