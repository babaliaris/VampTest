#include <VampTest/VampTest.h>
#include <string.h>
#include <assert.h>

VampTestNode *VampNewTestNode(const char *suite_name, const char *test_name)
{
    VampTestNode *new_node = (VampTestNode *)malloc(sizeof(VampTestNode));

    new_node->m_next        = NULL;
    new_node->m_prev        = NULL;
    new_node->m_suite_name  = NULL;
    new_node->m_name        = NULL;
    new_node->m_func        = NULL;

    //Both names must be specified for a test.
    if (suite_name && test_name)
    {
        size_t suite_name_length = strlen(suite_name);
        size_t name_length = strlen(test_name);

        new_node->m_suite_name  = (char *)malloc(sizeof(char) * (suite_name_length + 1));
        new_node->m_name        = (char *)malloc(sizeof(char) * (name_length + 1));

        strcpy(new_node->m_suite_name, suite_name);
        strcpy(new_node->m_name, test_name);
    }
   
   return new_node;
}

VampTestFramework *VampNewTestFramework()
{
    //Create the framework.
    VampTestFramework *new_framework = (VampTestFramework *)malloc(sizeof(VampTestFramework));

    //Create the head and the tail nodes. These are two different 
    //allocated nodes with no data. The actual elements will "live" between them.
    new_framework->m_head = VampNewTestNode(NULL, NULL);
    new_framework->m_tail = VampNewTestNode(NULL, NULL);

    //Initialize the rest.
    new_framework->m_number_of_tests = 0;
    new_framework->m_number_of_successes = 0;
    new_framework->m_number_of_failures = 0;

    return new_framework;
}


void VampDestroyTestFramework(VampTestFramework *test_framework)
{   
    //Nothing to do...
    if (!test_framework) return;

    VampTestNode *current   = test_framework->m_head->m_next;
    VampTestNode *temp      = current;

    while (current && current->m_next != NULL)
    {
        current = current->m_next;

        //Free the strings of suite_name & name.
        if (temp->m_name) free(temp->m_name);
        if (temp->m_suite_name) free(temp->m_suite_name);

        //Free the actual node.
        free(temp);

        //Update temp to current.
        temp = current;
    }

    //Free the head and the tail nodes.
    free(test_framework->m_head);
    free(test_framework->m_tail);

    //Free the framework itself.
    free(test_framework);
}


void VampAppendTest(VampTestFramework *test_framework, VampTestNode *test_node)
{
    //Nothing to do...
    if (!test_framework || !test_node) return;

    //If the list is empty.
    if (test_framework->m_head->m_next == NULL && test_framework->m_tail->m_prev == NULL)
    {
        test_framework->m_head->m_next = test_node;
        test_framework->m_tail->m_prev = test_node;

        test_node->m_prev = test_framework->m_head;
        test_node->m_next = test_framework->m_tail;
    }

    //The list is not Empty.
    else
    {
        test_node->m_next = test_framework->m_tail;
        test_node->m_prev = test_framework->m_tail->m_prev;

        test_node->m_prev->m_next = test_node;
        test_framework->m_tail->m_prev = test_node;
    }

    //Increase the number of tests.
    test_framework->m_number_of_tests++;
}


void VampRunAllTests(VampTestFramework *test_framework)
{
    VampTestNode *current = test_framework->m_head->m_next;

    while (current && current->m_next != NULL)
    {
        current->m_func(current, test_framework);
        current = current->m_next;
    }
}