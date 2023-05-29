# Goal
The goal of this project is to implement a small but fully functional kernel for a multithreaded operating system with time sharing. In the following text, this system will be referred to as the kernel.

Within this project, a subsystem of this kernel will be implemented - a thread management subsystem. This subsystem should provide the concept of threads, as well as services for creating and launching threads, the concept of semaphores and interrupt-related events, and support for time sharing.

The kernel is implemented in such a way that the user program (application) and the kernel itself share the same address space, meaning they represent a single program. Concurrent processes created within the application will actually be "lightweight" processes, i.e., threads. The application and the kernel should be compiled and linked into a single program, that is, they should be written as a "unified" source code.

# Detailes

The kernel and the user application are treated as a single program (.exe) obtained by compiling and linking the code written in the programming language in which they are implemented. They should be executed within the Windows operating system on a PC computer as a unified console application (.exe), or process. The Windows operating system will be referred to as the host system.

Both the kernel and the user application must terminate properly as console applications, provided there are no irregularities within the user application itself. This means that after all threads launched within the user application have finished, the entire program will terminate normally. The test cases are regular, so any irregular termination of the program indicates an irregularity in the kernel itself.

In the implementation of the kernel, host system services related to thread or process concepts, semaphores, interrupts, synchronization, or communication between threads or processes are not used. In other words, all the required concepts and functionalities are implemented completely independently and "from scratch".

In the implementation of the kernel, only standard (statically linked) C/C++ libraries that are fully portable and platform-independent (hardware and operating system) are used. The kernel and the user application, as a single program, are executable on practically any PC computer. The program is completely self-contained and independent of any other software component. It should be executed simply by running the .exe file from any medium.

# Relationship between the Kernel and the User Application

The kernel is implemented in C++, with optional use of assembly language for PC processors. The user application include test cases and is provided as a set of source files that need to be compiled and linked with the compiled kernel code and the provided "applicat.lib" library into a unified console program (.exe). The "applicat.lib" library contains modules that are provided as modules accessing the (imaginary, virtual) hardware.

The main program, i.e., the control flow of the user application, should be in the function:

```cpp
int userMain(int argc, char* argv[]);
```

The arguments and return value of this function serve the same purpose as the arguments of the `main()` function, which should be passed to this function. In other words, the command-line arguments of the host system should be passed to the `userMain()` function, and the result of the `userMain()` function should be returned to the host system as the result of the `main()` function.

# Thread Management Subsystem Introduction

This project include the implementation of:

- The concept of threads, along with operations for creating and launching threads.
- Context switching and preemption in the following ways:
  - Explicit request from the thread itself (synchronous, explicit preemption).
  - Due to an interrupt occurrence (asynchronous, implicit preemption).
  - Due to the expiration of the allocated time (asynchronous, implicit preemption), as support for time sharing.
- The concept of a standard semaphore.
- The concept of an event (actually a binary semaphore) on which only one thread can wait in a blocked state, and which

The provided text describes switching and thread synchronization mechanisms in a C++ environment. Here's a breakdown of the main concepts and tasks outlined in the text:

## Context Switching:
   - Explicit context switch: Implemented a function called `dispatch()` that performs explicit context switching when called.
   - Asynchronous context switch: Implemented a concept of events that trigger context switches when interrupts occur.
   - Synchronization primitive operation: Implemented context switching when operations on synchronization primitives (e.g., semaphores or events) occur.
   - Time-sliced context switch: Performed context switches when the time slice allocated to a thread expires.

## Classes

&nbsp; **Scheduler**:
   - The scheduling algorithm and ready queue management are provided externally. The `Scheduler` class, declared in `scheduler.h`, includes static methods `put()` and `get()` for managing the ready queue.

&nbsp; **Semaphore**:
   - Implemented a standard counting semaphore with the operations `wait()` and `signal()`. The `Semaphore` class, defined in `semaphor.h`, should support a time-limited waiting interval.

&nbsp; **Event**:
   - Implemented the concept of an event, which represents a binary semaphore that can block a thread and is triggered by interrupts.

&nbsp; **Event class**:
   - Implemented the `Event` class, defined in `event.h`, which encapsulates the event concept. It includes methods such as `wait()` and `signal()`, as well as a friend relationship with the `KernelEv` class.

&nbsp; **KernelEv**:
   - Implemented the `KernelEv` class as the kernel-level implementation of events.

