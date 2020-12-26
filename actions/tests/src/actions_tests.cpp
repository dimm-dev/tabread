#include <gtest/gtest.h>

#include "actions_tests.h"

class Action_add : public Test_action
{
    private:
        int _step;
    public:
        Action_add(int step = 0) : _step{step} {}
        void execute(int& d) { d += _step; }
};

TEST(Actions, action_changes)
{
	Action_add a { 5 };
    int x = 1;
    a.execute(x);
    ASSERT_EQ(x, 6) << "Action::exectute() doesn't modify input";
    a.execute(x);
    ASSERT_EQ(x, 11) << "Action::exectute() doesn't modify input after second call";
}

TEST(Actions, action_chain_changes)
{
    Test_action_chain chain
    {
        std::shared_ptr<Test_action>{ new Action_add {1} },
        std::shared_ptr<Test_action>{ new Action_add {2} }
    };
    int x = 0;
    chain.execute(x);
    ASSERT_EQ(x, 3) << "Action_chain::exectute() doesn't modify input";
    chain.execute(x);
    ASSERT_EQ(x, 6) << "Action_chain::exectute() doesn't modify input after second call";
}

TEST(Actions, action_chain_multiple)
{
    auto instance_to_copy = std::shared_ptr<Test_action>{ new Action_add {2} };
    Test_action_chain chain
    {
        std::shared_ptr<Test_action>{ new Action_add {1} },
        instance_to_copy,
        std::shared_ptr<Test_action>{ new Action_add {3} },
        instance_to_copy
    };
    int x = 0;
    chain.execute(x);
    ASSERT_EQ(x, 8) << "Action_chain::exectute() doesn't modify input";
    chain.execute(x);
    ASSERT_EQ(x, 16) << "Action_chain::exectute() doesn't modify input after second call";
}
