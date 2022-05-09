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

```
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
![](https://github.com/levasemin/hash/blob/master/images/stage_3.png)

Скорость составила 5073327920 тиков, что быстрее на 9.42 %

## Посмотрим на графики функций для хеширования
Все графики отображают коллизию выбранной функции на основе данных из 7 томов, для лучшей оценки 1% самых больших значений обрезается до максимума из оставшихся 99%.

### Only one
Функция хеширования `uint hash_only_one(const char *key)`
Возвращает: 1.

![](https://github.com/levasemin/hash/blob/master/images/graphics/only_one.png)

```
uint hash_only_one(const char *key)
{
    return 1;
}
```

### First word
Функция хеширования `uint hash_first_word(const char *key)` 
Возвращает: ASCII код первой буквы слова.

![](https://github.com/levasemin/hash/blob/master/images/graphics/first_word.png)

```
uint hash_first_word(const char *key)
{
    return (uint)*key;
}
```

### Len word
Функция хеширования `uint hash_len_word(const char *key)`
Возвращает: длина слова.

![](https://github.com/levasemin/hash/blob/master/images/graphics/hash_len.png)

```
uint hash_len_word(const char *key)
{
    return strlen(key);
}
```

### ASCII Sum
Функция хеширования `uint hash_ascii_sum(const char *key)`
Возвращает: сумму ASCII кодов букв слова

![](https://github.com/levasemin/hash/blob/master/images/graphics/ascii_sum.png)

```
uint hash_ascii_sum(const char *key)
{
    uint hash = 0;

    for (; *key != '\0'; hash += *key, key++);
    
    return hash;
}
```

### Rolling hash
Функция хеширования `uint hash_rolling(const char *key)`
Возвращает: значение по формуле.

![](https://github.com/levasemin/hash/blob/master/images/graphics/rolling.png)

```
uint hash_rolling(const char *key)
{
    uint hash = key[0];
    
    for(int i = 1; key[i] != '\0'; i++)
    {
        hash = ((hash >> 1) | (hash << 31)) ^ key[i];
    } 

    return hash;
}
```

### CRC32 hash
Функция хеширования  `uint hash_crc32(const char *key)`
Возвращает: значение по формуле.

![](https://github.com/levasemin/hash/blob/master/images/graphics/crc32.png)

```
uint hash_crc32(const char *key)
{
    unsigned ret = 0xFFFFFFFF;
    
    while (*key != '\0')
    {
        ret ^= *key & 0xFF;
        ret = (ret >> 8) ^ crc32_table[ret & 0xFF];
        
        key++;
    }

    return ret ^ 0xFFFFFFFF;
}
```
По графикам можно заметить, что наименьшие значения и разброс по всем ячейкам в хеш таблице принимает crc32. 
