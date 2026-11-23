#include "MainWindow.hpp"
#include <FL/fl_draw.H>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace ui {


MainWindow::ProductsTable::ProductsTable(int x, int y, int w, int h, MainWindow* owner)
    : Fl_Table(x, y, w, h), owner_(owner)
{
    cols(6);
    col_header(1);
    col_resize(1);
    row_header(0);
    row_resize(0);
    col_width_all(120);
    col_width(0, 50);
    col_width(1, 200);
    col_width(2, 90);
    col_width(3, 80);
    col_width(4, 140);
    col_width(5, 140);
    end();
}

void MainWindow::ProductsTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
    static const char* headers[] = {"ID", "Название", "Цена", "Кол-во", "Категория", "Поставщик"};

    switch (context) {
    case CONTEXT_STARTPAGE:
        fl_font(FL_HELVETICA, 12);
        return;
    case CONTEXT_COL_HEADER:
        fl_push_clip(X, Y, W, H);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
        fl_color(FL_BLACK);
        fl_draw(headers[C], X + 4, Y, W - 8, H, FL_ALIGN_LEFT);
        fl_pop_clip();
        return;
    case CONTEXT_CELL: {
        fl_push_clip(X, Y, W, H);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, (R % 2) ? FL_WHITE : fl_rgb_color(240, 245, 255));
        fl_color(FL_BLACK);

        if (R < static_cast<int>(owner_->products_.size())) {
            const auto& p = owner_->products_[R];
            std::ostringstream oss;
            switch (C) {
            case 0: oss << p.id; break;
            case 1: oss << p.name; break;
            case 2: oss << std::fixed << std::setprecision(2) << p.price; break;
            case 3: oss << p.quantity; break;
            case 4: oss << p.category_name; break;
            case 5: oss << (p.supplier_name.empty() ? "—" : p.supplier_name); break;
            }
            fl_draw(oss.str().c_str(), X + 4, Y, W - 8, H, FL_ALIGN_LEFT);
        }
        fl_pop_clip();
        return;
    }
    default:
        return;
    }
}

MainWindow::CategoriesTable::CategoriesTable(int x, int y, int w, int h, MainWindow* owner)
    : Fl_Table(x, y, w, h), owner_(owner)
{
    cols(3);
    col_header(1);
    col_resize(1);
    col_width(0, 60);
    col_width(1, 200);
    col_width(2, 350);
    end();
}

void MainWindow::CategoriesTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
    static const char* headers[] = {"ID", "Название", "Описание"};
    switch (context) {
    case CONTEXT_STARTPAGE: fl_font(FL_HELVETICA, 12); return;
    case CONTEXT_COL_HEADER:
        fl_push_clip(X, Y, W, H);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
        fl_color(FL_BLACK);
        fl_draw(headers[C], X + 4, Y, W - 8, H, FL_ALIGN_LEFT);
        fl_pop_clip();
        return;
    case CONTEXT_CELL: {
        fl_push_clip(X, Y, W, H);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, (R % 2) ? FL_WHITE : fl_rgb_color(240, 245, 255));
        fl_color(FL_BLACK);
        if (R < static_cast<int>(owner_->categories_.size())) {
            const auto& c = owner_->categories_[R];
            std::string text;
            if (C == 0) text = std::to_string(c.id);
            else if (C == 1) text = c.name;
            else text = c.description;
            fl_draw(text.c_str(), X + 4, Y, W - 8, H, FL_ALIGN_LEFT);
        }
        fl_pop_clip();
        return;
    }
    default: return;
    }
}

MainWindow::SuppliersTable::SuppliersTable(int x, int y, int w, int h, MainWindow* owner)
    : Fl_Table(x, y, w, h), owner_(owner)
{
    cols(4);
    col_header(1);
    col_resize(1);
    col_width(0, 60);
    col_width(1, 200);
    col_width(2, 180);
    col_width(3, 150);
    end();
}

void MainWindow::SuppliersTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
    static const char* headers[] = {"ID", "Название", "Контакт", "Телефон"};
    switch (context) {
    case CONTEXT_STARTPAGE: fl_font(FL_HELVETICA, 12); return;
    case CONTEXT_COL_HEADER:
        fl_push_clip(X, Y, W, H);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
        fl_color(FL_BLACK);
        fl_draw(headers[C], X + 4, Y, W - 8, H, FL_ALIGN_LEFT);
        fl_pop_clip();
        return;
    case CONTEXT_CELL: {
        fl_push_clip(X, Y, W, H);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, (R % 2) ? FL_WHITE : fl_rgb_color(240, 245, 255));
        fl_color(FL_BLACK);
        if (R < static_cast<int>(owner_->suppliers_.size())) {
            const auto& s = owner_->suppliers_[R];
            std::string text;
            if (C == 0) text = std::to_string(s.id);
            else if (C == 1) text = s.name;
            else if (C == 2) text = s.contact;
            else text = s.phone;
            fl_draw(text.c_str(), X + 4, Y, W - 8, H, FL_ALIGN_LEFT);
        }
        fl_pop_clip();
        return;
    }
    default: return;
    }
}

MainWindow::LowStockTable::LowStockTable(int x, int y, int w, int h, MainWindow* owner)
    : Fl_Table(x, y, w, h), owner_(owner)
{
    cols(5);
    col_header(1);
    col_resize(1);
    col_width(0, 50);
    col_width(1, 220);
    col_width(2, 90);
    col_width(3, 80);
    col_width(4, 150);
    end();
}

void MainWindow::LowStockTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
    static const char* headers[] = {"ID", "Название", "Цена", "Остаток", "Категория"};
    switch (context) {
    case CONTEXT_STARTPAGE: fl_font(FL_HELVETICA, 12); return;
    case CONTEXT_COL_HEADER:
        fl_push_clip(X, Y, W, H);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
        fl_color(FL_BLACK);
        fl_draw(headers[C], X + 4, Y, W - 8, H, FL_ALIGN_LEFT);
        fl_pop_clip();
        return;
    case CONTEXT_CELL: {
        fl_push_clip(X, Y, W, H);
        Fl_Color bg = (R % 2) ? FL_WHITE : fl_rgb_color(255, 240, 240);
        if (R < static_cast<int>(owner_->lowStock_.size()) && owner_->lowStock_[R].quantity <= 3)
            bg = fl_rgb_color(255, 200, 200);
        fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, bg);
        fl_color(FL_BLACK);
        if (R < static_cast<int>(owner_->lowStock_.size())) {
            const auto& p = owner_->lowStock_[R];
            std::ostringstream oss;
            switch (C) {
            case 0: oss << p.id; break;
            case 1: oss << p.name; break;
            case 2: oss << std::fixed << std::setprecision(2) << p.price; break;
            case 3: oss << p.quantity; break;
            case 4: oss << p.category_name; break;
            }
            fl_draw(oss.str().c_str(), X + 4, Y, W - 8, H, FL_ALIGN_LEFT);
        }
        fl_pop_clip();
        return;
    }
    default: return;
    }
}


MainWindow::MainWindow(int w, int h,
                       std::shared_ptr<service::ProductService> productService,
                       std::shared_ptr<service::CategoryService> categoryService,
                       std::shared_ptr<service::SupplierService> supplierService)
    : Fl_Window(w, h, "Склад / Магазин")
    , productService_(std::move(productService))
    , categoryService_(std::move(categoryService))
    , supplierService_(std::move(supplierService))
{
    buildUi();
    refreshCategories();
    refreshSuppliers();
    refreshProducts();
    refreshLowStock();
}

void MainWindow::buildUi() {
    begin();

    tabs_ = new Fl_Tabs(10, 10, w() - 20, h() - 20);
    tabs_->begin();

    productsTab_ = new Fl_Group(10, 35, w() - 20, h() - 45, "Товары");
    productsTab_->begin();

    productsTable_ = new ProductsTable(20, 50, w() - 40, h() - 140, this);

    filterCategory_ = new Fl_Choice(100, h() - 80, 200, 28, "Категория:");
    filterCategory_->callback(onFilterCategory, this);

    btnAddProduct_ = new Fl_Button(320, h() - 80, 100, 28, "Добавить");
    btnAddProduct_->callback(onAddProduct, this);

    btnEditProduct_ = new Fl_Button(430, h() - 80, 100, 28, "Изменить");
    btnEditProduct_->callback(onEditProduct, this);

    btnDeleteProduct_ = new Fl_Button(540, h() - 80, 100, 28, "Удалить");
    btnDeleteProduct_->callback(onDeleteProduct, this);

    btnRefreshProducts_ = new Fl_Button(650, h() - 80, 100, 28, "Обновить");
    btnRefreshProducts_->callback(onRefreshProducts, this);

    productsTab_->end();

    categoriesTab_ = new Fl_Group(10, 35, w() - 20, h() - 45, "Категории");
    categoriesTab_->begin();

    categoriesTable_ = new CategoriesTable(20, 50, w() - 40, h() - 140, this);

    btnAddCategory_ = new Fl_Button(20, h() - 80, 100, 28, "Добавить");
    btnAddCategory_->callback(onAddCategory, this);

    btnEditCategory_ = new Fl_Button(130, h() - 80, 100, 28, "Изменить");
    btnEditCategory_->callback(onEditCategory, this);

    btnDeleteCategory_ = new Fl_Button(240, h() - 80, 100, 28, "Удалить");
    btnDeleteCategory_->callback(onDeleteCategory, this);

    btnRefreshCategories_ = new Fl_Button(350, h() - 80, 100, 28, "Обновить");
    btnRefreshCategories_->callback(onRefreshCategories, this);

    categoriesTab_->end();

    suppliersTab_ = new Fl_Group(10, 35, w() - 20, h() - 45, "Поставщики");
    suppliersTab_->begin();

    suppliersTable_ = new SuppliersTable(20, 50, w() - 40, h() - 140, this);

    btnAddSupplier_ = new Fl_Button(20, h() - 80, 100, 28, "Добавить");
    btnAddSupplier_->callback(onAddSupplier, this);

    btnEditSupplier_ = new Fl_Button(130, h() - 80, 100, 28, "Изменить");
    btnEditSupplier_->callback(onEditSupplier, this);

    btnDeleteSupplier_ = new Fl_Button(240, h() - 80, 100, 28, "Удалить");
    btnDeleteSupplier_->callback(onDeleteSupplier, this);

    btnRefreshSuppliers_ = new Fl_Button(350, h() - 80, 100, 28, "Обновить");
    btnRefreshSuppliers_->callback(onRefreshSuppliers, this);

    suppliersTab_->end();

    reportsTab_ = new Fl_Group(10, 35, w() - 20, h() - 45, "Отчёты");
    reportsTab_->begin();

    new Fl_Box(20, 50, 300, 25, "Товары с низким остатком");

    thresholdInput_ = new Fl_Int_Input(150, 85, 80, 28, "Порог ≤");
    thresholdInput_->value("10");

    btnShowLowStock_ = new Fl_Button(250, 85, 120, 28, "Показать");
    btnShowLowStock_->callback(onShowLowStock, this);

    lowStockTable_ = new LowStockTable(20, 130, w() - 40, h() - 200, this);

    reportsTab_->end();

    tabs_->end();
    end();
    resizable(this);
}


void MainWindow::refreshProducts() {
    try {
        int catIdx = filterCategory_->value();
        if (catIdx <= 0) {
            products_ = productService_->getAll();
        } else {
            auto* idPtr = static_cast<int*>(filterCategory_->mvalue()->user_data());
            if (idPtr) {
                products_ = productService_->getByCategory(*idPtr);
            } else {
                products_ = productService_->getAll();
            }
        }
        productsTable_->rows(static_cast<int>(products_.size()));
        productsTable_->redraw();
    } catch (const std::exception& e) {
        fl_alert("Ошибка загрузки товаров:\n%s", e.what());
    }
}

void MainWindow::refreshCategories() {
    try {
        categories_ = categoryService_->getAll();
        categoriesTable_->rows(static_cast<int>(categories_.size()));
        categoriesTable_->redraw();

        fillCategoryChoice(filterCategory_, true);
    } catch (const std::exception& e) {
        fl_alert("Ошибка загрузки категорий:\n%s", e.what());
    }
}

void MainWindow::refreshSuppliers() {
    try {
        suppliers_ = supplierService_->getAll();
        suppliersTable_->rows(static_cast<int>(suppliers_.size()));
        suppliersTable_->redraw();
    } catch (const std::exception& e) {
        fl_alert("Ошибка загрузки поставщиков:\n%s", e.what());
    }
}

void MainWindow::refreshLowStock() {
    try {
        int threshold = 10;
        if (thresholdInput_ && thresholdInput_->value()) {
            threshold = std::atoi(thresholdInput_->value());
        }
        lowStock_ = productService_->getLowStock(threshold);
        lowStockTable_->rows(static_cast<int>(lowStock_.size()));
        lowStockTable_->redraw();
    } catch (const std::exception& e) {
        fl_alert("Ошибка отчёта:\n%s", e.what());
    }
}

void MainWindow::fillCategoryChoice(Fl_Choice* choice, bool withAll) {
    choice->clear();
    if (withAll) {
        choice->add("Все категории");
    }
    for (const auto& c : categories_) {
        int* id = new int(c.id);
        choice->add(c.name.c_str(), 0, nullptr, id);
    }
    choice->value(0);
}

void MainWindow::fillSupplierChoice(Fl_Choice* choice, bool withNone) {
    choice->clear();
    if (withNone) {
        choice->add("— без поставщика —");
    }
    for (const auto& s : suppliers_) {
        int* id = new int(s.id);
        choice->add(s.name.c_str(), 0, nullptr, id);
    }
    choice->value(0);
}

int MainWindow::selectedRow(Fl_Table* table) const {
    return -1;
}

void MainWindow::onRefreshProducts(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);
    self->refreshProducts();
}

void MainWindow::onFilterCategory(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);
    self->refreshProducts();
}

void MainWindow::onAddProduct(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);

    Fl_Window dlg(400, 320, "Новый товар");
    dlg.set_modal();
    dlg.begin();

    auto* nameIn = new Fl_Input(120, 20, 250, 28, "Название:");
    auto* priceIn = new Fl_Float_Input(120, 60, 120, 28, "Цена:");
    auto* qtyIn = new Fl_Int_Input(120, 100, 80, 28, "Количество:");
    auto* catChoice = new Fl_Choice(120, 140, 250, 28, "Категория:");
    auto* supChoice = new Fl_Choice(120, 180, 250, 28, "Поставщик:");

    self->fillCategoryChoice(catChoice, false);
    self->fillSupplierChoice(supChoice, true);

    auto* okBtn = new Fl_Button(120, 250, 100, 32, "Сохранить");
    auto* cancelBtn = new Fl_Button(240, 250, 100, 32, "Отмена");

    dlg.end();

    bool saved = false;
    okBtn->callback([](Fl_Widget* w, void* p) {
        *static_cast<bool*>(p) = true;
        w->window()->hide();
    }, &saved);
    cancelBtn->callback([](Fl_Widget* w, void*) {
        w->window()->hide();
    });

    dlg.show();
    while (dlg.shown()) Fl::wait();

    if (!saved) return;

    try {
        std::string name = nameIn->value();
        double price = std::atof(priceIn->value());
        int qty = std::atoi(qtyIn->value());

        int catId = 0;
        if (catChoice->mvalue() && catChoice->mvalue()->user_data()) {
            catId = *static_cast<int*>(catChoice->mvalue()->user_data());
        }

        std::optional<int> supId;
        if (supChoice->value() > 0 && supChoice->mvalue() && supChoice->mvalue()->user_data()) {
            supId = *static_cast<int*>(supChoice->mvalue()->user_data());
        }

        self->productService_->create(name, price, qty, catId, supId);
        self->refreshProducts();
        fl_message("Товар успешно добавлен");
    } catch (const std::exception& e) {
        fl_alert("Ошибка:\n%s", e.what());
    }
}

void MainWindow::onEditProduct(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);

    if (self->products_.empty()) {
        fl_alert("Нет товаров для редактирования");
        return;
    }

    const char* idStr = fl_input("Введите ID товара для изменения:", "");
    if (!idStr) return;
    int id = std::atoi(idStr);

    auto opt = self->productService_->getById(id);
    if (!opt) {
        fl_alert("Товар с ID %d не найден", id);
        return;
    }
    domain::Product p = *opt;

    Fl_Window dlg(400, 320, "Изменить товар");
    dlg.set_modal();
    dlg.begin();

    auto* nameIn = new Fl_Input(120, 20, 250, 28, "Название:");
    nameIn->value(p.name.c_str());
    auto* priceIn = new Fl_Float_Input(120, 60, 120, 28, "Цена:");
    priceIn->value(std::to_string(p.price).c_str());
    auto* qtyIn = new Fl_Int_Input(120, 100, 80, 28, "Количество:");
    qtyIn->value(std::to_string(p.quantity).c_str());
    auto* catChoice = new Fl_Choice(120, 140, 250, 28, "Категория:");
    auto* supChoice = new Fl_Choice(120, 180, 250, 28, "Поставщик:");

    self->fillCategoryChoice(catChoice, false);
    self->fillSupplierChoice(supChoice, true);

    for (int i = 0; i < catChoice->size(); ++i) {
        auto* item = catChoice->menu() + i;
        if (item->user_data() && *static_cast<int*>(item->user_data()) == p.category_id) {
            catChoice->value(i);
            break;
        }
    }
    if (p.supplier_id) {
        for (int i = 0; i < supChoice->size(); ++i) {
            auto* item = supChoice->menu() + i;
            if (item->user_data() && *static_cast<int*>(item->user_data()) == *p.supplier_id) {
                supChoice->value(i);
                break;
            }
        }
    }

    auto* okBtn = new Fl_Button(120, 250, 100, 32, "Сохранить");
    auto* cancelBtn = new Fl_Button(240, 250, 100, 32, "Отмена");

    dlg.end();

    bool saved = false;
    okBtn->callback([](Fl_Widget* w, void* p) {
        *static_cast<bool*>(p) = true;
        w->window()->hide();
    }, &saved);
    cancelBtn->callback([](Fl_Widget* w, void*) { w->window()->hide(); });

    dlg.show();
    while (dlg.shown()) Fl::wait();

    if (!saved) return;

    try {
        std::string name = nameIn->value();
        double price = std::atof(priceIn->value());
        int qty = std::atoi(qtyIn->value());

        int catId = p.category_id;
        if (catChoice->mvalue() && catChoice->mvalue()->user_data()) {
            catId = *static_cast<int*>(catChoice->mvalue()->user_data());
        }

        std::optional<int> supId;
        if (supChoice->value() > 0 && supChoice->mvalue() && supChoice->mvalue()->user_data()) {
            supId = *static_cast<int*>(supChoice->mvalue()->user_data());
        }

        self->productService_->update(id, name, price, qty, catId, supId);
        self->refreshProducts();
        fl_message("Товар обновлён");
    } catch (const std::exception& e) {
        fl_alert("Ошибка:\n%s", e.what());
    }
}

void MainWindow::onDeleteProduct(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);

    const char* idStr = fl_input("Введите ID товара для удаления:", "");
    if (!idStr) return;
    int id = std::atoi(idStr);

    if (fl_choice("Удалить товар с ID %d?", "Отмена", "Удалить", nullptr, id) != 1)
        return;

    try {
        self->productService_->remove(id);
        self->refreshProducts();
        fl_message("Товар удалён");
    } catch (const std::exception& e) {
        fl_alert("Ошибка:\n%s", e.what());
    }
}


void MainWindow::onRefreshCategories(Fl_Widget*, void* data) {
    static_cast<MainWindow*>(data)->refreshCategories();
}

void MainWindow::onAddCategory(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);

    const char* name = fl_input("Название категории:", "");
    if (!name || !*name) return;
    const char* desc = fl_input("Описание (необязательно):", "");

    try {
        self->categoryService_->create(name, desc ? desc : "");
        self->refreshCategories();
        self->refreshProducts();
        fl_message("Категория добавлена");
    } catch (const std::exception& e) {
        fl_alert("Ошибка:\n%s", e.what());
    }
}

void MainWindow::onEditCategory(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);

    const char* idStr = fl_input("ID категории для изменения:", "");
    if (!idStr) return;
    int id = std::atoi(idStr);

    auto opt = self->categoryService_->getById(id);
    if (!opt) {
        fl_alert("Категория не найдена");
        return;
    }

    const char* name = fl_input("Новое название:", opt->name.c_str());
    if (!name) return;
    const char* desc = fl_input("Новое описание:", opt->description.c_str());

    try {
        self->categoryService_->update(id, name, desc ? desc : "");
        self->refreshCategories();
        self->refreshProducts();
        fl_message("Категория обновлена");
    } catch (const std::exception& e) {
        fl_alert("Ошибка:\n%s", e.what());
    }
}

void MainWindow::onDeleteCategory(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);

    const char* idStr = fl_input("ID категории для удаления:", "");
    if (!idStr) return;
    int id = std::atoi(idStr);

    if (fl_choice("Удалить категорию ID %d?\n(если есть товары — будет ошибка)", "Отмена", "Удалить", nullptr, id) != 1)
        return;

    try {
        self->categoryService_->remove(id);
        self->refreshCategories();
        fl_message("Категория удалена");
    } catch (const std::exception& e) {
        fl_alert("Ошибка:\n%s", e.what());
    }
}


void MainWindow::onRefreshSuppliers(Fl_Widget*, void* data) {
    static_cast<MainWindow*>(data)->refreshSuppliers();
}

void MainWindow::onAddSupplier(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);

    const char* name = fl_input("Название поставщика:", "");
    if (!name || !*name) return;
    const char* contact = fl_input("Контактное лицо:", "");
    const char* phone = fl_input("Телефон:", "");

    try {
        self->supplierService_->create(name, contact ? contact : "", phone ? phone : "");
        self->refreshSuppliers();
        fl_message("Поставщик добавлен");
    } catch (const std::exception& e) {
        fl_alert("Ошибка:\n%s", e.what());
    }
}

void MainWindow::onEditSupplier(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);

    const char* idStr = fl_input("ID поставщика для изменения:", "");
    if (!idStr) return;
    int id = std::atoi(idStr);

    auto opt = self->supplierService_->getById(id);
    if (!opt) {
        fl_alert("Поставщик не найден");
        return;
    }

    const char* name = fl_input("Новое название:", opt->name.c_str());
    if (!name) return;
    const char* contact = fl_input("Контакт:", opt->contact.c_str());
    const char* phone = fl_input("Телефон:", opt->phone.c_str());

    try {
        self->supplierService_->update(id, name, contact ? contact : "", phone ? phone : "");
        self->refreshSuppliers();
        fl_message("Поставщик обновлён");
    } catch (const std::exception& e) {
        fl_alert("Ошибка:\n%s", e.what());
    }
}

void MainWindow::onDeleteSupplier(Fl_Widget*, void* data) {
    auto* self = static_cast<MainWindow*>(data);

    const char* idStr = fl_input("ID поставщика для удаления:", "");
    if (!idStr) return;
    int id = std::atoi(idStr);

    if (fl_choice("Удалить поставщика ID %d?", "Отмена", "Удалить", nullptr, id) != 1)
        return;

    try {
        self->supplierService_->remove(id);
        self->refreshSuppliers();
        self->refreshProducts();
        fl_message("Поставщик удалён");
    } catch (const std::exception& e) {
        fl_alert("Ошибка:\n%s", e.what());
    }
}


void MainWindow::onShowLowStock(Fl_Widget*, void* data) {
    static_cast<MainWindow*>(data)->refreshLowStock();
}

}