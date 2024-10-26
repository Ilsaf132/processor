#define PUSH_AND_POP 1
#define NO_ARG 0
#define JMP 2

DEF_CMD(PUSH, 1, PUSH_AND_POP, 
    {
        StackPush(&proc_t -> stk, GetArgPush(proc_t));
        break;
    })
DEF_CMD(POP, 13, PUSH_AND_POP, 
    {
        StackPop(&proc_t -> stk, &elem_pop);
        addr = GetArgPop(proc_t);
        *addr = elem_pop;
        break;
    })
DEF_CMD(ADD, 2, NO_ARG, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        StackPush(&proc_t -> stk, a + b); 
        proc_t -> ip += 1;
        break;
    })
DEF_CMD(SUB, 3, NO_ARG, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        StackPush(&proc_t -> stk, b - a);
        proc_t -> ip += 1;
        break;
    })
DEF_CMD(DIV, 4, NO_ARG, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        StackPush(&proc_t -> stk, (b*ACCURACY_TEN)/a);
        proc_t -> ip += 1;
        break;
    })
DEF_CMD(MUL, 5, NO_ARG, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        StackPush(&proc_t -> stk, (a*b)/ACCURACY_TEN);
        proc_t -> ip += 1;
        break;
    })
DEF_CMD(OUT, 6, NO_ARG, 
    {
        StackPop(&proc_t -> stk, &a);
        printf("%.*lf\n", ACCURACY, double(a)/ACCURACY_TEN);
        proc_t -> ip += 1;
        break;
    })
DEF_CMD(IN, 7, NO_ARG, 
    {
        scanf("%lf", &elem_double);
        StackPush(&proc_t -> stk, (int)(elem_double*ACCURACY_TEN));
        proc_t -> ip += 1;
        break;
    })
DEF_CMD(DUMP, 8, NO_ARG,
    {
        DumpProcessor(proc_t);
        proc_t -> ip += 1;
        break;
    })
DEF_CMD(SQRT, 9, NO_ARG,
    {
        StackPop(&proc_t -> stk, &elem);
        elem_double = sqrt(double(elem) / ACCURACY_TEN);
        StackPush(&proc_t -> stk, (int)(elem_double*ACCURACY_TEN));
        proc_t -> ip += 1;
        break;
    })
DEF_CMD(SIN, 10, NO_ARG, 
    {
        StackPop(&proc_t -> stk, &elem);
        elem_double = sin(double(elem) / ACCURACY_TEN);
        StackPush(&proc_t -> stk, (int)(elem_double*ACCURACY_TEN));
        proc_t -> ip += 1;
        break; 
    })
DEF_CMD(COS, 11, NO_ARG, 
    {
        StackPop(&proc_t -> stk, &elem);
        elem_double = cos(double(elem) / ACCURACY_TEN);
        StackPush(&proc_t -> stk, (int)(elem_double*ACCURACY_TEN));
        proc_t -> ip += 1;
        break; 
    })
DEF_CMD(JA, 14, JMP, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        if (b > a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
        else proc_t -> ip += 2;
        break;
    })
DEF_CMD(JAE, 15, JMP, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        if (b >= a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
        else proc_t -> ip += 2;
        break;
    })
DEF_CMD(JB, 16, JMP, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        if (b < a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
        else proc_t -> ip += 2;
        break;
    })
DEF_CMD(JBE, 17, JMP, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        if (b <= a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
        else proc_t -> ip += 2;
        break;
    })
DEF_CMD(JE, 18, JMP, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        if (b == a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
        else proc_t -> ip += 2;
        break;
    })
DEF_CMD(JNE, 19, JMP, 
    {
        StackPop(&proc_t -> stk, &a);
        StackPop(&proc_t -> stk, &b);
        if (b != a) proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
        else proc_t -> ip += 2;
        break;
    })
DEF_CMD(JMP, 20, JMP, 
    {
        proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
        break;
    })
DEF_CMD(CALL, 21, JMP,
    {
        StackPush(&proc_t -> stk_adr, proc_t -> ip + 2);
        proc_t -> ip = proc_t -> code[proc_t -> ip + 1];
        break;
    })
DEF_CMD(RET, 22, NO_ARG, 
    {
        StackPop(&proc_t -> stk_adr, &proc_t -> ip);
        break;
    })
DEF_CMD(HLT, 43962, NO_ARG,
    {
        printf("Program`s end\n");
        work_programm = 0;
        break;
    })
