# OS---Assignment

**Implementation Details:**

- Implemented `enqueue()` and `dequeue()` functions in `queue.c`.
- Implemented `get_proc()` and `get_mlq_proc()` functions in `sched.c`.
- Ensured all functionalities meet the requirements specified in section 2.1.
  
**Output Description:**

The scheduler component outputs include:

- Enqueueing a new PCB into the priority queue upon program initialization.
- Dequeueing the next "in turn" PCB from the priority queue.
- Retrieving the PCB of a waiting process from the ready queue system.
- Retrieving the PCB of a waiting process adhering to the MLQ policy.

**Functionalities:**

- Loader creates a new process for each new program.
- Content of the program is copied to the text segment of the new process.
- PCB of the process is pushed to the associated ready queue.
- CPU executes processes in round-robin style.
- CPU enqueues the process back to its associated priority ready queue after execution.
- CPU continues executing processes from ready_queue.

#### 4.2.1.2. Paging-based Memory Management

**Implementation Details:**

- Implemented files: `mm.c`, `mm-vm.c`, and `mm-memphy.c`.
- Illustrates theories learned in academic lessons.
  
**Output Description:**

The outputs of memory management are non-deterministic due to different CPU scheduling. However, they generally match the design and are reasonable.

### 4.2.2. Achievements and Practice Influence

#### 4.2.2.1. Achievements

**Key Achievements:**

- Comprehensive understanding of Multilevel Queue (MLQ) scheduling.
- Efficient process management on multiple processors.
- Round-robin style execution for fair CPU allocation.
- Solid understanding of virtual memory mapping.
- Efficient memory utilization and addressing scheme implementation.

#### 4.2.2.2. Practice Influence

**Influence on Practice:**

- Contribution to OS advancement through practical implementation.
- Real-world applicability in OS development and optimization.
- Insight into efficient process management and memory utilization.
- Practical problem-solving skills enhancement.

#### 4.2.2.3. Conclusion

**Conclusion:**

The project's focus on scheduling and memory management has led to significant achievements and positive impacts on operating systems understanding and practice. It deepened the understanding of scheduling algorithms and memory management techniques, paving the way for future research and advancements.
