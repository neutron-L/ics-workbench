/* this is a program for myself to test my code */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m);
int highest_bit(uint64_t num);


// bool judge_highest_bit(uint64_t num, int i)
// {
// 	return ((1<<(i - 1)) <= num && (1<<i) > num) ||  (num == 0 && i == 0);
// }


// void test_highest_bit(int argc, char **argv)
// {
// 	uint64_t num;
// 	int i;
// 	if (argc == 2)
// 	{
//         num = strtoul(argv[1], NULL, 10);
//         i = highest_bit(num);
//         if (!judge_highest_bit(num, i))
//         {
//             printf("error %ld %d\n", num, i);	
//         }
//     }
// 	else
// 	{
//         int count = 10000;
//         while (count--)
//         {
//             num = rand();
//             i = highest_bit(num);
//             if(!judge_highest_bit(num, i))
//             {
//                 printf("error: %ld %d\n", num, i);
//                 break;
//             }
//             printf("%ld %d\n", num, i);	
// 	}
// }

void test_multimod(int argc, char ** argv)
{
    char buf[64];
    char cmd[512];
    uint64_t a, b, m;

    if (argc == 4)
    {
        a = strtoul(argv[1], NULL, 10);
        b = strtoul(argv[2], NULL, 10);
        m = strtoul(argv[3], NULL, 10);

        memset(buf, 0, sizeof(buf));
        memset(cmd, 0, sizeof(cmd));
        
        sprintf(cmd, "python3 -c 'print(%lu*%lu %% %lu)'", a, b, m);
	printf("%s\n", cmd);
        FILE *fp = popen(cmd, "r");
        assert(fp);
        fscanf(fp, "%s", buf);
        uint64_t answer = strtoul(buf, NULL, 10);
        uint64_t output = multimod(a, b, m);
        printf("answer: %lu; output: %lu\n", answer, output);
        pclose(fp);  
    }
    else if (argc == 1)
    {
        // srand((unsigned)time(NULL));
        int count = 10;
        while(count--)
        {
            a = rand();
            b = rand();
            m = rand();
            sprintf(cmd, "python3 -c 'print(%lu*%lu %% %lu)'", a, b, m);
            printf("%s\n", cmd);
            FILE *fp = popen(cmd, "r");
            assert(fp);
            fscanf(fp, "%s", buf);
            uint64_t answer = strtoul(buf, NULL, 10);
            uint64_t output = multimod(a, b, m);
            // printf("answer: %ld; output: %ld\n", answer, output);
            if (answer != output)
                printf("error a: %lu; b: %lu; m: %lu\n", a, b, m);
            printf("%lu * %lu %% %lu = %lu\n", a, b, m, output);
            pclose(fp);  
        }
    }
    else
    {
        printf("usage: ./test <number1> <number2> <number3>\n");
        exit(1);
    }
}

int main(int argc, char ** argv)
{
    test_multimod(argc, argv);

    return 0;
}
