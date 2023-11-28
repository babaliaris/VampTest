#include <VampTest/VampTest.h>

void func1(void *node, void *framework)
{
    printf("func1\n");
}

void func2(void *node, void *framework)
{
    printf("func2\n");
}

int main()
{
    VampTestFramework *framework = VampNewTestFramework();

    VampTestNode *test1 = VampNewTestNode("suite1", "test1");
    VampTestNode *test2 = VampNewTestNode("suite2", "test2");

    test1->m_func = func1;
    test2->m_func = func2;

    VampAppendTest(framework, test1);
    VampAppendTest(framework, test2);


    VampRunAllTests(framework);

    VampDestroyTestFramework(framework);

    return 0;
}