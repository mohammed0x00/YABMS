#ifndef MMULT_DATASET_CONF_H
#define MMULT_DATASET_CONF_H

typedef struct {
    int colsA;
    int rowsA;
    int rowsB;
    char * name;
} dataset_t;

const dataset_t dataset[] =
{
    {16, 12, 8, "testing"},
    {121, 180, 115, "small"},
    {550, 620, 480, "medium"},
    {962, 1012, 1221, "large"},
    {2500, 3000, 2100, "native"}
};

#endif // MMULT_DATASET_CONF_H