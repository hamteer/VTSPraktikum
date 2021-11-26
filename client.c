#include "math.h"
#include <rpc/rpc.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    CLIENT* cl = clnt_create (argv[1], MATHPROG, MATHVERS, "tcp");
    if (argc == 4) {
        // declaration
        int a = atoi(argv[2]);
        int b = atoi(argv[3]);

        intpair numbers;
        numbers.a = a;
        numbers.b = b;

        static int result_add;
        result_add = *add_1(&numbers, cl);
        printf("Ergebnis der Addition: %d\n", result_add);

        static int result_multiply;
        result_multiply = *multiply_1(&numbers, cl);
        printf("Ergebnis der Multiplikation: %d\n", result_multiply);

        static int result_cube1;
        result_cube1 = *cube_1(&a, cl);
        printf("Erste Zahl hoch 3: %d\n", result_cube1);

        static int result_cube2;
        result_cube2 = *cube_1(&b, cl);
        printf("Zweite Zahl hoch 3: %d\n", result_cube2);
    } else {
        //declaration
        int a = *argv[2];

        printf("Bitte zwei Zahlen für Addition und Multiplikation angeben\n");

        static int result_cube;
        result_cube = *cube_1(&a, cl);
        printf("Zahl hoch 3: %d\n", result_cube);
    }
}