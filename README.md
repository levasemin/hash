# Оптимизация хэш таблицы

Хеш таблица имеет открытый тип и состоит из слов на английском языке, для её заполнения используются 7 томов Гарри Поттера, а для стресс теста файл с часто употребляемыми словами.

Целью данной работы является создание хеш таблицы, использование различных хэш функций и изучение различных сопосбов оптимизации языка С на примере таблицы.

## Описание работы

 Для определения нагруженных мест в программе используется профайлер callgrind, а для оценки оптимизации rdtsc.
 
Тестирование представляет из себя вызов функции `void run_test(hash_table *hash_table, struct buffer *buffer, int epoch)`, принимающей в себя  буффер из слов для поиска в хеш таблице и n кратном повторении этой о перации.
 
 ##### 1 этап
 Попробуем запустить профайлер на тесте неоптимизированной таблицы, используя хеш crc32.

 ![](https://github.com/levasemin/hash/blob/master/images/stage_1.png)
 
 Как видим, самым нагруженным местом является функция `uint hash_crc32(const char *key)`, которая вычисляет хеш,что в принципе логично, ведь она вызывается
 каждый раз, когда мы начинаем искать какое-то слово, чтобы узнать место списка в хеш таблице. Попробуем её оптимизировать. 
 
 Скорость составила 7689831940 тиков. 
  
  ##### 2 этап
  
  Заменена функция `uint hash_crc32(const char *key)` на `uint hash_crc32_intr(const char *key)`, использующая intrinsic функцию `unsigned __int64 _mm_crc32_u64 (unsigned __int64 crc, unsigned __int64 v)`. Посмотрим, как на это отреагирует профайлер и насколько ускорится программа.


uint hash_crc32_intr(const char *key)
{    
    long long *array = (long long *)key;

    uint res = 0;

    for (int i = 0; i < 4; i++)
    {
        if (array[i] == 0)
        {
            break;
        }
        
        res = _mm_crc32_u64(res, array[i]);
    }

    return res;
}
```

![](https://github.com/levasemin/hash/blob/master/images/stage_2.png)

После замены хеш функции главной проблемой стала функция `struct list *list_find(struct list *head, const char *elem)`. Если посмотреть на распределение нагрузки внутри неё, можно увидеть, что главная проблема `extern int strcmp (const char *__s1, const char *__s2)`, несмотря на то, что оставшиеся 52% занимает сравнение указателей с NULL. Ускорить сравнение указателей мы не можем, оно нагружает настолько сильно только из-за количества раз и факта обращения.

Скорость составила 5600845626 тиков, что быстрее на 17.17 %

##### 3 этап
Заменена функция `extern int strcmp (const char *__s1, const char *__s2)` на `int strcmp_intr(const char *string1, const char *string2)`, использующая intrinsic функции 

```
int strcmp_intr(const char *str1, const char *str2)
{
    __m256 arr1 = _mm256_load_ps((float *)str1);
    __m256 arr2 = _mm256_load_ps((float *)str2);
    
    __m256 cmp_res = _mm256_cmp_ps(arr1, arr2, _CMP_NEQ_OQ);
    
    int res = _mm256_movemask_ps(cmp_res);

    return res;
}
```

