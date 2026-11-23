#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>

#include "service/ProductService.hpp"
#include "service/CategoryService.hpp"
#include "service/SupplierService.hpp"

#include <memory>
#include <vector>
#include <string>

namespace ui {

class MainWindow : public Fl_Window {
public:
    MainWindow(int w, int h,
               std::shared_ptr<service::ProductService> productService,
               std::shared_ptr<service::CategoryService> categoryService,
               std::shared_ptr<service::SupplierService> supplierService);

    ~MainWindow() override = default;

private:
    std::shared_ptr<service::ProductService> productService_;
    std::shared_ptr<service::CategoryService> categoryService_;
    std::shared_ptr<service::SupplierService> supplierService_;

    Fl_Tabs* tabs_ = nullptr;

    Fl_Group* productsTab_ = nullptr;
    Fl_Table* productsTable_ = nullptr;
    Fl_Button* btnAddProduct_ = nullptr;
    Fl_Button* btnEditProduct_ = nullptr;
    Fl_Button* btnDeleteProduct_ = nullptr;
    Fl_Button* btnRefreshProducts_ = nullptr;
    Fl_Choice* filterCategory_ = nullptr;

    Fl_Group* categoriesTab_ = nullptr;
    Fl_Table* categoriesTable_ = nullptr;
    Fl_Button* btnAddCategory_ = nullptr;
    Fl_Button* btnEditCategory_ = nullptr;
    Fl_Button* btnDeleteCategory_ = nullptr;
    Fl_Button* btnRefreshCategories_ = nullptr;

    Fl_Group* suppliersTab_ = nullptr;
    Fl_Table* suppliersTable_ = nullptr;
    Fl_Button* btnAddSupplier_ = nullptr;
    Fl_Button* btnEditSupplier_ = nullptr;
    Fl_Button* btnDeleteSupplier_ = nullptr;
    Fl_Button* btnRefreshSuppliers_ = nullptr;

    Fl_Group* reportsTab_ = nullptr;
    Fl_Table* lowStockTable_ = nullptr;
    Fl_Int_Input* thresholdInput_ = nullptr;
    Fl_Button* btnShowLowStock_ = nullptr;

    std::vector<domain::Product> products_;
    std::vector<domain::Category> categories_;
    std::vector<domain::Supplier> suppliers_;
    std::vector<domain::Product> lowStock_;

    static void onAddProduct(Fl_Widget*, void* data);
    static void onEditProduct(Fl_Widget*, void* data);
    static void onDeleteProduct(Fl_Widget*, void* data);
    static void onRefreshProducts(Fl_Widget*, void* data);
    static void onFilterCategory(Fl_Widget*, void* data);

    static void onAddCategory(Fl_Widget*, void* data);
    static void onEditCategory(Fl_Widget*, void* data);
    static void onDeleteCategory(Fl_Widget*, void* data);
    static void onRefreshCategories(Fl_Widget*, void* data);

    static void onAddSupplier(Fl_Widget*, void* data);
    static void onEditSupplier(Fl_Widget*, void* data);
    static void onDeleteSupplier(Fl_Widget*, void* data);
    static void onRefreshSuppliers(Fl_Widget*, void* data);

    static void onShowLowStock(Fl_Widget*, void* data);

    void buildUi();
    void refreshProducts();
    void refreshCategories();
    void refreshSuppliers();
    void refreshLowStock();
    void fillCategoryChoice(Fl_Choice* choice, bool withAll = false);
    void fillSupplierChoice(Fl_Choice* choice, bool withNone = true);

    int selectedRow(Fl_Table* table) const;

    class ProductsTable : public Fl_Table {
    public:
        ProductsTable(int x, int y, int w, int h, MainWindow* owner);
        void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) override;
    private:
        MainWindow* owner_;
    };

    class CategoriesTable : public Fl_Table {
    public:
        CategoriesTable(int x, int y, int w, int h, MainWindow* owner);
        void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) override;
    private:
        MainWindow* owner_;
    };

    class SuppliersTable : public Fl_Table {
    public:
        SuppliersTable(int x, int y, int w, int h, MainWindow* owner);
        void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) override;
    private:
        MainWindow* owner_;
    };

    class LowStockTable : public Fl_Table {
    public:
        LowStockTable(int x, int y, int w, int h, MainWindow* owner);
        void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) override;
    private:
        MainWindow* owner_;
    };
};

}