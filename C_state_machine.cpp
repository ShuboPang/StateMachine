// C_state_machine.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <functional>
#include <list>

typedef enum {
    state_1 = 1,
    state_2,
    state_3,
    state_4,
    state_5,
}State;

bool f11111(int, int, void* arg)
{
    printf("this is f11111\n");
    return true;
}
bool f22222(int, int, void* arg)
{
    printf("this is f222222\n");
    return true;
}
bool f33333(int, int, void* arg)
{
    printf("this is f3333\n");
    return true;
}

bool f44444(int, int, void* arg)
{
    printf("this is f44444\n");
    return true;
}


class StateMachine {
public:
    StateMachine() {
        cur = nullptr;
    }
    ~StateMachine() {
        if (cur != nullptr)
            delete  cur;
    }

    struct StateInfo{
        int last_state;                                     //< 上一个状态
        int cur_state;                                      //< 当前状态
        std::function<bool(int,int,void* arg)> exec;        //< 执行函数       返回值：是否切换到下一个状态    参数：上一状态  当前状态 任意参数(arg_)
        int next_state;                                     //< 下一个状态
        void* arg_;                                         //< 任意参数
    };

    typedef std::list<StateInfo> STATELIST;

    void RegisterState(const StateInfo& info) {
        auto it = state_list.begin();
        while (it != state_list.end())
        {
            StateInfo info_ = *it;
            if (info_.cur_state == info.cur_state && info_.last_state == info.last_state) {
                return;
            }
            ++it;
        }
        state_list.push_back(info);
    }

    /// <summary>
    /// 设置状态机初始状态 (默认上一个状态为0）
    /// </summary>
    /// <param name="state"> 初始状态  </param>
    /// <returns></returns>
    bool SetInitialState(int state) {
        return SwitchState(state, 0);
    }

    /// <summary>
    /// 获取状态机当前状态
    /// </summary>
    /// <returns></returns>
    StateInfo* CurrentState() {
        return cur;
    }

    /// <summary>
    /// 循环调用
    /// </summary>
    bool Exec() {
        if (cur == nullptr) {
            return true;
        }
        bool ret = cur->exec(cur->last_state, cur->cur_state, cur->arg_);
        if (ret) {
            SwitchState(cur->next_state, cur->cur_state);
        }
        return false;
    }

private:
    bool SwitchState(int state,int last_state) {
        if (cur != nullptr)
            delete  cur;
        cur = new StateInfo;
        auto it = state_list.begin();
        while (it != state_list.end())
        {
            StateInfo info_ = *it;
            if (info_.cur_state == state && info_.last_state == last_state) {
                cur->arg_ = info_.arg_;
                cur->cur_state = info_.cur_state;
                cur->exec = info_.exec;
                cur->last_state = info_.last_state;
                cur->next_state = info_.next_state;
                return true;
            }
            ++it;
        }
        delete cur;
        cur = nullptr;
        return false;
    }

private:
    STATELIST state_list;
    StateInfo* cur;

};

int main()
{
    StateMachine* state_machine = new StateMachine();
    state_machine->RegisterState({ 0,state_1 ,std::bind(f11111,std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3),state_2 });
    state_machine->RegisterState({ state_1,state_2 ,std::bind(f22222,std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3) ,state_3});
    state_machine->RegisterState({ state_2,state_3 ,std::bind(f33333,std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3) ,state_4 });
    state_machine->RegisterState({ state_3,state_4 ,std::bind(f44444,std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3)  ,state_5});

    state_machine->SetInitialState(state_1);
    do {
        state_machine->Exec();
    } while (state_machine->CurrentState());
    delete state_machine;
    return 0;
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
