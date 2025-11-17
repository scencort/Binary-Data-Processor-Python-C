/*
Петров Алексей 403ИС-22
Сортировка бинарного файла методом пузырька.
Процесс исплнения программы:
Пишется программа в .py файле, котороая через модуль struct создает бинарный файл.
В данном примере формат бинарника такой: "<Id16s8s": uns int, double, char[16], char[8]
Этот бинарный файл открывает программа на Си, которая ложит данные из файла в структуру Student.
Программа сортирует данные по полю структуры id (по возрастанию) на лету, записывая изменения сразу в файле, 
не используя массивы. Отсортированный файл читает программа в .py файле.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME "data.bin" // Я решил вынести имя файла в препроцессор

// Формируем данные в структуре
#pragma pack(push, 1)
struct Student {
    unsigned int id; // ID студента
    double avg_ball; // Средний балл
    char name[16]; // Имя
    char group[8]; // Группа
};
#pragma pack(pop)

// Функция вывода данных из файла
void print_data(FILE* file)
{
    struct Student s;

    // Читаем данные из файла и выводим их на экран
    while(fread(&s, sizeof(struct Student), 1, file)){

        s.name[15] = '\0';
        s.group[7] = '\0';

        printf("Information of student:\n");
        printf("ID: %d\n", s.id);
        printf("Averge ball: %lf\n", s.avg_ball);
        printf("Name: %s\n", s.name);
        printf("Group: %s\n", s.group);
        printf("-----------------------\n");

    }

    fseek(file, 0, SEEK_SET);
}

// Функция свапа двух структур данных в файле:
// pos1, pos2 - смещения структур в файле
void swap_structures(FILE* fp, long pos1, long pos2)
{
    struct Student s1, s2;
    size_t struct_size = sizeof(struct Student);

    // читаем первую структуру
    fseek(fp, pos1, SEEK_SET);
    if(fread(&s1, struct_size, 1, fp) != 1) return;

    // читаем вторую структуру
    fseek(fp, pos2, SEEK_SET);
    if(fread(&s2, struct_size, 1, fp) != 1) return;

    // Переписываем позиции структур
    fseek(fp, pos1, SEEK_SET);
    fwrite(&s2, struct_size, 1, fp);

    fseek(fp, pos2, SEEK_SET);
    fwrite(&s1, struct_size, 1, fp);

}

int main(void)
{
    // Подготовим инициализацию данных (просто для демонстрации)
    FILE* frb = NULL; // Пока ничего не открыли
    struct Student s_curr, s_next; // текущая структура и следующая (для сваппинга)
    size_t s_size = sizeof(struct Student); // Размер структуры данных
    unsigned int count_student = 0; // Количество структур в файле
    long cur_pos, next_pos; // Позиции структур в файле
    int swapped; // Переменная проверки перестановки

    // 1 - открываем файл и считаем кол-во записей
    frb = fopen(FILE_NAME, "rb+"); // Сразу и на чтение и на запись

    if(!frb){
        fprintf(stderr, "Error: cant open file %s\n", FILE_NAME);
        return -1;
    }

    // Выводим неотсортированные данные из файла
    print_data(frb);

    // Считаем размер файла через ftell()
    fseek(frb, 0, SEEK_END);
    long file_size = ftell(frb);
    // Если посчитать размер не удалось выведем ошибку
    if(file_size < 0){
        fprintf(stderr, "Error: cant getting file size\n");
        fclose(frb);
        return -1;
    }

    // Кол-во записей:
    count_student = file_size / s_size;

    if(count_student == 1){
        printf("File consists of one entry\n");
        fclose(frb);
        return 0;
    }
    else if(count_student < 1){

        printf("File empty\n");
        fclose(frb);
        return 0;

    }

    // printf("Total entrys in file: %u\n", count_student);

    // 2. Сортировка пузырЁм :) 
    // Сортируем данные по ID студента:
    for(int i = 0; i < count_student - 1; i++){
        swapped = 0;

        for(int j = 0; j < count_student - 1 - i; j++){
            
            cur_pos = j * s_size;
            next_pos = (j + 1) * s_size;

            // Читаем данные текущей структуры
            fseek(frb, cur_pos, SEEK_SET); // Взвращаем позицию

            if(fread(&s_curr, s_size, 1, frb) != 1){
                fprintf(stderr, "Error: cant read at position: %ld\n", cur_pos);
                fclose(frb);
                return -1;
            }

            // Читаем следующую
            fseek(frb, next_pos, SEEK_SET); // Взвращаем позицию

            if(fread(&s_next, s_size, 1, frb) != 1){
                fprintf(stderr, "Error: cant read at position: %ld\n", next_pos);
                fclose(frb);
                return -1;
            }

            // Сравниваем ID по возрастанию
            if(s_curr.id > s_next.id){
                swap_structures(frb, cur_pos, next_pos);
                swapped = 1;
            }

        }

        // Проверяем нашу swapped, если 0, значит свапов не было, всё отсортировано
        if(!swapped){
            printf("Sorting finish for %u round\n", i+1);
            break;
        }

    }


    // Выводим отсортированные данные
    printf("Output Data after bubble sorted:\n");
    fseek(frb, 0, SEEK_SET); // Возвращаем позицию в начало, чтобы прочитать данные
    print_data(frb);

    fclose(frb); // Закрываем поток
    return 0;
}