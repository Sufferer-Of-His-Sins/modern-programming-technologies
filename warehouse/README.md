# Склад / Магазин

Консольное GUI-приложение на C++ для учёта товаров на складе.

**Стек:** C++17 · FLTK · libpqxx · PostgreSQL · CMake

## Возможности

- CRUD для **товаров**, **категорий** и **поставщиков**
- Фильтрация товаров по категории
- Отчёт «Товары с низким остатком»
- Архитектурное разделение на слои:
  - `domain` — сущности предметной области
  - `repository` — доступ к PostgreSQL
  - `service` — бизнес-логика и валидация
  - `ui` — графический интерфейс (FLTK)
  - `db` — подключение к БД

## Настройка базы данных

1. Создаем базу и пользователя:

```bash
sudo -u postgres psql
```

```sql
CREATE USER warehouse_user WITH PASSWORD 'secret';
CREATE DATABASE warehouse OWNER warehouse_user;
\q
```

2. Применяем схему:

```bash
psql -U warehouse_user -d warehouse -f sql/init.sql
```
## Настройка подключения

Содержимое `.env`:

```
PGHOST=localhost
PGPORT=5432
PGDATABASE=warehouse
PGUSER=warehouse_user
PGPASSWORD=secret
```
## Сборка

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Запуск

```bash
./warehouse
```

## Структура проекта

```
warehouse/
├── CMakeLists.txt
├── README.md
├── .env.example
├── sql/
│   └── init.sql
└── src/
    ├── main.cpp
    ├── domain/          # сущности
    ├── repository/      # PostgreSQL
    ├── service/         # логика
    ├── db/              # подключение
    └── ui/              # FLTK GUI
```