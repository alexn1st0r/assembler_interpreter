# Assembler interpreter

We want to create an interpreter of assembler which will support the following instructions:

1. ```mov x, y``` - copy y (either an integer or the value of a register) into register x.
2. ```inc x``` - increase the content of register x by one.
3. ```dec x``` - decrease the content of register x by one.
4. ```add x, y``` - add the content of the register x with y either an integer or the value of a register and stores the result in x i.e. register[x] += y.
5. ```sub x, y`` - subtract y either an integer or the value of a register from the register x and stores the result in x i.e. register[x] -= y.
6. ```mul x, y``` - same with multiply i.e. register[x] *= y.
7. ```div x, y``` - same with integer division i.e. register[x] /= y.
8. ```label:``` - define a label position (label = identifier + ":", an identifier being a string that does not match any other command). Jump commands and call are aimed to these labels positions in the program.
9. ```jmp lbl``` - jumps to the label lbl.
10. ```cmp x, y``` - compares x (either an integer or the value of a register) and y (either an integer or the value of a register). The result is used in the conditional jumps (jne, je, jge, jg, jle and jl)
11. ```jne lbl``` - jump to the label lbl if the values of the previous cmp command were not equal.
12. ```je lbl``` - jump to the label lbl if the values of the previous cmp command were equal.
13. ```jge lbl``` - jump to the label lbl if x was greater or equal than y in the previous cmp command.
14. ```jg lbl``` - jump to the label lbl if x was greater than y in the previous cmp command.
15. ```jle lbl``` - jump to the label lbl if x was less or equal than y in the previous cmp command.
16. ```jl lbl``` - jump to the label lbl if x was less than y in the previous cmp command.
17. ```call lbl``` - call to the subroutine identified by lbl. When a ret is found in a subroutine, the instruction pointer should return to the instruction next to this call command.
18. ```ret``` - when a ret is found in a subroutine, the instruction pointer should return to the instruction that called the current function.
19. ```msg``` 'Register: ', x - this instruction stores the output of the program. It may contain text strings (delimited by single quotes) and registers. The number of arguments isn't limited and will vary, depending on the program.
20. ```end``` - this instruction indicates that the program ends correctly, so the stored output is returned (if the program terminates without this instruction it should return the default output: see below).
21. ```; comment``` - comments should not be taken in consideration during the execution of the program.

## Output format:

The normal output format is a string (returned with the end command).
If the program does finish itself without using an ```end``` instruction, the default return value is:
```"-1"``` (as a string)

## Input format:

The function/method will take as input a multiline string of instructions, delimited with EOL characters.

For example:
```
program = R"(
; My first program
mov  a, 5
inc  a
call function
msg  '(5+1)/2 = ', a    ; output message
end

function:
    div  a, 2
    ret)";
assembler_interpreter(program);
```
The above code would set register a to 5, increase its value by 1, calls the subroutine function, divide its value by 2, returns to the first call instruction, prepares the output of the program and then returns it with the end instruction. In this case, the output would be (5+1)/2 = 3.

## Check it!


1. ``git submodule update --init --recursive``
2. ``sudo apt-get install build-essential cmake``
3. ``mkdir build``
4. ``cd build``
5. ``cmake .. -DBUILD_GMOCK=On``
6. ``make``
7. ``tests/assembler_interpreter``
