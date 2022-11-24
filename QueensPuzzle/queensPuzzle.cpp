#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

using namespace std;


class Queens {
    private:
        int size, solutions; // размер доски и кол-во решений
        vector<int> positionsInRow; // позиции в строка (индекс - номер строки, значение - номер колонки)
        ofstream f;

    
    public: Queens(int size) : size(size) {
        solutions = 0;
        positionsInRow.resize(size);
        for (int i = 0; i < size; i++)
            positionsInRow[i] = 0;
        f.open("answer.txt");
        answerToFile(0);
        // system("zip answer.zip answer.txt");
        // system("rm answer.txt");
        f.close();
    }


    private:
    /**
     * @brief выводит доску в файл
     * 
     */
    void show_full_board() {
        f << solutions + 1 << ":\n";
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (positionsInRow[row] == col) 
                    f << "☒";
                else 
                    f << "☐";
            }
            f << endl;
        }
        f << endl;
    }


    private:
    /**
     * @brief Проверяен наличие ферзя по диагоналям и сверху
     * 
     * @param rows номер проверяемой строки
     * @param col номер проверяемого столбца
     * @return true если ферзь найден
     * @return false если ферзь не найден
     */
    bool check_place(int rows, int col) {
        for (int i = 0; i < rows; i++) {
            // сравнение колонок, вычитание из номера колонки номер строки, сложение номера колонки и номера строки.
            if (positionsInRow[i] == col || positionsInRow[i] - i == col - rows || positionsInRow[i] + i == col + rows) {
                return true;
            }
        }
        return false;
    }


    private: 
    /**
     * @brief рекурсивная функция, расставляющая ферзей. возврящается к предыдущей строке, если ферзя невозможно поставить
     * 
     * @param target_row номер проверяемой строки
     */
    void answerToFile(int target_row) {
        if (target_row == size) {
            show_full_board();
            solutions++;
        } else {
            for (int col = 0; col < size; col++) {
                if (!check_place(target_row, col)) {
                    positionsInRow[target_row] = col;
                    answerToFile(target_row+1);
                }
            }
        }
    }
};


int main(int argc, char **argv) {

    int size;

    if (argc > 1)
        size = atoi(argv[1]);
    else
        exit(0);

    Queens queens(size);

    return 0;
}