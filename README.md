A stackful, asymmetric coroutine in C.

- Implemented the basic concurrent functions.
- Swap context by assembly.
- Using a ready list to swap context automatically.


这是一个有栈非对称协程的C语言实现版本。

- 实现了基础的协程功能，能够说明协程的基本原理。
- 通过汇编实现协程的上下文切换。
- 通过一个就绪队列，实现协程之间的自动切换。

Work on Ubuntu 20, gcc version 9.3.0.

项目详细介绍，请阅读[人人都可以写协程：一个基于汇编的协程实现](https://blog.csdn.net/m0_47696151/article/details/121297557).
