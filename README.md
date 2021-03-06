# Оптимизация хэш таблицы

Хеш таблица имеет открытый тип и состоит из слов на английском языке, для её заполнения используются 7 томов Гарри Поттера, а для стресс теста файл с часто употребляемыми словами.

Целью данной работы является создание хеш таблицы, использование различных хэш функций и изучение различных сопосбов оптимизации языка С на примере таблицы.

## Описание работы

Для определения нагруженных мест в программе используется профайлер callgrind, а для оценки оптимизации rdtsc.
 
Тестирование представляет из себя вызов функции `void run_test(hash_table *hash_table, struct buffer *buffer, int epoch)`, принимающей в себя  буффер из слов для поиска в хеш таблице и epoch кратном повторении этой операции.
 
 ## Тест функций для хеширования на коллизии и скорость
Нарисуем графики, отражающие коллизии хеш функций с помощью GnuPlot и замерим скорость поиска в каждой хеш таблицы. Коллизия замерена на основе данных из 7 томов. Скорость измеряется на основе поиска 10 раз слов из файла "eng_words.txt" в хеш таблице с выбранной функцией.

### Посмотрим на скорость 

```
Hash function: HashOnlyOne
Time : 139255952366

Hash function: HashFirstLetter
Time : 18082155664

Hash function: HashLenWord
Time : 66315319400

Hash function: HashAsciiSum
Time : 835738956

Hash function: HashRolling
Time : 457679570

Hash function: HashCrc32
Time : 245941998
```   

### Only one
Функция хеширования `uint hash_only_one(const char *key)`.
Возвращает: 1.

![](https://github.com/levasemin/hash/blob/master/images/graphics/only_one.png)

```
uint hash_only_one(const char *key)
{
    return 1;
}
```

### First letter
Функция хеширования `uint hash_first_letter(const char *key)`.
Возвращает: ASCII код первой буквы слова.

![](https://github.com/levasemin/hash/blob/master/images/graphics/first_letter.png)

```
uint hash_first_letter(const char *key)
{
    return (uint)*key;
}
```

### Len word
Функция хеширования `uint hash_len_word(const char *key)`. 
Возвращает: длина слова.

![](https://github.com/levasemin/hash/blob/master/images/graphics/len_word.png)

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
Функция хеширования `uint hash_rolling(const char *key)`.
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
Функция хеширования  `uint hash_crc32(const char *key)`.
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

 Итак, лучше всего для нашей хеш таблицы из выбранных функций подходит crc32.
 
 ## Оптимизация 
 
 ##### 0 этап 
 Запустим тест хеш таблицы без оптимизаций, который состоит из поиска часто употребляемых слов в файле `eng_words.txt` 100 раз, и посмотрим на профайлер.
 
 ![](https://github.com/levasemin/hash/blob/master/images/stage_1.png)
 
 Как видим, проблема в основном в `extern int strcmp (const char *__s1, const char *__s2)`.
 Скорость составила:
 
 ```
 Hash function: HashCrc32
 Time : 1582420990
 ```
 
 ##### 1 этап
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
![](https://github.com/levasemin/hash/blob/master/images/stage_2.png)

Скорость составила
```
Hash function: HashCrc32
Time : 1421138226
```
что быстрее на 10.2 %

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

![](https://github.com/levasemin/hash/blob/master/images/stage_3.png)

После замены хеш функции главной проблемой стала функция `struct list *list_find(struct list *head, const char *elem)`. Если посмотреть на распределение нагрузки внутри неё, можно увидеть, что главная проблема `extern int strcmp (const char *__s1, const char *__s2)`, несмотря на то, что оставшиеся 33% занимает сравнение указателей с NULL. Ускорить сравнение указателей мы не можем, оно нагружает настолько сильно только из-за количества раз и факта обращения.

Скорость составила 
```
Hash function: HashCrc32Intr
Time : 1337067284
```

Прирост составил 6.2%.


### Альтернативные способы оптимизации

## Функция на ассемблере 
Попробуем использовать силу ассемблера, предполагая, что всё на нём будет летать со скоростью света.

Напишем функцию `uint hash_ascii_sum(const char *key)` на ассемблере `u_int32_t hash_ascii_sum_asm(const char *key)` и прогоним два варианта на 100 повторениях будет ли с ней работать быстрее?

```
global hash_ascii_sum_asm

hash_ascii_sum_asm:

push rbx 
push rdi

xor eax, eax
xor ebx, ebx

next_symbol:

mov bl, [rdi]

cmp bl, 0
je out

add eax, ebx

inc rdi

jmp next_symbol
out:

pop rdi
pop rbx

ret
```

Результат функции на С 

```
Hash function: HashAsciiSum
Time : 6267579108
```

Результат функции на ассемблере

```
Hash function: HashAsciiSumAsm
Time : 6312632476
```

Как видим, разницы нет, сложно написать код на ассемблере, который утрет нос оптимизации О3

### Inline ассемблер
Попробуем другой вариант использования ассемблера - вставка в код, изменим функцию `uint hash_rolling(const char *key)` на `uint hash_rolling_asm(const char *key)`.

```
uint hash_rolling_asm(const char *key)
{
    uint hash = 0;

    __asm__  __volatile__ (
        ".intel_syntax noprefix ;" \
        "xor eax, eax ;"           \
        "xor edx, edx ;"           \
        "mov al, [%1];"            \
        "next_symbol%=: "          \

        "inc %1 ;"                 \
        "mov dl, [%1] ;"           \
        "cmp dl, 0 ;"              \
        "je out%= ;"               \
        "ror eax, 1 ;"             \
        "xor eax, edx ;"           \

        "jmp next_symbol%= ;"      \
        "out%=: "                  \
        "mov %0, eax ;"
        ".att_syntax prefix ;"     \
        :"=r"(hash)                \
        :"r"(key)                  \
        :"eax", "edx", "dl");

    return hash;
}
```
Результат функции на С

```
Hash function: HashRolling
Time : 2303597400
```

Результат функции с ассемблером

```
Hash function: HashRollingAsm
Time : 2283160756
```
Стало чуть лучше, но из-за ухудшения читаемости это того не стоит.

### Итог
Из оптимизаций на уровне логики- подбор хеш функции. тк от её коллизии напрямую зависит скорость работы хеш таблицы. С помощью оптимизаций на програмном уровне мы смогли на уровне оптимизации О3 с лучшей хеш функцией усокрить работу на 15.9%. 
