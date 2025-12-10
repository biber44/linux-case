# Task 1

## What is the code doing

The program runs two threads parallel.

Thread 1:

- Thread 1 is running two seconds per loop
- increments loop_counter1
- returns always false
- Stops itself when
  - 10 seconds elapse since the thread start
  - Thread 2 stops first

Thread 2:

- Thread 2 is running one second per loop
- Increments loop_counter2 while it's <5
- after 5 cycles returns true
- Stops both threads by setting running to false

## Issues

1. Result with GCC is: `C1: 0 C2: 6`. Result with CLANG is: `C1: 1 C2: 1`.
   Expected result should be: `C1: 5 C2: 5` or `C1: 3 C2: 5` dependent on `my_running`.
   It's not clear what the abort condition for the threads should be. Is it the result of Process() call or the timeout, or is
   it the "global" `my_running` variable which stops both threads? As a result there could be two outcomes.

   The main problem is the following function call of `StartThread`:

   ```c++
   void StartThread(std::thread& thread, std::atomic<bool>& running, const std::function<bool(void)>& Process, const std::chrono::seconds timeout) {
   thread = std::thread([&]() {
   ...
   }
   ```

   `thread = std::thread([&]()` creates a new thread object which should run multiple times the `Process()` function.
   Since all variables are captured per default via reference, `Process` and `running` will be become dangling references as soon as `StartThread()` returns. This will cause undefined behavior.

2. There is a danger, if the `Process()` is implemented in a bad way like

   ```c++
       StartThread(
        my_thread2,
        my_running,
        [&]() {
            while (1) {};
            return false;
        },
        10s);
   ```

   the thread runs eternally, and abort and timeout check will never be reached.

3. When the timeout should be a maximum of 10 seconds we need to change the abort condition from:

   ```c++
    if (aborted || duration > timeout)
   ```

   to

   ```c++
    if (aborted || duration >= timeout)
   ```
