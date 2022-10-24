#include "list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    unsigned char cdm_result[4];
    unsigned char flash_result[4];
    unsigned char irm1_result[4];
    unsigned char irm2_result[4];
    unsigned char have_chip;
} chipResult_t;

int main(void)
{
    list_t *list = (list_t *)malloc(sizeof(list_t));
    chipResult_t *chip;
    list_init(list);

    for (unsigned char i = 0; i < 5; i++)
    {
        chip = (chipResult_t *)malloc(sizeof(chipResult_t));
        chip->have_chip = i;
        list_insert(list, 0, chip);
    }

    chip = (chipResult_t *)malloc(sizeof(chipResult_t));
    chip->have_chip = 100;
    list_set(list, 2, chip);
    char data; //用来取出数据
    int pos;   //用来记录位置

    while (list_iter(list, (void **)&chip, &pos)) //迭代打印链表
        printf("%d %d\n", chip->have_chip, pos);

    return 0;
}
