#include <stdio.h>
#include <stdlib.h>

int hoursWorked;
double basicPayRate = 12.0;
int overTimeThreshold = 40;
double overTimePayCoefficient = 1.5;
double grossPay;
double totalTax;
double netPay;

int main()
{
    // Get user input:
    printf("How many hours did you work? ");
    scanf("%d", &hoursWorked);

    // Calculate gross pay
    if (hoursWorked > overTimeThreshold) {
        grossPay = basicPayRate*(overTimeThreshold + (hoursWorked-overTimeThreshold)*overTimePayCoefficient);
    }
    else {
        grossPay = hoursWorked*basicPayRate;
    }


    // Calculate total tax
    if (grossPay > 300 + 150) {
        totalTax = 0.25*(grossPay-450)+0.2*150+0.15*300;
    }
    else if (grossPay > 300) {
        totalTax = 0.2*(grossPay - 300) + 0.15*300;
    }
    else {
        totalTax = 0.15*grossPay;
    }


    // Calculate net pay
    netPay = grossPay - totalTax;

    // Print result
    printf("Hours worked: %d\n", hoursWorked);
    printf("Gross pay: %g\n", grossPay);
    printf("Total tax: %g\n", totalTax);
    printf("Net pay: %g\n", netPay);

    return 0;
}
