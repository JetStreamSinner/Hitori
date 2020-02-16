#include <algorithm>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include "MainWindow.h"

using Index = std::pair<int, int>;

MainWindow::MainWindow(QMainWindow* parent) : QMainWindow(parent), 
 generationType(GenerationType::Prepared), generator(minimum_value, maximum_value), 
    preparedGenerator(0)
{

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    initTable();
    initMenu();
    initStatusBar();

    view->setScene(scene);
    setCentralWidget(view);
}

void MainWindow::initStatusBar()
{
    QStatusBar * statusBar = new QStatusBar(this);
    statusBarLabel = new QLabel(QString("Generation type: %0").arg(generationTypeName(generationType)));
    statusBar->addWidget(statusBarLabel);
    setStatusBar(statusBar);
}

QString MainWindow::generationTypeName(GenerationType type)
{
    switch (type)
    {
    case GenerationType::Random:
        return "Random";
    case GenerationType::Prepared:
        return "Prepared";
    default:
        return "";
    }
}

void MainWindow::setPreparedTypeGeneration()
{
    generationType = GenerationType::Prepared;
    statusBarLabel->setText(QString("Generation type: %0").arg(generationTypeName(generationType)));
}

void MainWindow::setRandomTypeGeneration()
{
    generationType = GenerationType::Random;
    statusBarLabel->setText(QString("Generation type: %0").arg(generationTypeName(generationType)));
}

void MainWindow::initMenu()
{
    QMenu * menu = new QMenu("File", this);
    QMenu * setTypeMenu = new QMenu("Generation type", this);
    menuBar()->addMenu(menu);

    QAction * preparedTypeAction = new QAction("Prepared", this);
    QAction * randomTypeAction = new QAction("RandomType", this);

    QAction * newGameAction = new QAction("New game", this);
    QAction * exitGameAction = new QAction("Exit", this);

    menu->addAction(newGameAction);
    menu->addMenu(setTypeMenu);
    menu->addSeparator();
    menu->addAction(exitGameAction);

    setTypeMenu->addAction(preparedTypeAction);
    setTypeMenu->addAction(randomTypeAction);

    connect(preparedTypeAction, SIGNAL(triggered()), this, SLOT(setPreparedTypeGeneration()));
    connect(randomTypeAction, SIGNAL(triggered()), this, SLOT(setRandomTypeGeneration()));

    connect(newGameAction, SIGNAL(triggered()), this, SLOT(resetField()));
    connect(exitGameAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::initTable()
{
    table = filledTable();
    for (auto row_index = 0; row_index < _rowCount; ++row_index)
    {
        std::for_each(table.at(row_index).begin(), table.at(row_index).end(),
            [&](auto& graphics_item) {scene->addItem(graphics_item); });
    }
}

bool MainWindow::isValid(const int row_index, const int column_index)
{
    if ((row_index >= 0 && row_index <= _rowCount - 1) && (column_index >= 0
        && column_index <= _columnCount - 1))
        return true;
    else
        return false;
}

bool MainWindow::onItemClick(int row_index, int column_index)
{
    auto item = table.at(row_index).at(column_index);


    auto checkChangeable = [&]() {
        if (isValid(row_index - 1, column_index)) {
            if (table.at(row_index - 1).at(column_index)->isChecked()) {
                return false;
            }
        }

        if (isValid(row_index + 1, column_index)) {
            if (table.at(row_index + 1).at(column_index)->isChecked()) {
                return false;
            }
        }

        if (isValid(row_index, column_index - 1)) {
            if (table.at(row_index).at(column_index - 1)->isChecked()) {
                return false;
            }
        }

        if (isValid(row_index, column_index + 1)) {
            if (table.at(row_index ).at(column_index + 1)->isChecked()) {
                return false;
            }
        }
        return true;
    };

    if (!checkChangeable()) {
        item->setChecked(false);
    } else {
        if (!item->isChecked())
            --checkedElementsCount;
        else
            ++checkedElementsCount;
    }
    if (checkWinCondition()) {
        QMessageBox::information(this, "Win", "You won this game");
        resetField();
    }

    return true;
}

void MainWindow::resetField()
{
    scene->clear();
    checkedElementsCount = 0;
    preparedGenerator.clear();
    initTable();
}

bool MainWindow::checkWinCondition()
{
    if (checkComponents() && checkRows() && checkColumns())
        return true;
    return false;
}

bool MainWindow::checkRows()
{
    for (auto i = 0; i < _rowCount; ++i)
        for (auto j = 0; j < _columnCount; ++j)
            for (auto k = j + 1; k < _columnCount; ++k) 
                if (table.at(i).at(j)->value() == table.at(i).at(k)->value() 
                    && !table.at(i).at(j)->isChecked() && !table.at(i).at(k)->isChecked()) 
                    return false;
    return true;
}

bool MainWindow::checkComponents()
{
    Index start_index;
    std::vector<std::vector<bool>> passed_elements(_rowCount);
    std::for_each(passed_elements.begin(), passed_elements.end(), 
        [&](auto& vector) { vector.resize(_columnCount); });

    for (auto i = 0; i < _rowCount; ++i) {
        for (auto j = 0; j < _columnCount; ++j) {
            if (!table.at(i).at(j)->isChecked()) {
                start_index = { i, j };
            }
        }
    }
    checkCell(passed_elements, start_index.first, start_index.second);
    int uncheckedCount = cellsCount(passed_elements);
    int trueUncheckedCount = (_rowCount * _columnCount) - checkedElementsCount;
    if (uncheckedCount != trueUncheckedCount)
        return false;
    return true;
}

int MainWindow::cellsCount(const std::vector<std::vector<bool>>& passed_elements)
{
    int uncheckedCount = 0;
    auto rowIterator = passed_elements.begin();
    const auto rowEndIterator = passed_elements.end();
    while (rowIterator != rowEndIterator) {
        auto columnIterator = rowIterator->begin();
        const auto columnEndIterator = rowIterator->end();
        while (columnIterator != columnEndIterator) {
            if (*columnIterator)
                ++uncheckedCount;
            ++columnIterator;
        }
        ++rowIterator;
    }
    return uncheckedCount;
}

void MainWindow::checkCell(std::vector<std::vector<bool>>& passed_elements, const int row_index, const int column_index)
{
    if (table.at(row_index).at(column_index)->isChecked())
        return;
    if (passed_elements.at(row_index).at(column_index))
        return;
    passed_elements.at(row_index).at(column_index) = true;

    if (isValid(row_index + 1, column_index))
        checkCell(passed_elements, row_index + 1, column_index);
    if (isValid(row_index - 1, column_index))
        checkCell(passed_elements, row_index - 1, column_index);
    if (isValid(row_index, column_index + 1))
        checkCell(passed_elements, row_index, column_index + 1);
    if (isValid(row_index, column_index - 1))
        checkCell(passed_elements, row_index, column_index - 1);
}

bool MainWindow::checkColumns()
{
    for (auto j = 0; j < _columnCount; ++j)
        for (auto i = 0; i < _rowCount; ++i)
            for (auto k = i + 1; k < _rowCount; ++k)
                if (table.at(i).at(j)->value() == table.at(k).at(j)->value()
                    && !table.at(i).at(j)->isChecked() && !table.at(k).at(j)->isChecked()) {
                    return false;
                }
    return true;
}

MainWindow::RectsTable MainWindow::filledTable()
{
    if (generationType == GenerationType::Prepared)
    {
        auto generateIndex = [&]() {
            const auto lowerBound = 0;
            const auto upperBound = preparedGenerator.sourcesCount() - 1;

            std::random_device device;
            std::default_random_engine engine(device());
            std::uniform_int_distribution<int> distribution(lowerBound, upperBound);
            return distribution(engine);
        };
        const int index = generateIndex();
        preparedGenerator.setSource(index);
        _rowCount = preparedGenerator.rowCount();
        _columnCount = preparedGenerator.columnCount();
    }

    auto fillingTable = makeTable(_rowCount, _columnCount);

    for (auto i = 0; i < _rowCount; ++i) {
        for (auto j = 0; j < _columnCount; ++j) {
            switch (generationType) {
            case GenerationType::Prepared:
                fillingTable.at(i).at(j)->setValue(preparedGenerator.produceValue());
                break;
            case GenerationType::Random:
                fillingTable.at(i).at(j)->setValue(generator.produceValue());
                break;
            }
        }
    }
    return std::move(fillingTable);
}

MainWindow::RectsTable MainWindow::makeTable(const int rowCount, const int columnCount)
{
    RectsTable output;
    output.resize(rowCount);
    std::for_each(output.begin(), output.end(), [&](auto& vector) { vector.resize(columnCount); });

    const auto side_width = width() / rowCount;
    const auto side_height = height() / columnCount;

    for (auto row_index = 0; row_index < rowCount; ++row_index)
    {
        for (auto column_index = 0; column_index < columnCount; ++column_index)
        {
            output.at(row_index).at(column_index) = new HitoriRect();

            output.at(row_index).at(column_index)->setX(column_index * side_width);
            output.at(row_index).at(column_index)->setY(row_index * side_height);
            output.at(row_index).at(column_index)->setWidth(side_width);
            output.at(row_index).at(column_index)->setHeight(side_height);
            output.at(row_index).at(column_index)->setIndex(row_index, column_index);
            connect(output.at(row_index).at(column_index), SIGNAL(stateChanged(int, int)),
                this, SLOT(onItemClick(int, int)));
        }
    }
    return std::move(output);
}