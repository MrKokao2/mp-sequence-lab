# Лабораторная работа №0

## Тема

Разработка обобщённой структуры данных «последовательность» (`Sequence`)
с двумя реализациями: на динамическом массиве и на двусвязном списке.

## Цель работы

Освоить шаблоны C++, наследование от абстрактного интерфейса и базовые
приёмы UDD (Use-Driven Design): сначала пишется набор тестов, под который
подстраиваются обе реализации.

## Структура

```
lab0/
├── include/
│   ├── Sequence.h         — абстрактный интерфейс
│   ├── ArraySequence.h    — реализация на std::vector
│   └── ListSequence.h     — реализация на std::list
├── src/
│   ├── main.cpp           — точка входа демо-приложения
│   └── sequence.cpp       — заглушка для сборки разделяемой библиотеки
├── tests/
│   └── test_sequence.cpp  — общие тесты для обеих реализаций
├── CMakeLists.txt
└── Dockerfile
```

## Интерфейс `Sequence<T>`

```cpp
virtual int  getLength()  const = 0;
virtual bool getIsEmpty() const = 0;

virtual T Get(int index)  const = 0;
virtual T GetFirst()      const = 0;
virtual T GetLast()       const = 0;

virtual void Append(T item)          = 0;
virtual void Prepend(T item)         = 0;
virtual void InsertAt(int i, T item) = 0;
virtual void Remove(T item)          = 0;
```

Каждая реализация дополнительно предоставляет `GetSubsequence(start, end)`,
возвращающую новый объект того же типа.

## Особенности и нюансы

- **Один набор тестов на две реализации.** Шаблонная функция
  `runSequenceTest<TSequence>()` параметризуется типом и проверяет одну
  и ту же контрактную семантику для `ArraySequence<int>` и
  `ListSequence<int>`.
- **Проверка границ.** Все методы доступа по индексу бросают
  `std::out_of_range`. Это явно покрыто в тестах через `try/catch`.
- **Сложность операций.** У `ArraySequence` `Get` имеет `O(1)`, но
  `Prepend` — `O(n)`. У `ListSequence` ситуация обратная: `Append`/
  `Prepend` `O(1)`, а `Get(i)` — `O(i)`, потому что приходится
  «прокручивать» итератор через `std::advance`.
- **Компоновка как разделяемая библиотека.** `CMakeLists.txt` собирает
  `libsequence` как `SHARED`. Заголовки шаблонные, поэтому в .so/.dll
  попадает только маркер-символ — это упражнение на устройство сборки,
  а не на ABI шаблонов.

## Сборка и запуск

```bash
cmake -S . -B build
cmake --build build

./build/sequence_app     # запустит «Sequence application started»
./build/sequence_tests   # прогон тестов, печатает "All tests passed"
```

В Docker:

```bash
docker build -t lab0 .
docker run --rm lab0
```
