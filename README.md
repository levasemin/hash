# Оптимизация хэш таблицы

Хеш таблица имеет открытый тип и состоит из слов на английском языке, для её заполнения используются 7 томов Гарри Поттера, а для стресс теста файл с часто употребляемыми словами.

Целью данной работы является создание хеш таблицы, использование различных хэш функций и изучение различных сопосбов оптимизации языка С на примере таблицы.

## Описание работы

 Для определения нагруженных мест в программе используется профайлер callgrind, а для оценки оптимизации rdtsc.
 
Тестирование представляет из себя вызов функции `void run_test(hash_table *hash_table, struct buffer *buffer, int epoch)`, принимающей в себя  буффер из слов для поиска в хеш таблице и n кратном повторении этой о перации.
 
 ## Тест функций для хеширования на коллизии и скорость
Нарисуем графики, отражающие коллизии хеш функций с помощью GnuPlot и замерим скорость поиска в каждой хеш таблицы. Коллизия замерена на основе данных из 7 томов, для лучшей оценки 1% самых больших значений обрезается до максимума из оставшихся 99%. Скорость измеряется на основе поиска 10 раз слов из файла "eng_words.txt" в хеш таблице с выбранной функцией.

### Посмотрим на скорость 

```
Hash function: HashOnlyOne
Time : 142949718864

Hash function: HashFirstWord
Time : 14838281256

Hash function: HashAsciiSum
Time : 766177758

Hash function: HashLenWord
Time : 36776268010

Hash function: HashRolling
Time : 326661378

Hash function: HashCrc32
Time : 150202254
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

### First word
Функция хеширования `uint hash_first_word(const char *key)`.
Возвращает: ASCII код первой буквы слова.

![](https://github.com/levasemin/hash/blob/master/images/graphics/first_word.png)

```
uint hash_first_word(const char *key)
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
 
 ##### 1 этап
 Попробуем запустить профайлер на тесте неоптимизированной таблицы, используя хеш crc32.

 ![](https://github.com/levasemin/hash/blob/master/images/stage_1.png)
 
 Как видим, самым нагруженным местом является функция `uint hash_crc32(const char *key)`, которая вычисляет хеш,что в принципе логично, ведь она вызывается
 каждый раз, когда мы начинаем искать какое-то слово, чтобы узнать место списка в хеш таблице. Попробуем её оптимизировать. 
 
 Скорость составила 1205 млн тиков.
  
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

Скорость составила 961 млн тиков, что быстрее на 25.2 %

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

Скорость составила 858 млн тиков, что быстрее на 10.5 %

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

![](https://github.com/levasemin/hash/blob/master/images/speed/speed_ascii_sum_o3.png)

Результат функции на ассемблере

![](https://github.com/levasemin/hash/blob/master/images/speed/speed_ascii_sum_asm_o3.png)

Как видим, разницы нет, сложно написать код на ассемблере, который утрет нос оптимизации О3

### Inline ассемблер
Попробуем другой вариант использования ассемблера - вставка в код, изменим функцию `uint hash_rolling(const char *key)` на `uint hash_rolling_asm(const char *key)`.

Результат функции на С

![](https://github.com/levasemin/hash/blob/master/images/speed/speed_hash_rolling.png)

Результат функции с ассемблером

![](https://github.com/levasemin/hash/blob/master/images/speed/speed_hash_rolling_asm.png)

Разницы особой нет, 3%, что в районе погрешности.

### Итог
Из оптимизаций на уровне логики- подбор хеш функции. тк от её коллизии напрямую зависит скорость работы хеш таблицы. С помощью оптимизаций на програмном уровне мы смогли на уровне оптимизации О3 с лучшей хеш функцией усокрить работу на 30.1%. 
