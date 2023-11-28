#include <VampTest/VampTest.h>
#include <string.h>
#include <assert.h>


#define __VAMP_PRINT_TEST_PASSED_MSG__\
    printf("%sTEST: [%s.%s] %50s\n",\
            VAMP_CLI_COLOR_GREEN, current->m_suite_name, current->m_name, "[PASS]");\
    printf(VAMP_CLI_COLOR_RESET);


VampTestNode *VampNewTestNode(const char *suite_name, const char *test_name)
{
    VampTestNode *new_node = (VampTestNode *)malloc(sizeof(VampTestNode));

    new_node->m_next        = NULL;
    new_node->m_prev        = NULL;
    new_node->m_suite_name  = NULL;
    new_node->m_name        = NULL;
    new_node->m_failed      = 0;
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
    new_framework->m_number_of_tests_run = 0;
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


void VampRunAllTests(VampTestFramework *test_framework, int argc, char **argv)
{
    //-------------------Get the suite name and the test name from the --filter option-------------------//
    char *suite_name  = NULL;
    char *test_name   = NULL;

    //If the filter option is specified.
    if (argc == 3 && strcmp(argv[1], "--filter") == 0)
    {
        //Break the filter string using the dot delimiter.
        char *token = strtok(argv[2], ".");
        
        //Assign the suite name and the test name.
        unsigned int i = 0;
        while( token != NULL )
        {   
            if (i == 0) suite_name = token;
            if (i == 1) test_name  = token;
            token = strtok(NULL, ".");
            i++;
        }

        //At this point there should be 1 or two tokens that have been
        //seperated successfully using the dot delimiter. If not
        //then the --filter value was incorrect.
        if (i > 2)
        {
            printf("%sWarning: The value of --filter='%s' is not correct. It should be in the form of 'SuiteName.TestName' or just 'SuiteName.'%s\n\n",
            VAMP_CLI_COLOR_YELLOW, argv[2], VAMP_CLI_COLOR_RESET);

            //Reset the suite name and test name to NULL.
            suite_name = NULL;
            test_name = NULL;
        }
    }


    //The value of --filter is not correct.
    else if (argc == 2 && strcmp(argv[1], "--filter") == 0)
    {
        printf("%sWarning: You specified --filter without a value.%s\n\n",
            VAMP_CLI_COLOR_YELLOW, VAMP_CLI_COLOR_RESET);
    }


    //Uknown arguments.
    else if (argc > 3)
    {
        printf("%sWarning: The arguments that you specified are not supported.\n%s\n\n",
            VAMP_CLI_COLOR_YELLOW, VAMP_CLI_COLOR_RESET);
    }
    //-------------------Get the suite name and the test name from the --filter option-------------------//





    //-------------------------------------------Run The Tests-------------------------------------------//
    VampTestNode *current = test_framework->m_head->m_next;

    //TODO Use filter to run a specific single test or a test suite!!!
    //Run all the tests.
    while (current && current->m_next != NULL)
    {

        //Run All Tests.
        if (!suite_name && !test_name)
        {
            current->m_func(current, test_framework);
        
            //Test Passed.
            if (!current->m_failed)
            {
                __VAMP_PRINT_TEST_PASSED_MSG__
            }

            //Increase the number of tests that have been run.
            test_framework->m_number_of_tests_run++;
        }

        //Run all the tests in a suite.
        else if (suite_name && !test_name)
        {
            if (strcmp(current->m_suite_name, suite_name) == 0)
            {
                current->m_func(current, test_framework);
        
                //Test Passed.
                if (!current->m_failed)
                {
                    __VAMP_PRINT_TEST_PASSED_MSG__
                }

                //Increase the number of tests that have been run.
                test_framework->m_number_of_tests_run++;
            }
        }

        //Run only one specified test from a specified suite.
        else if (suite_name && test_name)
        {
            if (strcmp(current->m_suite_name, suite_name) == 0 && strcmp(current->m_name, test_name) == 0)
            {
                current->m_func(current, test_framework);
        
                //Test Passed.
                if (!current->m_failed)
                {
                    __VAMP_PRINT_TEST_PASSED_MSG__
                }

                //Increase the number of tests that have been run.
                test_framework->m_number_of_tests_run++;
            }
        }


        //Go to the next test.
        current = current->m_next;
    }

    //-------------------------------------------Run The Tests-------------------------------------------//

    //If there are tests but none has run, the --filter value was incorrect.
    if (test_framework->m_number_of_tests_run == 0 && test_framework->m_number_of_tests > 0)
    {
        if (suite_name && test_name)
        {
            printf("%sWarning: Test --filter='%s.%s', was not found. Nothing will run.\n%s\n\n",
                VAMP_CLI_COLOR_YELLOW, suite_name, test_name,VAMP_CLI_COLOR_RESET);
        }

        else if (suite_name)
        {
            printf("%sWarning: Test --filter='%s', was not found. Nothing will run.\n%s\n\n",
                VAMP_CLI_COLOR_YELLOW, suite_name,VAMP_CLI_COLOR_RESET);
        }
    }

    //Print Conclusion.
    printf("\n\n");
    printf("%sTOTAL TESTS REGISTERED : %d\n", VAMP_CLI_COLOR_PINK,test_framework->m_number_of_tests);
    printf("%sTOTAL TESTS RUN        : %d\n", VAMP_CLI_COLOR_WHITE,test_framework->m_number_of_tests_run);
    printf("%sTOTAL FAILED           : %d\n", VAMP_CLI_COLOR_RED,test_framework->m_number_of_failures);
    printf("%sTOTAL PASSED           : %d\n", VAMP_CLI_COLOR_GREEN, test_framework->m_number_of_tests_run - test_framework->m_number_of_failures);
    printf("%s\n", VAMP_CLI_COLOR_RESET);


}