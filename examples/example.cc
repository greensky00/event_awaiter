#include "event_awaiter.h"
#include "test_common.h"

#include <stdio.h>

struct TestArgs : TestSuite::ThreadArgs {
    TestArgs() : awaiter(nullptr), expected_val(0), val_by_invoker(-1) {}
    EventAwaiter* awaiter;
    int expected_val;
    int val_by_invoker;
};

int awaiter_thread(TestSuite::ThreadArgs* args_base) {
    TestArgs* args = static_cast<TestArgs*>(args_base);

    args->awaiter->wait();
    CHK_EQ(args->expected_val, args->val_by_invoker);

    return 0;
}

int invoker_thread(TestSuite::ThreadArgs* args_base) {
    TestArgs* args = static_cast<TestArgs*>(args_base);

    args->val_by_invoker = args->expected_val;
    args->awaiter->invoke();

    return 0;
}

int basic_example(int n) {
    for (int ii=0; ii<n; ++ii) {
        TestArgs args;
        EventAwaiter ea;
        args.expected_val = ii;
        args.awaiter = &ea;

        TestSuite::ThreadHolder awaiter(&args, awaiter_thread, nullptr);
        TestSuite::ThreadHolder invoker(&args, invoker_thread, nullptr);

        awaiter.join();
        invoker.join();

        CHK_Z(awaiter.getResult());
        CHK_Z(invoker.getResult());
    }

    return 0;
}

int main(int argc, char** argv) {
    TestSuite test(argc, argv);

    test.doTest("basic example", basic_example,
                TestRange<int>({100}) );

    return 0;
}


