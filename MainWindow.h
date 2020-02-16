#pragma once

#include <QMainWindow>
#include <QGraphicsView>
#include <vector>
#include <random>
#include <QLabel>

#include "HitoriRect.h"

enum class GenerationType { Prepared, Random };

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    explicit MainWindow(QMainWindow* parent = nullptr);

private slots:
    bool onItemClick(int row_index, int column_index);
    void resetField();

    void setPreparedTypeGeneration();
    void setRandomTypeGeneration();
private:
    GenerationType generationType;

    int checkedElementsCount = 0;

    bool checkWinCondition();
    bool checkRows();
    bool checkColumns();
    bool checkComponents();
    bool isValid(const int row_index, const int column_index);

    int cellsCount(const std::vector<std::vector<bool>> &passed_elements);
    void checkCell(std::vector<std::vector<bool>> &passed_elements, const int row_index, const int column_index);

    void initTable();
    void initMenu();

    QString generationTypeName(GenerationType type);

    QLabel * statusBarLabel;
    void initStatusBar();

    class Generator
    {
    public:
        Generator(const int& lower_bound, const int& upper_bound) : engine(device()),
            distribution(lower_bound, upper_bound)
        {

        }

        int produceValue()
        {
            return distribution(engine);
        }
    private:
        std::random_device device;
        std::default_random_engine engine;
        std::uniform_int_distribution<int> distribution;
    };

    class PreparedGenerator
    {
    public:
        explicit PreparedGenerator(const int index) 
        {
            setSource(index);
        }

        void setSource(const int index)
        {
            switch (index)
            {
            case 0:
                currentSource = { 4, 8, 1, 6, 3, 2, 5, 7,
                                  3, 6, 7, 2, 1, 6, 5, 4,
                                  2, 3, 4, 8, 2, 8, 6, 1,
                                  4, 1, 6, 5, 7, 7, 3, 5,
                                  7, 2, 3, 1, 8, 5, 1, 2,
                                  3, 5, 6, 7, 3, 1, 8, 4,
                                  6, 4, 2, 3, 5, 4, 7, 8,
                                  8, 7, 1, 4, 2, 3, 5, 6 };
                _rowCount = 8;
                _columnCount = 8;
                break;
            case 1:
                currentSource = { 8, 6, 3, 7, 8, 4, 2, 5, 1,
                                  9, 1, 5, 9, 3, 6, 2, 4, 5,
                                  5, 1, 8, 1, 6, 2, 2, 9, 3,
                                  2, 8, 1, 3, 2, 5, 4, 2, 4,
                                  7, 9, 6, 2, 2, 1, 3, 5, 4,
                                  9, 2, 3, 4, 4, 7, 1, 6, 5,
                                  4, 7, 2, 1, 5, 3, 4, 1, 6,
                                  1, 3, 5, 2, 4, 6, 5, 7, 4, 
                                  4, 5, 4, 6, 1, 3, 6, 3, 2 };
                _rowCount = 9;
                _columnCount = 9;
                break;
            case 2:
                currentSource = { 1, 3, 1, 7, 2, 8, 1, 1, 2,
                                  3, 7, 5, 1, 6, 7, 9, 8, 2,
                                  8, 7, 6, 4, 9, 1, 3, 5, 3,
                                  5, 2, 9, 8, 7, 5, 1, 3, 1,
                                  1, 7, 4, 5, 2, 2, 8, 7, 8,
                                  9, 6, 2, 4, 1, 1, 3, 6, 7,
                                  5, 8, 1, 3, 4, 7, 8, 2, 1,
                                  4, 8, 7, 2, 4, 3, 2, 4, 1,
                                  2, 1, 1, 4, 3, 1, 5, 6, 1};
                _rowCount = 9;
                _columnCount = 9;
            case 3:
                currentSource = {4, 6, 1, 7, 5, 2, 4, 1,
                                 2, 7, 3, 2, 6, 5, 1, 7,
                                 6, 2, 4, 1, 2, 7, 6 ,3,
                                 7, 4, 5, 2, 4, 1, 3, 7,
                                 1, 7, 1, 5, 2, 3, 1, 2,
                                 3, 4, 2, 6, 1, 8, 4, 8,
                                 2, 5, 1, 3, 3, 8, 2, 4,
                                 5, 1, 4, 3, 2, 8, 6, 7};
                _rowCount = 8;
                _columnCount = 8;
                break;
            case 4:
                currentSource = {4, 7, 7, 3, 6, 8, 2, 5,
                                 7, 5, 8, 1, 4, 7, 6, 5,
                                 5, 1, 4, 8, 8, 7, 3, 3,
                                 3, 3, 3, 4, 1, 7, 7, 8,
                                 1, 4, 2, 7, 5, 5, 3, 8,
                                 3, 2, 5, 6, 7, 1, 2, 4,
                                 4, 8, 2, 2, 2, 3, 1, 6,
                                 2, 4, 1, 6, 5, 8, 8, 8};
                _rowCount = 8;
                _columnCount = 8;
                break;
            default:
                currentSource = {};
            }
        }

        int rowCount() const
        {
            return _rowCount;
        }

        int columnCount() const
        {
            return _columnCount;
        }

        int produceValue() const
        {
            if (currentIndex > currentSource.size())
                currentIndex = 0;
            return currentSource.at(currentIndex++);
        }
        
        int sourcesCount() const
        {
            return _sourcesCount;
        }

        void clear()
        {
            currentSource = {};
            currentIndex = 0;
            _rowCount = 0;
            _columnCount = 0;
        }

    private:
        std::vector<int> currentSource;
        const int _sourcesCount = 5;
        mutable int currentIndex = 0;
        int _rowCount = 0;
        int _columnCount = 0;
    };
    int _rowCount = 8;
    int _columnCount = 8;
    using RectsTable = std::vector<std::vector<HitoriRect*>>;

    RectsTable makeTable(const int rowCount, const int columnCount);
    RectsTable filledTable();


    RectsTable table;
    QGraphicsView * view = nullptr;
    QGraphicsScene * scene = nullptr;

    const int minimum_value = 0;
    const int maximum_value = 9;
    Generator generator;
    PreparedGenerator preparedGenerator;
};