# P1: Process and Resource Management

This project develops a simple process and resource manager.

## Getting Started

### Usage

To run the project:
```bash
./PRManager --input <input.txt> --output <output.txt>
```

### Building from Source

If you prefer to build from source, follow these steps:

```bash
cmake . -B build
cmake --build build
```

## Command Reference

The following commands are used for input:

| Command | Description |
| ------------- | ------------- |
|in \<n> \<u0> \<u1> \<u2> \<u3> |Restore the system to its initial state, with n priority levels and resources 0-3 having \<u0>, \<u1>, \<u2>, \<u3> units respectively|
|id	|Equivalent to "in 3 1 1 2 3"| 
|cr \<p>	| Create a new process at priority level \<p>. \<p> can be 1 or 2.|
| de \<i> | Destroy the process identified by the PCB index \<i>, and all of its descendants |
| rq \<r> \<k>	| Request \<k> units of resource \<r>|
| rl \<r> \<k>	| Release \<k> units of resource <r>. <r> can be 0, 1, 2, or 3 | 
| to | Invoke the timeout function |

For each input command, output the index of the process running next, as a single integer separated by a space.
If an error occurs, output -1.
A new line starts for each in or id command.