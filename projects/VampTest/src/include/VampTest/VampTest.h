#ifndef VAMP_TEST_H
#define VAMP_TEST_H
#include <stdlib.h>
#include <stdio.h>


#define VAMP_CLI_COLOR_RESET    "\033[0;0m"
#define VAMP_CLI_COLOR_RED      "\033[0;31m"
#define VAMP_CLI_COLOR_GREEN    "\033[0;32m"
#define VAMP_CLI_COLOR_YELLOW   "\033[0;33m"
#define VAMP_CLI_COLOR_BLUE     "\033[0;34m"
#define VAMP_CLI_COLOR_PINK     "\033[0;35m"
#define VAMP_CLI_COLOR_CYAN     "\033[0;36m"
#define VAMP_CLI_COLOR_WHITE    "\033[0;37m"


typedef void (*VampTestFunc)(void *__vamp_Node__, void *__vamp_Framework__);

typedef struct VampTestNode
{
    struct VampTestNode *m_next;
    struct VampTestNode *m_prev;

    char *m_suite_name;
    char *m_name;

    char m_failed;

    VampTestFunc m_func;
}
VampTestNode;

typedef struct VampTestFramework
{
    VampTestNode *m_head;
    VampTestNode *m_tail;

    unsigned int m_number_of_tests;
    unsigned int m_number_of_tests_run;
    unsigned int m_number_of_failures;

}
VampTestFramework;



VampTestNode *VampNewTestNode(const char *suite_name, const char *test_name);

VampTestFramework *VampNewTestFramework();

void VampDestroyTestFramework(VampTestFramework *test_framework);

void VampAppendTest(VampTestFramework *test_framework, VampTestNode *test_node);

void VampRunAllTests(VampTestFramework *test_framework, int argc, char **argv);


void VampRunBeforeAllTests();
void VampRunAfterAllTests();


#define VAMP_TEST(suite_name, test_name)\
    void suite_name##_##test_name(void *__vamp_Node__, void *__vamp_Framework__)

#define VAMP_DECLARE_TEST(suite_name, test_name)\
    void suite_name##_##test_name(void *__vamp_Node__, void *__vamp_Framework__)

#define VAMP_REGISTER_TEST(suite_name, test_name)\
    VampTestNode *new_test_##suite_name##_##test_name = VampNewTestNode(#suite_name, #test_name);\
    new_test_##suite_name##_##test_name->m_func = suite_name##_##test_name;\
    VampAppendTest(__Vamp_Actual_Test_Framework__, new_test_##suite_name##_##test_name)


#define VAMP_TEST_FRAMEWORK(registers)\
    int main(int argc, char **argv)\
    {\
        VampTestFramework *__Vamp_Actual_Test_Framework__ = VampNewTestFramework();\
        registers\
        VampRunBeforeAllTests();\
        VampRunAllTests(__Vamp_Actual_Test_Framework__, argc, argv);\
        VampRunAfterAllTests();\
        VampDestroyTestFramework(__Vamp_Actual_Test_Framework__);\
        return 0;\
    }


#define VAMP_EXPECT(cond, ...)\
    if ( !(cond) )\
    {\
        VampTestNode *__vamp_Node_Casted__ = (VampTestNode *)__vamp_Node__;\
        VampTestFramework *__vamp_Framework_Casted__ = (VampTestFramework *)__vamp_Framework__;\
        __vamp_Framework_Casted__->m_number_of_failures++;\
        __vamp_Node_Casted__->m_failed = 1;\
        printf("%sTEST: [%s.%s] ------------------------------------------------------------------------------> [FAILED]\n", \
            VAMP_CLI_COLOR_RED, __vamp_Node_Casted__->m_suite_name, __vamp_Node_Casted__->m_name);\
        printf("@   : [%s:%d]\n", __FILE__, __LINE__);\
        printf("%s\tExpected: %s\n", VAMP_CLI_COLOR_YELLOW, #cond);\
        printf("%s\tMessage : ", VAMP_CLI_COLOR_CYAN);\
        printf(__VA_ARGS__);\
        printf("%s\n\n", VAMP_CLI_COLOR_RESET);\
    }





#endif