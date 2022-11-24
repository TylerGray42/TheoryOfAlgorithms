#include <iostream>
#include <cstdio>
#include <climits>
#include <vector>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

typedef struct _Cell {
    int from, to;
    int potential; // потенциал ячейки
    int weight; // вес перехода по ячейка
    int transfer; // пройденная ячейка (стоимоть перевозки)
    int transfer_for_cycle; // пройденная ячейка в цикле
    _Cell *from_which_cell; // от куда попал в эту ячейку
} Cell;


/**
 * @brief Construct a new Grid object
 * 
 * @param grid 
 * @param x кол-во столбцов
 * @param y кол-во строк
 */
class Grid {
    
    public:
        vector<vector<Cell>> grid;
        pair<int, int> size; // столбцы, строки
        vector<Cell> gridOfWays; // путь цикла


    public: Grid(vector<vector<Cell>> g, int x, int y) : grid(g) {
        size.first = x + 1;
        size.second = y + 1;
        int i;
        int sumProdFromProvide = 0, sumProdForConsumer = 0;

        for (i = 0; i < y; i++) {
            sumProdFromProvide += grid[i][x].weight;
        }

        for (i = 0; i < x; i++) {
            sumProdForConsumer += grid[y][i].weight;
        }

        if (sumProdFromProvide > sumProdForConsumer) { // Если запаса больше потребностей
            // for (i = 0; i < y; i++) {
            //     grid[i].emplace(grid[i].end()-1, (Cell){i, size.first, INT_MIN, 0, -1});
            // }
            // grid[i].emplace(grid[i].end(), (Cell){-1, size.first, INT_MIN, sumProdFromProvide - sumProdForConsumer, -1});
            // size.first++;
            cout << "Задача не решаема пока. Запаса больше потребностей\n";
            exit(0);
        } else if (sumProdForConsumer > sumProdFromProvide) { // Если потребностей больше запаса
            // vector<Cell> step;
            // for (i = 0; i < x ; i++) {
            //     step.push_back({size.second, i, INT_MIN, 0, -1});
            // }
            // step.push_back({size.second, -1, INT_MIN, sumProdForConsumer - sumProdFromProvide, -1});
            // grid.emplace(grid.end()-1, step);
            // size.second++;
            cout << "Задача не решаема пока. Потребностей больше запаса\n";
            exit(0);
        }

    }


    public: friend ostream& operator<<(ostream&out, const Grid& g) {

        out << "         ";
        for (int i = 0; i < g.size.first - 1; i++) {
            out << "|" << setw(9) << i;
        }
        out << "|\n";
        for (int i = 0; i < g.size.second - 1; i++) {

            for (int j = 0; j <= g.size.first; j++) {
                out << "——————————";
            }


            out << "\n" << setw(9) << i;

            for (int j = 0; j < g.size.first; j++) {
                if (g.grid[i][j].potential == INT_MIN) {
                    out << "|" << setw(4) << g.grid[i][j].weight << "(" << setw(3) << "?" << ")";
                } else {
                    out << "|" << setw(4) << g.grid[i][j].weight << "(" << setw(3) << g.grid[i][j].potential << ")";
                }
            }

            out << endl;
        }

        for (int j = 0; j <= g.size.first; j++) {
            out << "——————————";
        }
        out << "\n         |";

        for (int j = 0; j < g.size.first - 1; j++) {
            if (g.grid[g.size.second-1][j].potential == INT_MIN) {
                out << setw(4) << g.grid[g.size.second-1][j].weight << "(" << setw(3) << "?" << ")" << "|";
            } else {
                out << setw(4) << g.grid[g.size.second-1][j].weight << "(" << setw(3) << g.grid[g.size.second-1][j].potential << ")" << "|";
            }
        }

        out << "\n";
        return out;
    }


    public:
    /**
     * @brief Поиск маршрутов методом минимального элемента (стоимости пути)
     * 
     */
    void min_element() {
        int min = 0, max = 0, item;
        vector<vector<Cell>> g = grid;
        st:
        int allMin = INT_MAX;
        for (int i = 0; i < size.second - 1; i++) {
            for (int j = 0; j < size.first - 1; j++) {
                item = g[i][j].weight;
                if (item > min && item <= allMin) 
                    allMin = item;
                if (item > max) max = item;
            }
        }

        for (int i = 0; i < size.second - 1; i++) {
            for (int j = 0; j < size.first - 1; j++) {
                if (g[i][j].weight == allMin) {
                    if (g[i][size.first-1].weight == 0 || g[size.second-1][j].weight == 0) continue;
                    if (g[i][size.first-1].weight > g[size.second-1][j].weight) {
                        g[i][size.first-1].weight -= g[size.second-1][j].weight;
                        grid[i][j].transfer = g[size.second-1][j].weight;
                        // cout << i << " -> " << j << " = " << g[i][j].weight * g[size.second-1][j].weight << endl;
                        g[size.second-1][j].weight = 0;
                    } else {
                        g[size.second-1][j].weight -= g[i][size.first-1].weight;
                        grid[i][j].transfer = g[i][size.first-1].weight;
                        // cout << i << " -> " << j << " = " << g[i][j].weight * g[i][size.first-1].weight << endl;
                        g[i][size.first-1].weight = 0;
                    }
                }
            }
        }

        min = allMin;
        if (min == max) {
            for (int i = 0; i < size.second - 1; i++) {
                for (int j = 0; j < size.first - 1; j++) {
                    if (grid[i][j].transfer != -1) {
                        if (checkCycle(grid[i][j])) {
                            cout << "Данная программа не может решить эту задачу\n";
                            exit(0);
                        }
                    }
                }
            }
            return;
        }
        else goto st;
    }


    public:
    /**
     * @brief 
     * 
     * @param stCell начальная ячейка
     * @param direct (от куда пришёл) 0 - нет направления, 1 - сверху, 2 - слева, 3 - снизу, 4 - справа.
     */
    void dfs(Cell &stCell, int direct) {

        // printf("(%d, %d) - %d\n", stCell.from, stCell.to, direct);

        if (direct != 3) {
            for (int i = 0; i < stCell.from; i++) { // v
                if (grid[i][stCell.to].transfer_for_cycle == -1 && grid[i][stCell.to].transfer != -1) {
                    grid[i][stCell.to].transfer_for_cycle = 1;
                    grid[i][stCell.to].from_which_cell = &stCell;
                    dfs(grid[i][stCell.to], 1);
                } else if (grid[i][stCell.to].transfer_for_cycle == 0) {
                    grid[i][stCell.to].from_which_cell = &stCell;
                    return;
                }
            }
        }

        if (direct != 1) {
            for (int i = size.second-2; i > stCell.from; i--) { // ^
                if (grid[i][stCell.to].transfer_for_cycle == -1 && grid[i][stCell.to].transfer != -1) {
                    grid[i][stCell.to].transfer_for_cycle = 1;
                    grid[i][stCell.to].from_which_cell = &stCell;
                    dfs(grid[i][stCell.to], 3);
                } else if (grid[i][stCell.to].transfer_for_cycle == 0) {
                    grid[i][stCell.to].from_which_cell = &stCell;
                    return;
                }
            }
        }

        if (direct != 4) {
            for (int i = 0; i < stCell.to; i++) { // >
                if (grid[stCell.from][i].transfer_for_cycle == -1 && grid[stCell.from][i].transfer != -1) {
                    grid[stCell.from][i].transfer_for_cycle = 1;
                    grid[stCell.from][i].from_which_cell = &stCell;
                    dfs(grid[stCell.from][i], 2);
                } else if (grid[stCell.from][i].transfer_for_cycle == 0) {
                    grid[stCell.from][i].from_which_cell = &stCell;
                    return;
                }
            }
        }

        if (direct != 2) {
            for (int i = size.first-2; i > stCell.to; i--) { // <
                if (grid[stCell.from][i].transfer_for_cycle == -1 && grid[stCell.from][i].transfer != -1) {
                    grid[stCell.from][i].transfer_for_cycle = 1;
                    grid[stCell.from][i].from_which_cell = &stCell;
                    dfs(grid[stCell.from][i], 4);
                } else if (grid[stCell.from][i].transfer_for_cycle == 0) {
                    grid[stCell.from][i].from_which_cell = &stCell;
                    return;
                }
            }
        }
    }


    public:
    /**
     * @brief Проверяет наличие цикла в таблице, с начальной позиции
     * 
     * @param cell начальная ячейка
     * 
     * @return true цикл найден
     * @return false цикл не найден
     */
    bool checkCycle(Cell &cell) {
        for (int i = 0; i < size.second - 1; i++) 
            for (int j = 0; j < size.first - 1; j++) {
                grid[i][j].transfer_for_cycle = -1;
                grid[i][j].from_which_cell = nullptr;
            }
        cell.transfer_for_cycle = 0;
        dfs(cell, 0);
        Cell c = cell;
        if (cell.from_which_cell == nullptr) {
            return false;
        } else {
            while (c.from_which_cell != &cell) {
                //printf("(%d, %d) <- ", c.from, c.to);
                c = *c.from_which_cell;
            }
        }
        //printf("(%d, %d)\n", c.from, c.to);
        return true;
    }


    public:
    /**
     * @brief Проверка перед поиском маршрутов методом потенциалов (на вырожденность) !! Доделать
     * 
     */
    void checkBeforePot() {
        int check = size.first+size.second-3;
        int checked = 0; // кол-во отмеченных ячеек
        for (int i = 0; i < size.second - 1; i++) 
            for (int j = 0; j < size.first - 1; j++) 
                if (grid[i][j].transfer != -1)
                    checked++;

        
        if (checked == check) return;
        else if (checked < check) { // кол-во пройденный ячеек меньше необходимого.
            for (int i = 0; i < size.second - 1; i++) {
                for (int j = 0; j < size.first - 1; j++) {
                    if (grid[i][j].transfer == -1 && !checkCycle(grid[i][j])) {
                        // cout << " " << grid[i][j].from << " " << grid[i][j].to << endl;
                        grid[i][j].transfer = 0;
                        checked++;
                        if (checked == check) {
                            // return;
                            checkBeforePot();
                        }
                    }
                }
            }
        } else { // кол-во пройденный ячеек больше необходимого.
            for (int i = 0; i < size.second - 1; i++) {
                for (int j = 0; j < size.first - 1; j++) {
                    if (grid[i][j].transfer == 0) {
                        grid[i][j].transfer = -1;
                        checked--;
                        if (checked == check) {
                            // return;
                            checkBeforePot();
                        }
                    }
                }
            }
        }
    }


    public:
    /**
     * @brief Поиск маршрутов методом потенциалов
     * 
     */
    void potentialMethod() {
        int n;
        cout << "Potential Method" << endl;
        checkBeforePot();
        for (int i = 0; i < size.second; i++) {
            for (int j = 0; j < size.first; j++) {
                grid[i][j].potential = INT_MIN;
            }
        }

        grid[0][size.first-1].potential = 0;
        st2:
        n = 0;
        for (int i = 0; i < size.second - 1; i++) {
            for (int j = 0; j < size.first - 1; j++) {
                if (grid[i][j].transfer != -1) {
                    n++;
                    if (grid[i][size.first - 1].potential == INT_MIN && grid[size.second - 1][j].potential == INT_MIN) {
                        continue;
                    }
                    else if (grid[i][size.first - 1].potential == INT_MIN) {
                        grid[i][size.first - 1].potential = grid[i][j].weight - grid[size.second - 1][j].potential;
                        n--;
                    } else if (grid[size.second - 1][j].potential == INT_MIN) {
                        grid[size.second - 1][j].potential = grid[i][j].weight - grid[i][size.first - 1].potential;
                        n--;
                    } else if (grid[i][size.first - 1].potential != INT_MIN && grid[size.second - 1][j].potential != INT_MIN) {
                        n--;
                    }
                    continue;
                }
            }  
        }
        if (n != 0) goto st2;

        for (int i = 0; i < size.second - 1; i++) {
            for (int j = 0; j < size.first - 1; j++) {
                grid[i][j].potential = grid[i][j].weight - (grid[i][size.first - 1].potential + grid[size.second - 1][j].potential);
            }
        }  

         // проверить, что у объекта миниммальный вес.
        Cell cell;
        n = 0;
        for (int i = 0; i < size.second - 1; i++) {
            for (int j = 0; j < size.first - 1; j++) {
                if (grid[i][j].potential < 0) {
                    if (checkCycle(grid[i][j])) {
                        cell = grid[i][j];
                        n++;
                    } else {
                        cout << "Error in potent method";
                        exit(0);
                    }
                }
            }
        }

        if (n == 0) {
            int sum = 0;
            for (int i = 0; i < size.second - 1; i++) {
                for (int j = 0; j < size.first - 1; j++) {
                    if (grid[i][j].transfer > 0)
                        sum += grid[i][j].transfer * grid[i][j].weight;
                }
            }
            cout << "Общие затраты на перевозку: " << sum << endl;
            return;
        }


        Cell c = cell;
        int min_weight = INT_MAX;
        while ((*c.from_which_cell).from != cell.from || (*c.from_which_cell).to != cell.to) {
            c = *c.from_which_cell;
            if (c.transfer < min_weight && c.transfer > 0) min_weight = c.transfer;
        }
        
        c = *cell.from_which_cell;
        grid[cell.from][cell.to].transfer += 1 + min_weight;
        n = 0;
        while (c.from != cell.from || c.to != cell.to) {
            if (n % 2 == 0) {
                grid[c.from][c.to].transfer -= min_weight;
            }
            else {
                grid[c.from][c.to].transfer += min_weight;
            }
            c = *c.from_which_cell;
            n++;
        }

        potentialMethod();

    }



};



int main(int argc, char **argv){

    int x, y, weight; // x - покупатели, y - продавцы.
    vector<vector<Cell>> grid;
    vector<Cell> step;

    if (argc == 2) {
        ifstream f(argv[1]);
        f >> x >> y;
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                f >> weight;
                step.push_back({i, j, INT_MIN, weight, -1});
            }
            f >> weight;
            step.push_back({i, -1, INT_MIN, weight, -1});
            grid.push_back(step);
            step.erase(step.begin(), step.end());
        }

        for (int i = 0; i < x; i++) {
            f >> weight;
            step.push_back({-1, i, INT_MIN, weight, -1});
        }
        grid.push_back(step);
        step.erase(step.begin(), step.end());
        f.close();
    } else {
        cout << "Введите кол-во потребителей и поставщиков: ";
        cin >> x >> y;

        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                printf("%d -> %d = ", i, j);
                cin >> weight;
                step.push_back({i, j, INT_MIN, weight, -1});
            }
            printf("Prod from %d = ", i);
            cin >> weight;
            step.push_back({i, -1, INT_MIN, weight, -1});
            grid.push_back(step);
            step.erase(step.begin(), step.end());
        }

        for (int i = 0; i < x; i++) {
            printf("Prod to %d = ", i);
            cin >> weight;
            step.push_back({-1, i, INT_MIN, weight, -1});
        }
        grid.push_back(step);
        step.erase(step.begin(), step.end());
    }

    Grid transport(grid, x, y);
    cout << endl << transport << endl;
    transport.min_element();
    cout << endl << endl;
    //transport.checkBeforePot();
    transport.potentialMethod();
    // cout << endl << transport << endl;
    //transport.checkCycle(transport.grid[1][1]);
}