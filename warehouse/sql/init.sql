-- init.sql — схема базы данных для приложения «Склад / Магазин»

DROP TABLE IF EXISTS products CASCADE;
DROP TABLE IF EXISTS categories CASCADE;
DROP TABLE IF EXISTS suppliers CASCADE;

CREATE TABLE categories (
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(100) NOT NULL UNIQUE,
    description TEXT
);

CREATE TABLE suppliers (
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(150) NOT NULL,
    contact     VARCHAR(100),
    phone       VARCHAR(30)
);

CREATE TABLE products (
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(200) NOT NULL,
    price       NUMERIC(12, 2) NOT NULL CHECK (price >= 0),
    quantity    INTEGER NOT NULL DEFAULT 0 CHECK (quantity >= 0),
    category_id INTEGER NOT NULL REFERENCES categories(id) ON DELETE RESTRICT,
    supplier_id INTEGER REFERENCES suppliers(id) ON DELETE SET NULL
);

-- Индексы для ускорения выборок
CREATE INDEX idx_products_category ON products(category_id);
CREATE INDEX idx_products_supplier ON products(supplier_id);
CREATE INDEX idx_products_quantity ON products(quantity);

-- Начальные данные
INSERT INTO categories (name, description) VALUES
    ('Электроника', 'Смартфоны, ноутбуки, аксессуары'),
    ('Одежда', 'Мужская и женская одежда'),
    ('Продукты', 'Продукты питания'),
    ('Книги', 'Художественная и учебная литература');

INSERT INTO suppliers (name, contact, phone) VALUES
    ('ООО ТехноМир', 'Иванов А.С.', '+7-495-111-22-33'),
    ('ИП Смирнова', 'Смирнова Е.В.', '+7-916-444-55-66'),
    ('ГлобалТрейд', 'Петров Д.И.', '+7-812-777-88-99');

INSERT INTO products (name, price, quantity, category_id, supplier_id) VALUES
    ('Смартфон Xiaomi 13', 45990.00, 15, 1, 1),
    ('Ноутбук ASUS VivoBook', 62990.00, 8, 1, 1),
    ('Наушники Sony WH-1000XM5', 34990.00, 3, 1, 1),
    ('Футболка мужская', 1290.00, 50, 2, 2),
    ('Джинсы женские', 3490.00, 25, 2, 2),
    ('Молоко 1л', 89.90, 120, 3, 3),
    ('Хлеб белый', 45.00, 80, 3, 3),
    ('Война и мир', 890.00, 12, 4, NULL),
    ('C++ Primer', 2490.00, 5, 4, 1);