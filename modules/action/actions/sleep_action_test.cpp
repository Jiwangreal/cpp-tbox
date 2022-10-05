#include <gtest/gtest.h>
#include <tbox/event/loop.h>
#include <tbox/base/scope_exit.hpp>

#include "sleep_action.h"
#include "../executor.h"

namespace tbox {
namespace action {

TEST(SleepAction, Basic) {
  auto loop = event::Loop::New();
  SetScopeExitAction([loop] { delete loop; });

  Executor exec(*loop);
  SleepAction action(exec.context(), "", std::chrono::milliseconds(10));
  bool is_finished = false;
  action.setFinishCallback(
    [loop, &is_finished] (bool is_succ) {
      loop->exitLoop();
      is_finished = true;
    }
  );
  action.start();

  loop->runLoop();
  EXPECT_TRUE(is_finished);
}
}
}
